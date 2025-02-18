# include <config.h>

# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <errno.h>
# include <error.h>

typedef struct {
  unsigned left, right, down, up, top;
} node_t;

unsigned N, NSOLS, SETSIZE, INDEX = 0, NSOLUTIONS = 7;
unsigned ***SOLUTION = NULL;

// initialize ------------------------------------------------------------------

void initialize(node_t **table,
                unsigned **solution,
                unsigned **feasible,
                unsigned nfeasible)
{
  unsigned i, j, k, item, p;

  unsigned z = N + (nfeasible * SETSIZE);
  *table = (node_t*)malloc((z + 1) * sizeof(node_t));
  *solution = (unsigned*)malloc(NSOLS * sizeof(unsigned));

  /* initialize left/right links in header */
  for ( i = 1; i <= N; i++ ) {
    /* horizontal links */
    (*table)[i - 1].right = i;
    (*table)[i].left = i - 1;

    /* vertical */
    (*table)[i].up = i;
    (*table)[i].down = i;

    /* length */
    (*table)[i].top = 0;
  }
  (*table)[0].left = N;
  (*table)[N].right = 0;

  /* initialize links in body */
  for ( k = 0; k < nfeasible; k++ ) {
    /* first item in option */
    item = feasible[k][0];
    (*table)[i].top = item;
    (*table)[i].left = i + SETSIZE - 1;
    p = (*table)[item].up;
    (*table)[i].up = p;
    (*table)[p].down = i;
    (*table)[i].down = item;
    (*table)[item].up = i;
    (*table)[item].top++;
    i++;

    /* rest of list item */
    for ( j = 1; j < SETSIZE; j++, i++ ) {
      item = feasible[k][j];
      (*table)[i].top = item;
      (*table)[i - 1].right = i;
      (*table)[i].left = i - 1;
      p = (*table)[item].up;
      (*table)[i].up = p;
      (*table)[p].down = i;
      (*table)[i].down = item;
      (*table)[item].up = i;
      (*table)[item].top++;
    }
    (*table)[i - 1].right = i - SETSIZE;
  }
}

static inline
void destroy(node_t **table, unsigned **solution)
{
  if ( *table != NULL ) free(*table);
  if ( *solution != NULL ) free(*solution);
}

// covering/uncovering----------------------------------------------------------

void cover(node_t *table, unsigned c)
{
  unsigned i, j, r, l, u, d;

  /* cover c in header */
  l = table[c].left;
  r = table[c].right;
  table[r].left = l;
  table[l].right = r;

  /* hide every item in any option containing c */
  for ( i = table[c].down; i != c; i = table[i].down )
    for ( j = table[i].right; j != i; j = table[j].right ) {
      u = table[j].up;
      d = table[j].down;
      table[d].up = u;
      table[u].down = d;
      table[table[j].top].top--;
    }
}

void uncover(node_t *table, unsigned c)
{
  unsigned i, j, r, l, u, d;

  /* unhide every item in any option containing c */
  for ( i = table[c].up; i != c; i = table[i].up )
    for ( j = table[i].left; j != i; j = table[j].left ) {
      table[table[j].top].top++;
      u = table[j].up;
      d = table[j].down;
      table[d].up = j;
      table[u].down = j;
    }

  /* uncover c in header */
  r = table[c].right;
  l = table[c].left;
  table[r].left = c;
  table[l].right = c;
}

// find min --------------------------------------------------------------------

static inline
unsigned min(node_t *table, unsigned nfeasible)
{
  unsigned c, j, s = nfeasible;
  for ( j = table[0].right; j != 0; j = table[j].right )
    if ( table[j].top < s ) {
      c = j;
      s = table[j].top;
    }
  return c;
}

// feasible --------------------------------------------------------------------

int cmp(const void *_a, const void *_b)
{
  unsigned a = *(unsigned*)_a;
  unsigned b = *(unsigned*)_b;
  if ( a < b ) return -1;
  if (a > b) return 1;
  return 0;
}

int line_cmp(const void *_a, const void *_b)
{
  unsigned *a = *(unsigned**)_a;
  unsigned *b = *(unsigned**)_b;
  for ( unsigned i = 0; i < SETSIZE; i++ ) {
    if ( a[i] == b[i] ) continue;
    if ( a[i] < b[i] ) return -1;
    return 1;
  }
}

void append_spread(node_t *table, unsigned *solution)
{
  unsigned i, j, s, index;
  for ( i = 0; i < NSOLS; i++ ) {
    s = solution[i];
    j = s;
    index = 0;
    do {
      SOLUTION[INDEX][i][index++] = table[j].top;
      j = table[j].right;
    } while ( j != s );
    qsort(SOLUTION[INDEX][i], SETSIZE, sizeof(unsigned), cmp);
  }
  qsort(SOLUTION[INDEX], SETSIZE + 2, sizeof(unsigned*), line_cmp);
}

unsigned check_intersect(unsigned *feasible)
{
  unsigned k1, k2, i, intersect;
  for ( i = 0; i < NSOLS; i++ ) {
    intersect = 0;
    for ( k1 = 0, k2 = 0; k1 < SETSIZE && k2 < SETSIZE; ) {
      if ( feasible[k1] < SOLUTION[INDEX - 1][i][k2] ) k1++;
      else if ( feasible[k1] > SOLUTION[INDEX - 1][i][k2] ) k2++;
      else {
        intersect++, k1++, k2++;
      }
      if ( intersect > 1 ) return 0;
    }
  }
  return 1;
}

unsigned **make_new_feasible(unsigned **old_feasible,
                             unsigned nfeasible,
                             unsigned *new_nfeasible)
{
  unsigned i, j, index;
  unsigned **new_feasible = (unsigned**)malloc(nfeasible * sizeof(unsigned*));
  for ( i = 0, index = 0; i < nfeasible; i++ ) {
    if ( check_intersect(old_feasible[i]) == 1 ) {
      new_feasible[index] = (unsigned*)malloc(SETSIZE * sizeof(unsigned));
      for ( j = 0; j < SETSIZE; j++ ) new_feasible[index][j] = old_feasible[i][j];
      index++;
    }
  }
  *new_nfeasible = index;
  return new_feasible;
}

static inline
void free_feasible(unsigned **feasible, unsigned nfeasible)
{
  for ( unsigned i = 0; i < nfeasible; i++ )
    if ( feasible[i] != NULL ) free(feasible[i]);
  free(feasible);
}

// algorithm X -----------------------------------------------------------------

void print(void)
{
  unsigned i, j, k;
  for ( i = 0; i < INDEX; i++ ) {
    for ( j = 0; j < SETSIZE + 2; j++ ) {
      for ( k = 0; k < SETSIZE; k++ ) printf("%u ", SOLUTION[i][j][k]);
      printf("\n");
    }
    printf("\n");
  }
  printf("\n##############################################################\n\n");
  fflush(stdout);
}

void dfs(node_t *table,
         unsigned *restrict solution,
         unsigned k,
         unsigned *restrict*restrict feasible,
         unsigned nfeasible)
{
  if ( nfeasible < SETSIZE + 2 || INDEX >= 7 ) {
    printf("CLIQUE SIZE = %u\n\n", INDEX);
    print();
    return;
  }
  /* if ( nfeasible < SETSIZE + 2 || INDEX >= 3 ) { */
  /*   if (INDEX >= 3) { */
  /*     printf("CLIQUE SIZE = %u\n\n", INDEX); */
  /*     print(); */
  /*     exit(0); */
  /*   } */
  /*   return; */
  /* } */
  if ( table[0].right == 0 ) {
    unsigned new_nfeasible = 0;
    append_spread(table, solution); INDEX++;
    unsigned **new_feasible = make_new_feasible(feasible, nfeasible,
                                                &new_nfeasible);
    node_t *new_table;
    unsigned *new_solution;
    initialize(&new_table, &new_solution, new_feasible, new_nfeasible);
    dfs(new_table, new_solution, 0, new_feasible, new_nfeasible);
    destroy(&new_table, &new_solution);
    free_feasible(new_feasible, new_nfeasible);
    INDEX--;
    return;
  }

  unsigned j, c, r;

  c = min(table, nfeasible);
  cover(table, c);

  for ( r = table[c].down; r != c; r = table[r].down ) {
    solution[k] = r;
    for ( j = table[r].right; j != r; j = table[j].right )
      cover(table, table[j].top);
    dfs(table, solution, k + 1, feasible, nfeasible);
    r = solution[k];
    c = table[r].top;
    for ( j = table[r].left; j != r; j = table[j].left )
      uncover(table, table[j].top);
  }

  uncover(table, c);
}

// driver ----------------------------------------------------------------------

int main(int argc, char **argv)
{
  if ( argc < 4 )
    error(1, errno, "USAGE: ./feasible_coverings <nitems> <noptions> <optsize> < <ifile>");

  unsigned i, j, nfeasible;

  if ( sscanf(argv[1], "%u", &N) == EOF )
    error(1, errno, "ERROR: could not read <nitems>");
  if ( sscanf(argv[2], "%u", &nfeasible) == EOF )
    error(1, errno, "ERROR: could not read <noptions>");
  if ( sscanf(argv[3], "%u", &SETSIZE) == EOF )
    error(1, errno, "ERROR: could not read <optsize>");

  SOLUTION = (unsigned***)malloc(NSOLUTIONS * sizeof(unsigned**));
  for ( i = 0; i < NSOLUTIONS; i++ ) {
    SOLUTION[i] = (unsigned**)malloc((SETSIZE + 2) * sizeof(unsigned*));
    for ( j = 0; j < SETSIZE + 2; j ++ )
      SOLUTION[i][j] = (unsigned*)malloc(SETSIZE * sizeof(unsigned));
  }

  NSOLS = N / SETSIZE;

  if ( N % SETSIZE )
    error(1, errno, "ERROR: <optsize> does not divide <nitems>");
  if ( NSOLS > nfeasible )
    error(1, errno, "ERROR: not enough options to cover items");

  unsigned **feasible = (unsigned**)malloc(nfeasible * sizeof(unsigned*));
  for ( i = 0; i < nfeasible; i++ ) {
    feasible[i] = (unsigned*)malloc(SETSIZE * sizeof(unsigned));
    for ( j = 0; j < SETSIZE; j++ )
      if ( scanf("%u", &feasible[i][j]) == EOF )
        error(1, errno, "ERROR: failed to read option %u item %u", i + 1, j + 1);
    qsort(feasible[i], SETSIZE, sizeof(unsigned), cmp);
  }

  node_t *table;
  unsigned *solution;
  initialize(&table, &solution, feasible, nfeasible);

  dfs(table, solution, 0, feasible, nfeasible);

  destroy(&table, &solution);
  free_feasible(feasible, nfeasible);

  for ( i = 0; i < NSOLUTIONS; i++ ) {
    for ( j = 0; j < SETSIZE + 2; j++ ) free(SOLUTION[i][j]);
    free(SOLUTION[i]);
  }
  free(SOLUTION);

  exit(0);
}

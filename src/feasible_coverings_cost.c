# include <config.h>

# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdint.h>
# include <time.h>
# include <errno.h>
# include <error.h>

typedef struct {
  uint32_t left, right, down, up, top;
} node_t;

long double S = 0.0;
int64_t D = 1;
uint32_t N, NSOLS, SETSIZE, INDEX = 0, NSOLUTIONS = 7, ***SOLUTION = NULL;

// initialize ------------------------------------------------------------------

void initialize(node_t **table,
                uint32_t **solution,
                uint32_t **feasible,
                uint32_t nfeasible)
{
  uint32_t i, j, k, item, p;

  uint32_t z = N + (nfeasible * SETSIZE);
  *table = (node_t*)malloc((z + 1) * sizeof(node_t));
  *solution = (uint32_t*)malloc(NSOLS * sizeof(uint32_t));

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
void destroy(node_t **table, uint32_t **solution)
{
  if ( *table != NULL ) free(*table);
  if ( *solution != NULL ) free(*solution);
}

// covering/uncovering----------------------------------------------------------

void cover(node_t *table, uint32_t c)
{
  uint32_t i, j, r, l, u, d;

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

void uncover(node_t *table, uint32_t c)
{
  uint32_t i, j, r, l, u, d;

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
uint32_t min(node_t *table, uint32_t nfeasible)
{
  uint32_t c, j, s = nfeasible;
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
  uint32_t a = *(uint32_t*)_a;
  uint32_t b = *(uint32_t*)_b;
  if ( a < b ) return -1;
  if (a > b) return 1;
  return 0;
}

int line_cmp(const void *_a, const void *_b)
{
  uint32_t *a = *(uint32_t**)_a;
  uint32_t *b = *(uint32_t**)_b;
  for ( uint32_t i = 0; i < SETSIZE; i++ ) {
    if ( a[i] == b[i] ) continue;
    if ( a[i] < b[i] ) return -1;
    return 1;
  }
}

void append_spread(node_t *table, uint32_t *solution)
{
  uint32_t i, j, s, index;
  for ( i = 0; i < NSOLS; i++ ) {
    s = solution[i];
    j = s;
    index = 0;
    do {
      SOLUTION[INDEX][i][index++] = table[j].top;
      j = table[j].right;
    } while ( j != s );
    qsort(SOLUTION[INDEX][i], SETSIZE, sizeof(uint32_t), cmp);
  }
  qsort(SOLUTION[INDEX], SETSIZE + 2, sizeof(uint32_t*), line_cmp);
}

uint32_t check_intersect(uint32_t *feasible)
{
  uint32_t k1, k2, i, intersect;
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

uint32_t **make_new_feasible(uint32_t **old_feasible,
                             uint32_t nfeasible,
                             uint32_t *new_nfeasible)
{
  uint32_t i, j, index;
  uint32_t **new_feasible = (uint32_t**)malloc(nfeasible * sizeof(uint32_t*));
  for ( i = 0, index = 0; i < nfeasible; i++ ) {
    if ( check_intersect(old_feasible[i]) == 1 ) {
      new_feasible[index] = (uint32_t*)malloc(SETSIZE * sizeof(uint32_t));
      for ( j = 0; j < SETSIZE; j++ ) new_feasible[index][j] = old_feasible[i][j];
      index++;
    }
  }
  *new_nfeasible = index;
  return new_feasible;
}

static inline
void free_feasible(uint32_t **feasible, uint32_t nfeasible)
{
  for ( uint32_t i = 0; i < nfeasible; i++ )
    if ( feasible[i] != NULL ) free(feasible[i]);
  free(feasible);
}

// Mersenne Twister ------------------------------------------------------------

/* /\* period parameters *\/ */
/* # define MT_N 624 */
/* # define MT_M 397 */
/* # define MATRIX_A 0x9908b0df */
/* # define UPPER_MASK 0x80000000 */
/* # define LOWER_MASK 0x7fffffff */

/* /\* tempering parameters *\/ */
/* # define TEMPERING_MASK_B 0x9d2c5680 */
/* # define TEMPERING_MASK_C 0xefc60000 */
/* # define TEMPERING_SHIFT_U(y) ((y) >> 11UL) */
/* # define TEMPERING_SHIFT_S(y) ((y) << 7UL) */
/* # define TEMPERING_SHIFT_T(y) ((y) << 15UL) */
/* # define TEMPERING_SHIFT_L(y) ((y) >> 18UL) */

/* uint32_t MT[MT_N];       /\* state vector *\/ */
/* uint32_t MTI = MT_N + 1; */

/* static inline */
/* void sgenrand(uint32_t seed) */
/* { */
/*   MT[0] = seed & 0xffffffff; */
/*   for ( MTI = 1; MTI < MT_N; MTI++ ) */
/*     MT[MTI] = (69069UL * MT[MTI - 1]) & 0xffffffff; */
/* } */

/* long double genrand(void) */
/* { */
/*   uint32_t y; */
/*   static uint32_t mag01[2] = {0x0, MATRIX_A}; */

/*   if ( MTI >= MT_N ) { */
/*     size_t k; */
/*     if ( MTI == MT_N + 1 ) sgenrand(4357); */

/*     for ( k = 0; k < MT_N - MT_M; k++ ) { */
/*       y = (MT[k] & UPPER_MASK) | (MT[k + 1] & LOWER_MASK); */
/*       MT[k] = MT[k + MT_M] ^ (y >> 1UL) ^ mag01[y & 0x1]; */
/*     } */
/*     for ( ; k < MT_N - 1; k++ ) { */
/*       y = (MT[k] & UPPER_MASK) | (MT[k + 1] & LOWER_MASK); */
/*       MT[k] = MT[k + MT_M - MT_N] ^ (y >> 1UL) ^ mag01[y & 0x1]; */
/*     } */
/*     y = (MT[MT_N - 1] & UPPER_MASK) | (MT[0] & LOWER_MASK); */
/*     MT[MT_N - 1] = MT[MT_M - 1] ^ (y >> 1UL) ^ mag01[y & 0x1]; */
/*     MTI = 0; */
/*   } */

/*   y = MT[MTI++]; */
/*   y ^= TEMPERING_SHIFT_U(y); */
/*   y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B; */
/*   y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C; */
/*   y ^= TEMPERING_SHIFT_L(y); */

/*   return (long double)y / (uint32_t)0xffffffff; */
/* } */

/* static inline */
/* uint32_t uniform_int(uint32_t _d) */
/* { */
/*   return genrand() * _d; */
/* } */

// C rand ----------------------------------------------------------------------

static inline
sgenrand(uint32_t seed)
{
  srand(seed);
}

static inline
long double genrand(void)
{
  return (long double)rand() / RAND_MAX;
}

static inline
uint32_t uniform_int(uint32_t _d)
{
  return genrand() * _d;
}

// algorithm X -----------------------------------------------------------------

static inline
void print(void)
{
  printf("%Lf\n", S / 3600);
  fflush(stdout);
}

_Noreturn
void dfs(node_t *table,
         uint32_t *restrict solution,
         uint32_t k,
         uint32_t *restrict*restrict feasible,
         uint32_t nfeasible)
{
  clock_t begin = clock();

  if ( nfeasible < SETSIZE + 2 || INDEX >= 3 ) {
    print();
    exit(0);
  }
  if ( table[0].right == 0 ) {
    uint32_t new_nfeasible = 0;
    append_spread(table, solution); INDEX++;
    uint32_t **new_feasible = make_new_feasible(feasible, nfeasible,
                                                &new_nfeasible);
    node_t *new_table;
    uint32_t *new_solution;
    initialize(&new_table, &new_solution, new_feasible, new_nfeasible);

    clock_t end = clock();
    long double cost = (long double)(end - begin) / CLOCKS_PER_SEC;
    S += D * cost;

    dfs(new_table, new_solution, 0, new_feasible, new_nfeasible);

    error(1, errno, "ERROR -- return failure 1");
  }

  uint32_t j, c, r, R, p;

  c = min(table, nfeasible);
  cover(table, c);

  if ( table[c].top == 0 ) {
    print();
    exit(0);
  }
  D *= table[c].top;
  if ( D < 0 ) error(1, errno, "ERROR -- overload error");

  R = uniform_int(table[c].top);
  for ( p = 0, r = table[c].down; r != c && p < R; r = table[r].down, p++ );

  solution[k] = r;
  for ( j = table[r].right; j != r; j = table[j].right )
    cover(table, table[j].top);

  clock_t end = clock();
  long double cost = (long double)(end - begin) / CLOCKS_PER_SEC;
  S += D * cost;

  dfs(table, solution, k + 1, feasible, nfeasible);

  error(1, errno, "ERROR -- return failure 2");
}

// driver ----------------------------------------------------------------------

int main(int argc, char **argv)
{
  if ( argc < 4 )
    error(1, errno, "USAGE: ./feasible_coverings <nitems> <noptions> <optsize> < <ifile>");

  uint32_t i, j, nfeasible;

  if ( sscanf(argv[1], "%u", &N) == EOF )
    error(1, errno, "ERROR: could not read <nitems>");
  if ( sscanf(argv[2], "%u", &nfeasible) == EOF )
    error(1, errno, "ERROR: could not read <noptions>");
  if ( sscanf(argv[3], "%u", &SETSIZE) == EOF )
    error(1, errno, "ERROR: could not read <optsize>");

  SOLUTION = (uint32_t***)malloc(NSOLUTIONS * sizeof(uint32_t**));
  for ( i = 0; i < NSOLUTIONS; i++ ) {
    SOLUTION[i] = (uint32_t**)malloc((SETSIZE + 2) * sizeof(uint32_t*));
    for ( j = 0; j < SETSIZE + 2; j ++ )
      SOLUTION[i][j] = (uint32_t*)malloc(SETSIZE * sizeof(uint32_t));
  }

  NSOLS = N / SETSIZE;

  if ( N % SETSIZE )
    error(1, errno, "ERROR: <optsize> does not divide <nitems>");
  if ( NSOLS > nfeasible )
    error(1, errno, "ERROR: not enough options to cover items");

  uint32_t **feasible = (uint32_t**)malloc(nfeasible * sizeof(uint32_t*));
  for ( i = 0; i < nfeasible; i++ ) {
    feasible[i] = (uint32_t*)malloc(SETSIZE * sizeof(uint32_t));
    for ( j = 0; j < SETSIZE; j++ )
      if ( scanf("%u", &feasible[i][j]) == EOF )
        error(1, errno, "ERROR: failed to read option %u item %u", i + 1, j + 1);
    qsort(feasible[i], SETSIZE, sizeof(uint32_t), cmp);
  }

  node_t *table;
  uint32_t *solution;
  initialize(&table, &solution, feasible, nfeasible);

  sgenrand(time(NULL));

  dfs(table, solution, 0, feasible, nfeasible);

  error(1, errno, "ERROR -- return failure 3");

  exit(0);
}

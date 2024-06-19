# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <error.h>
# include <errno.h>
# include <libgen.h>
# include <stdint.h>

# include <cliquer-1.21/cliquer.h>

uint8_t orthogoval(const uint32_t *restrict*restrict ag1,
                   const uint32_t *restrict*restrict ag2,
                   size_t nb, size_t lb)
{
     size_t i, j, k1, k2, intersect;
     for ( i = 0; i < nb; i++ )
          for ( j = 0; j < lb; j++ ) {
               intersect = 0;
               for ( k1 =  0, k2 = 0; k1 < lb && k2 < lb; ) {
                    if ( ag1[i][k1] == ag2[j][k2] ) {
                         intersect++, k1++, k2++;
                    }
                    else if ( ag1[i][k1] < ag2[j][k2] ) k1++;
                    else k2++;
                    if ( intersect > 2 ) return 0;
               }
          }
     return 1;
}

boolean pr(set_t s, graph_t *G, clique_options *opts)
{
     set_print(s);
     printf("\n");
     fflush(stdout);
     return TRUE;
}

int main( int argc, char **argv )
{
     if ( argc < 3 ) {
          fputs("USAGE -- requires parameters <nvertices> <order>\n", stderr);
          exit(1);
     }

     /* read in parameters */
     size_t nvertices, order, nb;
     if ( sscanf(argv[1], "%zu", &nvertices) == EOF )
          error_at_line(1, errno, basename(__FILE__), __LINE__, "sscanf failed");
     if ( sscanf(argv[2], "%zu", &order) == EOF )
          error_at_line(1, errno, basename(__FILE__), __LINE__, "sscanf failed");
     nb = order + 1;

     /* read in vertices (i.e., spreads) */
     size_t i, j, k;
     uint32_t ***vertices = (uint32_t***)malloc(nvertices * sizeof(uint32_t**));
     for ( i = 0; i < nvertices; i++ ) {
          vertices[i] = (uint32_t**)malloc(nb * sizeof(uint32_t*));
          for ( j = 0; j < nb; j++ ) {
               vertices[i][j] = (uint32_t*)malloc(order * sizeof(uint32_t));
               for ( k = 0; k < order; k++ )
                    if ( scanf("%u", &vertices[i][j][k]) == EOF )
                         error_at_line(1, errno, basename(__FILE__), __LINE__,
                                       "scanf failed");
          }
     }

     /* construct graph */
     graph_t *G = graph_new(nvertices);

     for ( i = 0; i < nvertices - 1; i++ )
          for ( j = i + 1; j < nvertices; j++ )
               if ( orthogoval(vertices[i], vertices[j], order + 1, order) > 0 )
                    GRAPH_ADD_EDGE(G, i, j);

     ASSERT(graph_test(G, NULL));

     clique_options opts = {
          .reorder_function = reorder_by_default,
          .reorder_map = NULL,
          .time_function = NULL,
          .output = NULL,
          .user_function = pr,
          .user_data = NULL,
          .clique_list = NULL,
          .clique_list_length = 0,
     };

     int maximum_size = (order == 4) ? 6 : (order + 1);
     (void)clique_unweighted_find_all(G, 2, maximum_size, FALSE, &opts);

     /* free graph data structure */
     graph_free(G);

     /* free vertices */
     for ( i = 0; i < nvertices; i++ ) {
          for ( j = 0; j < nb; j++ ) free(vertices[i][j]);
          free(vertices[i]);
     }
     free(vertices);

     exit(0);
}

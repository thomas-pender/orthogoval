# include <main.h>
# include <matrix.h>
# include <ag.h>

# define NGENS 2

int main(int argc, char **argv)
{
     if ( argc < 5 ) {
          std::cerr << "USAGE -- requires <degree> <order> <generators> <spread>\n";
          return 1;
     }

     /* read in generators of matrix group */
     matrix gens[NGENS];
     {
          std::size_t n;
          std::stringstream ss(argv[1]);
          ss >> n;

          std::fstream F;
          F.open(argv[3], std::ios::in);

          std::for_each(std::begin(gens), std::end(gens), [&](auto &x){
               x.resize(n);
               F >> x;
          });

          F.close();
     }

     /* read in canonical spread */
     AG canonical{};
     {
          std::size_t order;
          std::stringstream ss(argv[2]);
          ss >> order;

          std::fstream F;
          F.open(argv[4],std::ios::in);

          canonical.resize(order+1, order);
          F >> canonical;

          F.close();
     }

     /* calculate intersection of orbit and orthogoval pairs */
     std::size_t nvertices{0};
     std::unordered_set<AG, AG::hash_func> orbit{canonical};
     std::list<AG> prev_first{canonical};

     do {
          std::list<AG> prev_second{};
          for ( const auto & ag : prev_first )
               for ( const auto & gen : gens ) {
                    AG new_ag = gen * ag;
                    if ( auto it = orbit.find(new_ag); it == orbit.end() ) {
                         if ( orthogoval(canonical, new_ag) ) {
                              std::cout << new_ag << '\n' << std::flush;
                              nvertices++;
                         }
                         orbit.insert(new_ag);
                         prev_second.push_back(std::move(new_ag));
                    }
               }
          std::swap(prev_first, prev_second);
     } while( !prev_first.empty() );

     std::cerr << nvertices << '\n';

     return 0;
}

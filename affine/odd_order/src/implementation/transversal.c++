# include <main.h>
# include <ag.h>
# include <matrix.h>

# define NGENS 2

int main(int argc, char **argv)
{
     if ( argc < 6 ) {
          std::cerr << "USAGE -- requires <degree> <order> <modulus> <generators> <spread>\n";
          return 1;
     }

     /* read in data */
     matrix gens[NGENS];
     AG canonical{};
     {
          std::size_t degree, order;
          std::uint32_t modulus;

          std::stringstream ss(argv[1]);
          ss >> degree; ss.clear(); ss.str(argv[2]);
          ss >> order; ss.clear(); ss.str(argv[3]);
          ss >> modulus;

          std::fstream F;
          F.open(argv[4], std::ios::in);

          std::for_each(std::begin(gens), std::end(gens), [&](auto & x){
               x.set_modulus(modulus);
               x.resize(degree);
               F >> x;
          });

          F.close();
          F.open(argv[5], std::ios::in);
          canonical.resize(order + 1, order, degree);
          F >> canonical;
     }

     /* calculate intersection of orbit and orthogoval pairs */
     std::size_t nvertices{0};
     std::unordered_set<AG, AG::hash_func> orbit{canonical};
     std::list<AG> prev_first{canonical};

     do {
          std::list<AG> prev_second{};
          for ( const auto & ag : prev_first )
               for ( const auto & gen : gens ) {
                    AG new_ag{gen * ag};
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
     std::cerr << orbit.size() << '\n';

     return 0;
}

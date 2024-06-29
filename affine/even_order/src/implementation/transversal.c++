# include <main.h>
# include <matrix.h>
# include <ag.h>

# include <memory>

# define NGENS 2
# define NTHREADS 6
# define LIM 300

/* hashing shared pointers */
namespace AG_ptr
{
     struct hash_func
     {
          std::size_t operator()(const std::shared_ptr<AG> & p) const {
               return AG::hash_func()(*p);
          }
     };
     struct comp_func
     {
          bool operator()(const std::shared_ptr<AG> & p1,
                          const std::shared_ptr<AG> & p2) const {
               return *p1 == *p2;
          }
     };
}

/* driver */
int main(int argc, char **argv)
{
     if ( argc < 5 ) {
          std::cerr << "USAGE -- requires <degree> <order> <generators> <spread>\n";
          return 1;
     }

     /* read in generators of matrix group */
     std::vector<matrix> gens(NGENS);
     {
          std::size_t n;
          std::stringstream ss(argv[1]);
          ss >> n;

          std::fstream F;
          F.open(argv[3], std::ios::in);

          std::for_each(gens.begin(), gens.end(), [&](auto &x){
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
     // std::size_t nvertices{0};
     // std::unordered_set<AG, AG::hash_func> orbit{canonical};
     // std::list<AG> prev_first{canonical};

     // do {
     //      std::list<AG> prev_second{};
     //      for ( const auto & ag : prev_first )
     //           for ( const auto & gen : gens ) {
     //                AG new_ag{gen * ag};
     //                if ( auto it = orbit.find(new_ag); it == orbit.end() ) {
     //                     if ( orthogoval(canonical, new_ag) ) {
     //                          std::cout << new_ag << '\n' << std::flush;
     //                          nvertices++;
     //                     }
     //                     orbit.insert(new_ag);
     //                     prev_second.push_back(std::move(new_ag));
     //                }
     //           }
     //      prev_first = std::move(prev_second);
     // } while( !prev_first.empty() );

     // std::cout << "number of vertices = " << nvertices << '\n';

     std::uint64_t nvertices{0};
     std::unordered_set<std::shared_ptr<AG>, AG_ptr::hash_func, AG_ptr::comp_func> orbit{};
     std::list<std::shared_ptr<AG>> prev_first{};
     {
          auto canon_ptr = std::make_shared<AG>(canonical);
          orbit.insert(canon_ptr);
          prev_first.push_back(std::move(canon_ptr));
     }

     do {
          std::list<std::shared_ptr<AG>> prev_second{};
          for ( const auto & ag : prev_first )
               for ( const auto & gen : gens ) {
                    auto new_ag_ptr = std::make_shared<AG>(gen * (*ag));
                    if ( auto it = orbit.find(new_ag_ptr); it == orbit.end() ) {
                         if ( orthogoval(canonical, *new_ag_ptr) ) {
                              std::cout << *new_ag_ptr << '\n' << std::flush;
                              nvertices++;
                         }
                         orbit.insert(new_ag_ptr);
                         prev_second.push_back(std::move(new_ag_ptr));
                    }
               }
          prev_first = std::move(prev_second);
     } while ( !prev_first.empty() );

     std::cout << "number of vertices = " << nvertices << '\n';

     return 0;
}

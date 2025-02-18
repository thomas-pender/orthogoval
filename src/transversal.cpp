# include <config.h>

# include <iostream>
# include <fstream>
# include <sstream>
# include <memory>
# include <cstdint>
# include <cstddef>
# include <unordered_set>
# include <list>
# include <vector>
# include <utility>

/* project headers */
# include <libspread.h>

/* hashing shared pointers */
// namespace AG_ptr
// {
//      struct hash_func
//      {
//           std::size_t operator()(const std::shared_ptr<AG> & p) const {
//                return AG::hash_func()(*p);
//           }
//      };
//      struct comp_func
//      {
//           bool operator()(const std::shared_ptr<AG> & p1,
//                           const std::shared_ptr<AG> & p2) const {
//                return *p1 == *p2;
//           }
//      };
// }

/* driver */
int main(int argc, char **argv)
{
  if ( argc < 4 ) {
    std::cerr << "USAGE -- requires <degree> <order> <spread>\n";
    return 1;
  }

  /* construct generators of matrix group */
  std::vector<matrix> gens{};
  {
    std::size_t n;
    std::stringstream ss(argv[1]);
    ss >> n;
    gens = generators(n);
  }

  /* read in canonical spread */
  AG canonical{};
  {
    std::size_t order;
    std::stringstream ss(argv[2]);
    ss >> order;

    std::fstream F;
    F.open(argv[3],std::ios::in);

    canonical.resize(order+1, order);
    F >> canonical;
  }

  /* calculate intersection of orbit and orthogoval pairs */
  std::uint64_t nvertices{0};
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
    prev_first = std::move(prev_second);
  } while( !prev_first.empty() );

  std::cout << "number of vertices = " << nvertices << '\n';

  // std::uint64_t nvertices{0};
  // std::unordered_set<std::shared_ptr<AG>, AG_ptr::hash_func, AG_ptr::comp_func> orbit{};
  // std::list<std::shared_ptr<AG>> prev_first{};
  // {
  //      auto canon_ptr = std::make_shared<AG>(canonical);
  //      orbit.insert(canon_ptr);
  //      prev_first.push_back(std::move(canon_ptr));
  // }

  // do {
  //      std::list<std::shared_ptr<AG>> prev_second{};
  //      for ( const auto & ag : prev_first )
  //           for ( const auto & gen : gens ) {
  //                auto new_ag_ptr = std::make_shared<AG>(gen * (*ag));
  //                if ( auto it = orbit.find(new_ag_ptr); it == orbit.end() ) {
  //                     if ( orthogoval(canonical, *new_ag_ptr) ) {
  //                          std::cout << *new_ag_ptr << '\n' << std::flush;
  //                          nvertices++;
  //                     }
  //                     orbit.insert(new_ag_ptr);
  //                     prev_second.push_back(std::move(new_ag_ptr));
  //                }
  //           }
  //      prev_first = std::move(prev_second);
  // } while ( !prev_first.empty() );

  // std::cout << "number of vertices = " << nvertices << '\n';

  return 0;
}

# include <config.h>

# include <iostream>
# include <sstream>
# include <cstddef>
# include <iterator>
# include <numeric>
# include <utility>
# include <algorithm>
# include <vector>
# include <unordered_set>

# include <ag.h>
# include <solution.h>
# include <space.h>

// GLOBAL VARS -----------------------------------------------------------------

namespace Vector {
  struct hash_func
  {
    std::size_t operator()(std::vector<std::uint32_t> const& v) const {
      std::size_t seed{v.size()};
      for (auto x : v) {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }
  };
}

std::size_t N{}, ORDER{}, NONZERO{}, EXP{};
Solution SOLUTION{};
Space SPACE{};
AG SPREAD{};
std::unordered_set<std::vector<std::uint32_t>, Vector::hash_func> SUBSPACES{};

// SEARCH ----------------------------------------------------------------------

void dfs(std::uint32_t c)
{
  if ( SOLUTION.total_size() >= NONZERO || SPACE[0].second == 0 ) {
    std::vector<std::uint32_t> solution{SOLUTION.flatten()};
    if ( auto it = SUBSPACES.find(solution); it == SUBSPACES.end() ) {
      SUBSPACES.insert(std::move(solution));
      std::cout << SOLUTION << '\n' << std::flush;
    }
    return;
  }

  std::vector<std::vector<std::uint32_t> > arr{SPACE.partition(SOLUTION)};
  for ( auto const& v : arr ) {
    if (c > *(std::min_element(v.begin(), v.end())))
      continue;
    SPACE.cover(v);
    SOLUTION.push_back(v);
    if (SOLUTION.check_intersect(SPREAD))
      dfs(c);
    SOLUTION.pop_back();
    SPACE.uncover(v);
  }
}

// DRIVER ----------------------------------------------------------------------

int main(int argc, char **argv)
{
  if (argc < 2) {
    std::cerr << "USAGE: ./feasible_subspaces_imp <degree> < <spread_file>\n"
              << std::flush;
    return 1;
  }

  {
    std::stringstream ss{argv[1]};
    ss >> EXP;
  }
  N = 1UL << (EXP << 1UL);
  ORDER = 1UL << EXP;
  NONZERO = ORDER - 1;

  SPREAD.resize(ORDER + 1, ORDER);
  std::cin >> SPREAD;

  SPACE = Space{N};
  for ( std::uint32_t c{1}; c < N; c++ ) {
    SPACE.cover(c);
    SOLUTION.push_back(std::vector<std::uint32_t>{c});
    dfs(c);
    SOLUTION.pop_back();
    SPACE.uncover(c);
  }

  return 0;
}

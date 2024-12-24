# include <config.h>

# include <iostream>
# include <sstream>
# include <vector>
# include <cstddef>
# include <unordered_set>
# include <list>

# include <libspread.h>

matrix A{}, Arref{};
std::vector<std::vector<matrix> > GENS{};
std::size_t N{}, K{};

bool lex(std::vector<matrix> const& gens)
{
  std::unordered_set<matrix> orbit{Arref};
  std::list<matrix> prev_first{Arref};

  do {
    std::list<matrix> prev_second{};
    for ( auto const& x : prev_first )
      for ( auto const& g : gens ) {
        matrix new_x{(g * x).rref()};
        if ( auto it = orbit.find(new_x); it == orbit.end() ) {
          orbit.insert(new_x);
          prev_second.push_back(std::move(new_x));
        }
      }
    prev_first = std::move(prev_second);
  } while ( !prev_first.empty() );
}

void dfs(std::size_t l)
{
  if ( l == K ) {
    A.print();
    std::cout << '\n' << std::flush;
    return;
  }

  for ( std::uint32_t i{A[l - 1] + 1}; i < (1U << N); i++ ) {
    A[l] = i;
    if ( A.rank() < l + 1 ) continue;

    Arref = A.rref();
    if ( l > 1 && !lex(GENS[l + 1]) ) continue;

    dfs(l + 1);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "USAGE -- requires <degree> < <spread_file>\n";
    return 1;
  }

  std::uint32_t i;

  /* read-in degree, etc */
  std::size_t order{};
  {
    std::stringstream ss(argv[1]);
    ss >> N;
    K = N >> 1U;
    order = 1U << K;
  }
  for ( i = 0; i < 3; i++ ) GENS.append(std::vector<matrix>{});
  for ( i = 3; i <= K; i++ ) GENS.append(generators(i));

  /* read-in spread */
  // std::vector<matrix> spread_bases(order + 1, matrix{k, n});
  // {
  //   std::fstream F;
  //   F.open(argv[2], std::ios::in);
  //   for ( auto & x : spread_bases ) {
  //     F >> x;
  //     x.resize(n);
  //   }
  // }

  A.resize(K, N);
  for ( i = 1; i < (1U << N); i++ ) {
    A[0] = i;
    dfs(1);
  }

  return 0;
}

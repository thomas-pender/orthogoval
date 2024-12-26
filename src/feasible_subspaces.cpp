# include <config.h>

# include <iostream>
# include <sstream>
# include <vector>
# include <cstddef>
# include <unordered_set>
# include <list>

# include <libspread.h>

matrix A{};
std::vector<matrix> SPREAD_BASES{};
std::vector<std::vector<matrix> > GENS{};
std::size_t N{}, K{};

inline
bool intersection()
{
  std::size_t i{};
  for ( auto & basis : SPREAD_BASES ) {
    basis.nrows() = K + A.nrows();
    for ( i = 0; i < A.nrows(); i++ ) basis[i + 2] = A[i];
    if ( basis.rank() < A.nrows() + K - 1 ) return false;
  }
  return true;
}

bool lex()
{
  std::unordered_set<matrix, matrix::hash_func> orbit{A};
  std::list<matrix> prev_first{A};

  do {
    std::list<matrix> prev_second{};
    for ( auto const &x : prev_first )
      for ( auto const &g : GENS[A.nrows()] ) {
        matrix new_x{g * x};
        if ( auto it = orbit.find(new_x); it == orbit.end() ) {
          if ( new_x < A ) return false;
          orbit.insert(new_x);
          prev_second.push_back(std::move(new_x));
        }
      }
    prev_first = std::move(prev_second);
  } while ( !prev_first.empty() );

  return true;
}

void dfs(std::size_t l)
{
  if ( l == K ) {
    if ( !lex() ) return;
    A.row_space();
    std::cout << '\n' << std::flush;
    return;
  }

  for ( std::uint32_t i{A[l - 1] + 1}; i < (1U << N); i++ ) {
    A[l] = i;
    A.nrows() = l + 1;
    if ( A.rank() < A.nrows() ) continue;
    if ( !intersection() ) continue;
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
  for ( i = 0; i < 2; i++ ) GENS.push_back(std::vector<matrix>{});
  for ( i = 2; i <= K; i++ ) GENS.push_back(generators(i));

  /* read-in spread */
  for ( i = 0; i < order + 1; i++ ) {
    SPREAD_BASES.push_back(matrix{K, N});
    std::cin >> SPREAD_BASES[i];
    SPREAD_BASES[i].resize(K << 1U, N);
  }

  /* effect search */
  A.resize(K, N);
  for ( i = 1; i < (1U << N); i++ ) {
    A[0] = i;
    dfs(1);
  }

  return 0;
}

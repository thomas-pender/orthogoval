# ifndef LIBSPREAD_H_INCLUDED
# define LIBSPREAD_H_INCLUDED

# include <matrix.h>
# include <ag.h>

# define NGENS 2

inline
std::vector<matrix> generators(std::size_t n) {
  std::vector<matrix> gens(NGENS, matrix{n});

  gens[0] = identity_matrix32(n);
  gens[0][0] |= (1U << (n - 2));

  gens[1][0] = 1U;
  for (std::size_t i{1}; i < n; i++)
    gens[1][i] |= (1U << (n - i));

  return gens;
}

# endif

# include <config.h>

# include <matrix.h>

# if HAVE_GXX
static inline
std::uint32_t popcount(std::uint32_t n)
{
  return (std::uint32_t)__builtin_popcountl(n);
}
# else
# define B2(n) n, n+1, n+1, n+2
# define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
# define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)

static inline
std::uint32_t popcount(std::uint32_t n)
{
  static std::uint32_t _table[256]={B6(0), B6(1), B6(1), B6(2)};
  return _table[n & 0xff] + _table[(n >> 8) & 0xff] + _table[(n >> 16) & 0xff]
    + _table[(n >> 24) & 0xff];
}
# endif

std::uint32_t operator*(matrix const& A, std::uint32_t v)
{
  std::uint32_t retval=0;
  for (std::size_t i{0}; i<A.M.size(); i++)
    if ((std::uint32_t)popcount(A.M[i]&v) & 1UL)
      retval |= (1UL << (A.M.size()-i-1));
  return retval;
}

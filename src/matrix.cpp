# include <config.h>

# include <climits>

# include <matrix.h>

# if HAVE_GXX

std::uint32_t popcount(std::uint32_t n)
{
  return (std::uint32_t)__builtin_popcountl(n);
}

static inline
auto sig_bit(std::uint32_t n)
{
  return 1U << (31 - (std::uint32_t)__builtin_clz(n));
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

static inline
auto sig_bit(std::uint32_t n)
{
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n = ((n + 1) >> 1) | (n & (1 << ((sizeof(n) * CHAR_BIT)-1)));
  return n;
}

# endif

matrix operator*(matrix const& A, matrix const& B)
{
  assert(A.dim() == B.dim());
  std::size_t i,j;
  matrix C{A.dim()}, BT{B.transpose()};
  for ( i = 0; i < A.dim(); i++ )
    for ( j = 0; j < A.dim(); j++ )
      if ( (std::uint32_t)popcount(A[i] & BT[j]) & 1U )
        C[i] |= (1U << (A.dim() - j - 1));
  return C;
}

std::uint32_t operator*(matrix const& A, std::uint32_t v)
{
  std::uint32_t u{0};
  for ( std::size_t i{0}; i < A.dim(); i++ )
    if ( (std::uint32_t)popcount(A[i] & v) & 1U )
      u |= (1U << (A.dim() - i -1));
  return u;
}

matrix matrix::transpose() const
{
  matrix A{dim()};
  std::size_t i,j;
  for ( i = 0; i < dim(); i++ )
    for ( j = 0; j < dim(); j++ )
      if ( M[j] & (1U << (dim() - i - 1)) )
        A[i] |= (1U << (dim() - j - 1));
  return A;
  }

matrix matrix::inverse() const
{
  matrix A{M}, I{identity_matrix32(dim())};

  std::uint32_t max_bit;
  int i,j,max_index;
  for ( i = 0; i < dim() - 1; i++ ) {
    max_index = i;
    for ( j = i + 1; j < dim(); j++ )
      if ( A[max_index] < A[j] ) max_index = j;
    if ( max_index != i ) {
      std::swap(A[i], A[max_index]);
      std::swap(I[i], I[max_index]);
    }

    max_bit = sig_bit(A[i]);
    for ( j = 0; j < dim(); j++ ) {
      if ( (j != i) && (max_bit & A[j]) ) {
        A[j] ^= A[i];
        I[j] ^= I[i];
      }
    }
  }

  for ( i = dim() - 1; i > 0; i-- ) {
    for ( j = 0; j < i; j++ )
      if ( A[j] & (1U << dim() - i - 1) ) {
        A[j] ^= A[i];
        I[j] ^= I[i];
      }
  }

  return I;
}

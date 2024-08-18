# ifndef MATRIX_H_INCLUDED
# define MATRIX_H_INCLUDED

# include <iostream>
# include <fstream>
# include <vector>
# include <algorithm>
# include <cstddef>
# include <iterator>
# include <cassert>

class matrix
{
  typedef typename std::vector<std::uint32_t> container;
  typedef typename container::iterator iterator;
  typedef typename container::const_iterator const_iterator;

  friend std::ostream& operator<<(std::ostream& os, matrix const& A) {
    std::copy(A.cbegin(),A.cend(),
              std::ostream_iterator<std::uint32_t>(os," "));
    os << '\n';
    return os;
  }

  friend std::istream& operator>>(std::istream& is, matrix& A) {
    std::for_each(A.begin(),A.end(),[&](auto &x){is >> x;});
    return is;
  }

  friend matrix operator*(matrix const&, matrix const&);
  friend std::uint32_t operator*(matrix const&, std::uint32_t);

public:
  struct hash_func
  {
    std::size_t operator()(matrix const& A) const {
      std::size_t seed{A.dim()};
      for (auto x : A.data()) {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }
  };

  matrix transpose() const;

  bool operator==(matrix const& A) const {return M == A.M;}
  auto operator<=>(matrix const& A) const {return M <=> A.M;}

  matrix& operator*=(matrix const& A) {
    assert(dim() == A.dim());
    *this = *this * A;
    return *this;
  }

  void print(std::fstream &F) const {
    std::size_t i,j;
    for ( i = 0; i < dim(); i++ ) {
      for ( j = 0; j < dim(); j++ )
        F << at(i, j) << " ";
      F << '\n';
    }
  }

  matrix inverse() const;

  std::size_t dim() const {return M.size();}

  std::uint32_t& operator[](std::size_t i) & {return M[i];}
  std::uint32_t operator[](std::size_t i) && {return M[i];}
  std::uint32_t const& operator[](std::size_t i) const& {return M[i];}

  container const& data() const& {return M;}

  std::uint32_t at(std::size_t i, std::size_t j) const
  {return (M[i] & (1U << (dim() - j - 1))) ? 1 : 0;}

  void resize(std::size_t n) {M.resize(n);}

  iterator begin() & {return M.begin();}
  iterator begin() && {return M.begin();}
  iterator end() & {return M.end();}
  iterator end() && {return M.end();}
  const_iterator cbegin() && {return M.cbegin();}
  const_iterator cbegin() const& {return M.cbegin();}
  const_iterator cend() && {return M.cend();}
  const_iterator cend() const& {return M.cend();}

  matrix& operator=(matrix && A) & noexcept = default;
  matrix& operator=(matrix const& A) & = default;

  matrix() = default;
  matrix(matrix const&) = default;
  matrix(matrix &&) noexcept = default;
  explicit matrix(container const& v) : M{v} {}
  explicit matrix(container && v) : M{std::move(v)} {}
  explicit matrix(std::size_t n) : M(n,0) {}
  ~matrix() = default;

private:
  container M;
};

inline matrix identity_matrix32(std::size_t n)
{
  matrix I{n};
  for ( std::size_t i{0}; i < n; i++ )
    I[i] |= (1U << (n - i -1));
  return I;
}

inline matrix zeros_matrix32(std::size_t n)
{
  matrix Z{n};
  return Z;
}

inline matrix ones_matrix32(std::size_t n)
{
  matrix J{n};
  std::size_t i,j;
  for ( i = 0; i < n; i++ )
    for ( j = 0; j < n; j++ )
      J[i] |= (1U << (n - j - 1));
  return J;
}

# endif

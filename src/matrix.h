# ifndef MATRIX_H_INCLUDED
# define MATRIX_H_INCLUDED

# include <iostream>
# include <fstream>
# include <vector>
# include <algorithm>
# include <cstddef>
# include <iterator>
# include <cassert>
# include <type_traits>
# include <utility>

class matrix
{
  using dimensions = std::pair<std::size_t, std::size_t>;
  using container = std::vector<std::uint32_t>;
  using iterator = container::iterator;
  using const_iterator = container::const_iterator;

  friend std::ostream& operator<<(std::ostream& os, matrix const& A) {
    std::copy(A.begin(), A.end(),
              std::ostream_iterator<std::uint32_t>(os, " "));
    os << '\n';
    return os;
  }

  friend std::istream& operator>>(std::istream& is, matrix& A) {
    std::for_each(A.begin(), A.end(), [&](auto &x){is >> x;});
    return is;
  }

  friend matrix operator*(matrix const&, matrix const&);
  friend std::uint32_t operator*(matrix const&, std::uint32_t);
  friend std::uint32_t operator*(std::uint32_t v, matrix const& A) {
    return A.transpose() * v;
  }

public:
  struct hash_func
  {
    std::size_t operator()(matrix const& A) const {
      std::size_t seed{A.nrows()};
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
    assert(ncols() == A.nrows());
    *this = *this * A;
    return *this;
  }

  void print(std::ostream &os) const {
    std::size_t i, j;
    for (i = 0; i < nrows(); i++) {
      for (j = 0; j < ncols(); j++)
        os << at(i, j) << " ";
      os << '\n';
    }
  }

  matrix inverse() const;
  matrix gauss_form() const;
  matrix rref() const;
  std::size_t rank() const;

  void row_space() const {
    for (std::uint32_t v{1}; v < (1U << rank()); v++)
      std::cout << v * (*this) << " ";
  }

  dimensions& shape() & { return dims; }
  dimensions shape() && { return std::move(dims); }
  dimensions const& shape() const& { return dims; }

  std::size_t& nrows() & { return dims.first; }
  std::size_t const& nrows() const& { return dims.first; }

  std::size_t& ncols() & { return dims.second; }
  std::size_t const& ncols() const& { return dims.second; }

  std::uint32_t& operator[](std::size_t i) & { return M[i]; }
  std::uint32_t operator[](std::size_t i) && { return std::move(M[i]); }
  std::uint32_t const& operator[](std::size_t i) const& { return M[i]; }

  container& data() & { return M; }
  container data() && { return std::move(M); }
  container const& data() const& { return M; }

  std::uint32_t at(std::size_t i, std::size_t j) const {
    return (M[i] & (1U << (ncols() - j - 1))) ? 1 : 0;
  }

  void resize(std::size_t n) {
    M.resize(n);
    dims.first = n;
    dims.second = n;
  }
  void resize(std::size_t i, std::size_t j) {
    M.resize(i);
    dims.first = i;
    dims.second = j;
  }

  iterator begin() & { return M.begin(); }
  const_iterator begin() const& { return M.begin(); }

  iterator end() & { return M.end(); }
  const_iterator end() const& { return M.end(); }

  matrix& operator=(matrix && A) & noexcept = default;
  matrix& operator=(matrix const& A) & = default;

  matrix() = default;
  matrix(matrix const&) = default;
  matrix(matrix &&) noexcept = default;
  ~matrix() = default;

  template <typename T>
  requires std::is_convertible_v<T, container>
  explicit matrix(T &&v) : M{std::forward<T>(v)}, dims{M.size(), M.size()} {}

  template <typename T>
  requires std::is_convertible_v<T, container>
  matrix(T&& v, std::size_t j) : M{std::forward<T>(v)}, dims{M.size(), j} {}

  explicit matrix(std::size_t n) : M(n, 0), dims{n, n} {}
  matrix(std::size_t i, std::size_t j) : M(i,0), dims{i, j} {}

private:
  container M;
  dimensions dims;
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
  std::size_t i, j;
  for ( i = 0; i < n; i++ )
    for ( j = 0; j < n; j++ )
      J[i] |= (1U << (n - j - 1));
  return J;
}

# endif

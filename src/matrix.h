/**
   @file 
 */
# ifndef MATRIX_H
# define MATRIX_H

# include <iostream>
# include <cstdint>
# include <cstddef>
# include <vector>
# include <iterator>
# include <utility>
# include <algorithm>

class matrix
{
  typedef typename std::vector<std::uint32_t> container;

  friend std::uint32_t operator*(matrix const&, std::uint32_t);

  friend std::ostream& operator<<(std::ostream& os, matrix const& A) {
    std::copy(A.M.begin(),A.M.end(),
              std::ostream_iterator<std::uint32_t>(os," "));
    os << '\n';
    return os;
  }
  friend std::istream& operator>>(std::istream& is, matrix& A) {
    std::for_each(A.M.begin(),A.M.end(),[&](auto &x){is >> x;});
    return is;
  }

public:
  void resize(std::size_t n) {M.resize(n);}

  std::uint32_t& operator[](std::size_t i) & {return M[i];}
  std::uint32_t operator[](std::size_t i) && {return M[i];}
  std::uint32_t operator[](std::size_t i) const& {return M[i];}

  matrix& operator=(matrix &&) & noexcept = default;
  matrix& operator=(matrix const&) & = default;

  matrix() = default;
  matrix(matrix const&) = default;
  matrix(matrix &&) noexcept = default;
  explicit matrix(std::size_t n)
    : M(n,0) {}
  explicit matrix(container const& v)
    : M{v} {}
  explicit matrix(container && v)
    : M{std::move(v)} {}
  ~matrix() = default;

private:
  container M;
};

# endif

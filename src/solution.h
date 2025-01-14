# ifndef SOLUTION_H_INCLUDED
# define SOLUTION_H_INCLUDED

# include <iostream>
# include <iterator>
# include <algorithm>
# include <numeric>
# include <cstddef>
# include <vector>
# include <type_traits>
# include <utility>

# include <ag.h>

class Solution
{
  using array = std::vector<std::uint32_t>;
  using iterator = std::vector<array>::iterator;
  using const_iterator = std::vector<array>::const_iterator;

  friend std::ostream& operator<<(std::ostream& os, Solution const& S)
  {
    for ( auto const& x : S.M )
      std::copy(x.cbegin(), x.cend(),
                std::ostream_iterator<std::uint32_t>(os, " "));
    return os;
  }

public:
  void resize(std::size_t n) { M.resize(n); }
  std::size_t size() const { return M.size(); }
  std::size_t total_size() const
  {
    return std::accumulate(M.begin(), M.end(), 0, [&](std::size_t n, array a)
    {
      return n + a.size();
    });
  }

  array flatten() const
  {
    array v{};
    for ( auto const& x : M ) v.insert(v.end(), x.begin(), x.end());
    std::sort(v.begin(),v.end());
    return v;
  }

  bool check_intersect(AG &spread) const
  {
    std::size_t i1{}, i2{}, intersection{};
    for ( auto const& x1 : spread ) {
      intersection = 0;
      for ( auto const& x2 : M )
        for ( i1 = 0, i2 = 0; i1 < x1.size() && i2 < x2.size(); ) {
          if ( x1[i1] == x2[i2] ) { intersection++; i1++; i2++; }
          else if ( x1[i1] < x2[i2] ) i1++;
          else i2++;
          if ( intersection > 1 ) return false;
        }
    }
    return true;
  }

  template<typename... Args>
  void emplace_back(Args&&... args)
  {
    M.emplace_back(std::forward<Args>(args)...);
  }

  template <typename T>
  requires std::is_convertible_v<T, array>
  void push_back(T&& v) { M.push_back(std::forward<T>(v)); }

  void pop_back() { M.pop_back(); }

  array& operator[](std::size_t i) & { return M[i]; }
  array const& operator[](std::size_t i) const& { return M[i]; }
  array operator[](std::size_t i) && { return std::move(M[i]); }

  iterator begin() & { return M.begin(); }
  const_iterator begin() const& { return M.begin(); }

  iterator end() & { return M.end(); }
  const_iterator end() const& { return M.end(); }

  Solution& operator=(Solution const&) & = default;
  Solution& operator=(Solution &&) & noexcept = default;

  Solution() = default;
  Solution(Solution const&) = default;
  Solution(Solution &&) noexcept = default;
  ~Solution() = default;

private:
  std::vector<array> M;
};

# endif

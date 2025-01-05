# ifndef SOLUTION_H_INCLUDED
# define SOLUTION_H_INCLUDED

# include <iostream>
# include <iterator>
# include <algorithm>
# include <numeric>
# include <cstddef>
# include <vector>

class Solution
{
  typedef typename std::vector<std::uint32_t> array;
  typedef typename std::vector<array>::iterator iterator;
  typedef typename std::vector<array>::const_iterator const_iterator;

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
    std::sort(v.begin(), v.end());
    return v;
  }

  void emplace_back(array && v) { M.emplace_back(std::move(v)); }
  void push_back(array const& v) { M.push_back(v); }
  void push_back(array && v) { M.push_back(std::move(v)); }
  void pop_back() { M.pop_back(); }

  array& operator[](std::size_t i) & { return M[i]; }
  array const& operator[](std::size_t i) const& { return M[i]; }
  array operator[](std::size_t i) && { return std::move(M[i]); }

  iterator begin() & { return M.begin(); }
  iterator end() & { return M.end(); }
  const_iterator cbegin() const { return M.cbegin(); }
  const_iterator cend() const { return M.cend(); }

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

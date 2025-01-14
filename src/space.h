# ifndef SPACE_H_INCLUDED
# define SPACE_H_INCLUDED

# include <algorithm>
# include <cstddef>
# include <iostream>
# include <vector>
# include <utility>

# include <solution.h>

class Space
{
  using cell = std::pair<std::uint32_t, std::uint32_t>;
  using array = std::vector<cell>;
  using uarray = std::vector<std::uint32_t>;

public:
  void cover(std::uint32_t c)
  {
    std::uint32_t first{M[c].first}, second{M[c].second};
    M[first].second = second;
    M[second].first = first;
    size--;
  }

  void cover(uarray const& v)
  {
    for ( auto it = v.begin(); it != v.end(); it++ ) cover(*it);
  }

  void uncover(std::uint32_t c)
  {
    std::uint32_t first{M[c].first}, second{M[c].second};
    M[first].second = c;
    M[second].first = c;
    size++;
  }

  void uncover(uarray const& v)
  {
    for ( auto it = v.rbegin(); it != v.rend(); it++ ) uncover(*it);
  }

  std::vector<uarray> partition(Solution const& S)
  {
    std::size_t Ssize{S.total_size() + 1}, index{}, arr_index{0};
    std::vector<uarray> arr(size / Ssize);

    std::uint32_t c{};
    while ( (c = M[0].second) != 0 ) {
      uarray v(Ssize);

      index = 0;
      v[index++] = c;
      for ( auto it1 = S.begin(); it1 != S.end(); it1++ )
        for ( auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++ )
          v[index++] = c ^ (*it2);

      cover(v);

      std::sort(v.begin(), v.end());
      arr[arr_index++] = std::move(v);
    }

    for ( auto it = arr.rbegin(); it != arr.rend(); it++ ) uncover(*it);

    return arr;
  }

  cell& operator[](std::size_t i) & { return M[i]; }
  cell const& operator[](std::size_t i) const& { return M[i]; }
  cell operator[](std::size_t i) && { return std::move(M[i]); }

  Space& operator=(Space const&) & = default;
  Space& operator=(Space &&) & noexcept = default;

  Space() = default;
  Space(Space const&) = default;
  Space(Space &&) noexcept = default;
  explicit Space(std::size_t n)
    : M(n), size{n}
  {
    for ( std::uint32_t i{1}; i < n; i++ ) {
      M[i - 1].second = i;
      M[i].first = i - 1;
    }
    M[0].first = n - 1;
    M[n - 1].second = 0;
  }
  ~Space() = default;

private:
  array M;
  std::size_t size;
};

# endif

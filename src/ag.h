# ifndef AG_H
# define AG_H

# include <iostream>
# include <cstdint>
# include <cstddef>
# include <vector>
# include <algorithm>
# include <iterator>
# include <utility>

/* project headers */
# include <matrix.h>

class AG
{
  friend AG operator*(matrix const&, AG const&);

  friend std::ostream& operator<<(std::ostream& os, AG const& ag) {
    std::for_each(ag.spread.begin(),ag.spread.end(),[&](const auto &x){
      std::copy(x.begin(),x.end(),
                std::ostream_iterator<std::uint32_t>(os," "));
      os << '\n';
    });
    return os;
  }
  friend std::istream& operator>>(std::istream& is, AG& ag) {
    std::for_each(ag.spread.begin(),ag.spread.end(),[&](auto &x){
      for (std::size_t i{0}; i<x.size(); i++) is >> x[i];
      std::sort(x.begin(),x.end());
    });
    std::sort(ag.spread.begin(),ag.spread.end());
    return is;
  }

  friend bool operator==(AG const& ag1, AG const& ag2) {
    return ag1.spread==ag2.spread;
  }
  friend bool operator<(AG const& ag1, AG const& ag2) {
    return ag1.spread < ag2.spread;
  }

  friend bool orthogoval(AG const&, AG const&);

public:
  struct hash_func
  {
    std::size_t vec_hash(std::vector<std::uint32_t> const& v) const {
      std::size_t seed{v.size()};
      for (auto x : v) {
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }

    std::size_t operator()(AG const& ag) const {
      std::size_t seed{0};
      std::for_each(ag.spread.begin(),ag.spread.end(),[&](const auto &x){
        seed ^= vec_hash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      });
      return seed;
    }
  };

  void resize(std::size_t nb,std::size_t lb) {
    spread.resize(nb);
    std::for_each(spread.begin(),spread.end(),[&](auto &x){
      x.resize(lb);
    });
  }

  AG& operator=(AG &&) & noexcept = default;
  AG& operator=(AG const&) & = default;

  AG() = default;
  AG(AG const&) = default;
  AG(AG &&) noexcept = default;
  AG(std::size_t nb, std::size_t lb) : spread(nb) {
    std::for_each(spread.begin(),spread.end(),[this,lb](auto &x){
      x.resize(lb);
    });
  }
  ~AG() = default;

private:
  std::vector<std::vector<std::uint32_t>> spread;
};

bool orthogoval(AG const&, AG const&);

# endif

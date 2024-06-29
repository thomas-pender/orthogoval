# ifndef AG_H
# define AG_H

# include <main.h>
# include <matrix.h>

class AG
{
     friend AG operator*(const matrix&, const AG&);
     friend std::ostream& operator<<(std::ostream&, const AG&);
     friend std::istream& operator>>(std::istream&, AG&);
     friend bool operator==(const AG&, const AG&);
     friend bool operator<(const AG&, const AG&);
     friend bool orthogoval(const AG&, const AG&);

public:
     struct hash_func
     {
          std::size_t vec_hash(const std::vector<std::uint32_t> &v) const {
               std::size_t seed{v.size()};
               for (auto x : v) {
                    x = ((x >> 16) ^ x) * 0x45d9f3b;
                    x = ((x >> 16) ^ x) * 0x45d9f3b;
                    x = (x >> 16) ^ x;
                    seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
               }
               return seed;
          }

          std::size_t operator()(const AG &ag) const {
               std::size_t seed{0};
               std::for_each(ag.spread.begin(),ag.spread.end(),[&](const auto &x){
                    seed ^= vec_hash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
               });
               return seed;
          }
     };

     void resize(std::size_t,std::size_t);

     AG& operator=(const AG&);
     AG& operator=(AG&&);

     AG()=default;
     AG(std::size_t, std::size_t);
     AG(const AG&);
     AG(AG&&);
     ~AG()=default;

private:
     std::vector<std::vector<std::uint32_t>> spread;
};

bool orthogoval(const AG&, const AG&);

# endif

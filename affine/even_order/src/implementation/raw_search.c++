# include <main.h>
# include <matrix.h>
# include <ag.h>

# define NTHREADS 6
# define LEN 256
# define DIM 8

// unordered set ///////////////////////////////////////////////////////////////

std::mutex mtx1, mtx2;
std::unordered_set<AG, AG::hash_func> orth;

void insert(AG && ag)
{
     std::lock_guard<std::mutex> lock(mtx1);
     orth.insert(ag);
}

bool find(const AG & ag)
{
     std::lock_guard<std::mutex> lock(mtx1);
     if ( auto it = orth.find(ag); it == orth.end() )
          return true;
     return false;
}

void print(const AG & ag)
{
     std::lock_guard<std::mutex> lock(mtx2);
     std::cout << ag << '\n' << std::flush;
}

// dependency checks ///////////////////////////////////////////////////////////

inline bool
dep2(std::size_t a, std::size_t b)
{
     /* check a and b are independent */
     return a == b;
}

inline bool
dep3(std::size_t a, std::size_t b, std::size_t c)
{
     /* assumes a and b are independent */
     return (c == a) || (c == b) || (c == (a^b));
}

inline bool
dep4(std::size_t a, std::size_t b, std::size_t c, std::size_t d)
{
     /* assumes a, b, and c are independent */
     return (d == a) || (d == b) || (d == c) ||
          (d == (a^b)) || (d == (a^c)) || (d == (b^c)) ||
          (d == (a^b^c));
}

inline bool
dep5(std::size_t a, std::size_t b, std::size_t c, std::size_t d, std::size_t e)
{
     /* assumes a, b, c, and d are independent */
     return (e == a) || (e == b) || (e == c) || (e == d) ||
          (e == (a^b)) || (e == (a^c)) || (e == (a^d)) || (e == (b^c)) || (e == (b^d)) || (e == (c^d)) ||
          (e == (a^b^c)) || (e == (a^b^d)) || (e == (a^c^d)) || (e == (b^c^d)) || (e == (a^b^c^d));
}

inline bool
dep6(std::size_t a, std::size_t b, std::size_t c,
     std::size_t d, std::size_t e, std::size_t f)
{
     /* only checks equality */
     return (f == a) || (f == b) || (f == c) || (f == d) || (f == e);
}

inline bool
dep7(std::size_t a, std::size_t b, std::size_t c,
     std::size_t d, std::size_t e, std::size_t f, std::size_t g)
{
     /* only checks equality */
     return (g == a) || (g == b) || (g == c) ||
          (g == d) || (g == e) || (g == f);
}

inline std::uint32_t
sig_bit(std::uint32_t n)
{
     return 1U << (31 - __builtin_clz(n));
}

/* row reduce to check invertibility */
bool reduce(const std::vector<std::uint32_t> & u)
{
     std::vector<std::uint32_t> v{u};
     std::size_t max_bit;
     std::vector<std::uint32_t>::iterator it1, it2;
     for ( it1 = v.begin(); it1 != v.end() - 1; it1++ ) {
          /* find maximum row among those not processed
           * and swap to first unprocessed position */
          it2 = std::max_element(it1, v.end());
          std::swap(*it1, *it2);

          /* get significant bit */
          max_bit = sig_bit(*it1);

          /* reduce lower rows */
          for ( it2 = it1 + 1; it2 != v.end(); it2++ ) {
               if ( max_bit & *it2 ) *it2 ^= *it1;
               if ( *it2 == 0 ) return true;
          }
     }
     return false;
}

// threads /////////////////////////////////////////////////////////////////////

void thread_func(std::uint32_t start, std::uint32_t end, const AG & canonical)
{
     matrix A{DIM};
     std::uint32_t a,b,c,d,e,f,g,h;
     for ( a = start; a < end; a++ )
          for ( b = 0; b < LEN; b++ ) {
               if ( dep2(a,b) ) continue;
               for ( c = 0; c < LEN; c++ ) {
                    if ( dep3(a,b,c) ) continue;
                    for ( d = 0; d < LEN; d++ ) {
                         if ( dep4(a,b,c,d) ) continue;
                         for ( e = 0; e < LEN; e++ ) {
                              if( dep5(a,b,c,d,e) ) continue;
                              for ( f = 0; f < LEN; f++ ) {
                                   if ( dep6(a,b,c,d,e,f) ) continue;
                                   for ( g = 0; g < LEN; g++ ) {
                                        if ( dep7(a,b,c,d,e,f,g) ) continue;
                                        for ( h = 0; h < LEN; h++ ) {
                                             std::vector<std::uint32_t> v{a,b,c,d,e,f,g,h};
                                             if ( reduce(v) ) continue;
                                             A.swap(std::move(v));
                                             AG new_ag{A * canonical};
                                             if ( find(new_ag) ) {
                                                  if ( orthogoval(new_ag, canonical) ) {
                                                       print(new_ag);
                                                       insert(std::move(new_ag));
                                                  }
                                             }
                                        }
                                   }
                              }
                         }
                    }
               }
          }
}

// driver //////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
     if ( argc < 3 ) {
          std::cerr << "USAGE -- <start> <end>\n";
          return 1;
     }

     std::uint32_t start, end, tot, block_len;
     {
          std::stringstream ss(argv[1]);
          ss >> start;
          ss.clear(); ss.str(argv[2]);
          ss >> end;
          tot = end - start;
          block_len = tot / NTHREADS;
     }

     AG canonical{DIM + 1, DIM};
     {
          std::fstream F("./var/ord8/spread.8.txt", std::ios::in);
          F >> canonical;
          F.close();
     }

     std::uint32_t first;
     std::vector<std::thread> threads(NTHREADS - 1);
     for ( std::uint32_t i{0}; i < NTHREADS - 1; i++ ) {
          first = i * block_len;
          threads[i] = std::thread(thread_func, first, first + block_len, canonical);
     }

     first = (NTHREADS - 1) * block_len;
     thread_func(first, end, canonical);

     for ( auto & t : threads )
          if ( t.joinable() ) t.join();

     return 0;
}

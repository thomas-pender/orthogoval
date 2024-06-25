# include <main.h>
# include <matrix.h>
# include <ag.h>

# define NTHREADS 6
# define LEN 64
# define DIM 6
# define ORDER 8

// hash table //////////////////////////////////////////////////////////////////

std::mutex mtx;
std::unordered_set<AG, AG::hash_func> orth;

std::uint64_t count = 0;

void inc(void)
{
     std::lock_guard<std::mutex> lock(mtx);
     count++;
}

// dependency checks ///////////////////////////////////////////////////////////

inline bool
dep2(std::uint32_t a, std::uint32_t b)
{
     /* check a and b are independent
      * assumes bot a and b nonzero */
     return a == b;
}

inline bool
dep3(std::uint32_t a, std::uint32_t b, std::uint32_t c)
{
     /* assumes a and b are independent
      * and c nonzero */
     return (c == a) || (c == b) || (c == (a^b));
}

inline bool
dep4(std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d)
{
     /* assumes a, b, and c are independent
      * and d nonzero */
     return (d == a) || (d == b) || (d == c) ||
          (d == (a^b)) || (d == (a^c)) || (d == (b^c)) ||
          (d == (a^b^c));
}

inline bool
dep5(std::uint32_t a, std::uint32_t b,
     std::uint32_t c, std::uint32_t d, std::uint32_t e)
{
     /* assumes a, b, c, and d are independent
      * and e nonzero */
     return (e == a) || (e == b) || (e == c) || (e == d) ||
          (e == (a^b)) || (e == (a^c)) || (e == (a^d)) || (e == (b^c)) || (e == (b^d)) || (e == (c^d)) ||
          (e == (a^b^c)) || (e == (a^b^d)) || (e == (a^c^d)) || (e == (b^c^d)) || (e == (a^b^c^d));
}

inline bool
dep6(std::uint32_t a, std::uint32_t b, std::uint32_t c,
     std::uint32_t d, std::uint32_t e, std::uint32_t f)
{
     /* only checks equality
      * and f nonzero */
     return (f == a) || (f == b) || (f == c) || (f == d) || (f == e);
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
     std::uint32_t max_bit;
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

void thread_func(std::uint32_t _start, std::uint32_t end, const AG & canonical)
{
     std::uint32_t start = (_start == 0) ? 1 : _start;
     std::uint32_t a,b,c,d,e,f;
     for ( a = start; a < end; a++ )
          for ( b = 1; b < LEN; b++ ) {
               if ( dep2(a,b) ) continue;
               for ( c = 1; c < LEN; c++ ) {
                    if ( dep3(a,b,c) ) continue;
                    for ( d = 1; d < LEN; d++ ) {
                         if ( dep4(a,b,c,d) ) continue;
                         for ( e = 1; e < LEN; e++ ) {
                              if( dep5(a,b,c,d,e) ) continue;
                              for ( f = 1; f < LEN; f++ ) {
                                   if ( dep6(a,b,c,d,e,f) ) continue;
                                   std::vector<std::uint32_t> v{a,b,c,d,e,f};
                                   if ( reduce(v) ) continue;
                                   inc();
                                   // matrix A{std::move(v)};
                                   // AG new_ag{A * canonical};
                                   // std::lock_guard<std::mutex> lock(mtx);
                                   // if ( auto it = orth.find(new_ag); it == orth.end() ) {
                                   //      if ( orthogoval(new_ag, canonical) ) {
                                   //           std::cout << new_ag << '\n' << std::flush;
                                   //           orth.insert(std::move(new_ag));
                                   //      }
                                   // }
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

     AG canonical{ORDER + 1, ORDER};
     {
          std::fstream F("./var/ord8/spread.8.txt", std::ios::in);
          F >> canonical;
          F.close();
     }

     orth.insert(canonical);

     std::uint32_t first;
     std::vector<std::thread> threads(NTHREADS - 1);
     for ( std::uint32_t i{0}; i < NTHREADS - 1; i++ ) {
          first = i * block_len;
          threads[i] = std::thread(thread_func, first + start, first + start + block_len, canonical);
     }

     first = (NTHREADS - 1) * block_len;
     thread_func(first + start, end, canonical);

     for ( auto & t : threads )
          if ( t.joinable() ) t.join();

     // std::cout << orth.size() - 1 << '\n';

     std::cout << count << '\n';

     return 0;
}

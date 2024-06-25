# include <main.h>
# include <matrix.h>
# include <ag.h>

# include <functional>
# include <numeric>

# define NTHREADS 6
# define LEN 256
# define DIM 8
# define ORDER 16

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
     /* assumes a, b, c, d, and e are independent
      * and f nonzero */
     return (f == a) || (f == b) || (f == c) || (f == d) || (f == e) ||
          (f == (a^b)) || (f == (a^c)) || (f == (a^d)) || (f == (a^e)) || (f == (b^c)) ||
          (f == (b^d)) || (f == (b^e)) || (f == (c^d)) || (f == (c^e)) || (f == (d^e)) ||
          (f == (a^b^c)) || (f == (a^b^d)) || (f == (a^b^e)) || (f == (a^c^d)) || (f == (a^c^e)) ||
          (f == (a^d^e)) || (f == (b^c^d)) || (f == (b^c^e)) || (f == (b^d^e)) || (f == (c^d^e)) ||
          (f == (a^b^c^d)) || (f == (a^b^c^e)) || (f == (a^b^d^e)) || (f == (a^c^d^e)) || (f == (b^c^d^e)) ||
          (f == (a^b^c^d^e));
}

// threads /////////////////////////////////////////////////////////////////////

void thread_func(std::uint32_t _start, std::uint32_t end,
                 std::uint64_t & count, const AG & canonical)
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
                                   count++;
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

     std::uint32_t first;
     std::vector<std::uint64_t> counts(NTHREADS, 0);
     std::vector<std::thread> threads(NTHREADS - 1);
     for ( std::uint32_t i{0}; i < NTHREADS - 1; i++ ) {
          first = i * block_len;
          threads[i] = std::thread(thread_func, first + start, first + start + block_len, std::ref(counts[i]), canonical);
     }

     first = (NTHREADS - 1) * block_len;
     thread_func(first + start, end, std::ref(counts[NTHREADS - 1]), canonical);

     for ( auto & t : threads )
          if ( t.joinable() ) t.join();

     std::cout << std::accumulate(counts.begin(), counts.end(), 0) << '\n';

     return 0;
}

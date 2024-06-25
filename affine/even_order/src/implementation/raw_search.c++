# include <main.h>

# include <functional>
# include <numeric>

# define NTHREADS 6
# define LEN 64

// dependencies ////////////////////////////////////////////////////////////////

bool dep2(std::uint32_t a, std::uint32_t b)
{
     /* assumes a and b are nonzero */
     return a == b;
}

bool dep3(std::uint32_t a, std::uint32_t b, std::uint32_t c)
{
     /* assumes a and b are independent and c nonzero */
     return (c == a) || (c == b) || (c == (a^b));
}

bool dep4(std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d)
{
     /* assumes a, b, and c are independent and d nonzero */
     return (d == a) || (d == b) || (d == c) ||
          (d == (a^b)) || (d == (a^c)) || (d == (b^c)) ||
          (d == (a^b^c));
}

bool dep5(std::uint32_t a, std::uint32_t b,
          std::uint32_t c, std::uint32_t d, std::uint32_t e)
{
     /* assumes a, b, c, and d are independent and e nonzero */
     return (e == a) || (e == b) || (e == c) || (e == d) ||
          (e == (a^b)) || (e == (a^c)) || (e == (a^d)) ||
          (e == (b^c)) || (e == (b^d)) || (e == (c^d)) ||
          (e == (a^b^c)) || (e == (a^b^d)) || (e == (a^c^d)) || (e == (b^c^d)) ||
          (e == (a^b^c^d));
}

bool dep6(std::uint32_t a, std::uint32_t b, std::uint32_t c,
          std::uint32_t d, std::uint32_t e, std::uint32_t f)
{
     /* assumes a, b, c, d, and e are independent and f nonzero */
     return (f == a) || (f == b) || (f == c) || (f == d) || (f == e) ||
          (f == (a^b)) || (f == (a^c)) || (f == (a^d)) || (f == (a^e)) || (f == (b^c)) ||
          (f == (b^d)) || (f == (b^e)) || (f == (c^d)) || (f == (c^e)) || (f == (d^e)) ||
          (f == (a^b^c)) || (f == (a^b^d)) || (f == (a^b^e)) || (f == (a^c^d)) || (f == (a^c^e)) ||
          (f == (a^d^e)) || (f == (b^c^d)) || (f == (b^c^e)) || (f == (b^d^e)) || (f == (c^d^e)) ||
          (f == (a^b^c^d)) || (f == (a^b^c^e)) || (f == (a^b^d^e)) || (f == (a^c^d^e)) || (f == (b^c^d^e)) ||
          (f == (a^b^c^d^e));
}

bool dep7(std::uint32_t a, std::uint32_t b, std::uint32_t c,
          std::uint32_t d, std::uint32_t e, std::uint32_t f, std::uint32_t g)
{
     /* assumes a, b, c, d, e, and f are independent and g nonzero */
     return (g == a) || (g == b) || (g == c) || (g == d) || (g == e) || (g == f) ||
          (g == (a^b)) || (g == (a^c)) || (g == (a^d)) || (g == (a^e)) || (g == (a^f)) ||
          (g == (b^c)) || (g == (b^d)) || (g == (b^e)) || (g == (b^f)) || (g == (c^d)) ||
          (g == (c^e)) || (g == (c^f)) || (g == (d^e)) || (g == (d^f)) || (g == (e^f)) ||
          (g == (a^b^c)) || (g == (a^b^d)) || (g == (a^b^e)) || (g == (a^b^f)) || (g == (a^c^d)) ||
          (g == (a^c^e)) || (g == (a^c^f)) || (g == (a^d^e)) || (g == (a^d^f)) || (g == (a^e^f)) ||
          (g == (b^c^d)) || (g == (b^c^e)) || (g == (b^c^f)) || (g == (b^d^e)) || (g == (b^d^f)) ||
          (g == (b^e^f)) || (g == (c^d^e)) || (g == (c^d^f)) || (g == (c^e^f)) || (g == (d^e^f)) ||
          (g == (a^b^c^d)) || (g == (a^b^c^e)) || (g == (a^b^c^f)) || (g == (a^b^d^e)) || (g == (a^b^d^f)) ||
          (g == (a^b^e^f)) || (g == (a^c^d^e)) || (g == (a^c^d^f)) || (g == (a^c^e^f)) || (g == (a^d^e^f)) ||
          (g == (b^c^d^e)) || (g == (b^c^d^f)) || (g == (b^c^e^f)) || (g == (b^d^e^f)) || (g == (c^d^e^f)) ||
          (g == (a^b^c^d^e)) || (g == (a^b^c^d^f)) || (g == (a^b^c^e^f)) || (g == (a^b^d^e^f)) || (g == (a^c^d^e^f)) || (g == (b^c^d^e^f)) ||
          (g == (a^b^c^d^e^f));
}

bool dep8(std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
          std::uint32_t e, std::uint32_t f, std::uint32_t g, std::uint32_t h)
{
     /* assumes a, b, c, d, e, f, and g are independent and h nonzero */
     return (h == a) || (h == b) || (h == c) || (h == d) || (h == e) || (h == f) || (h == g) ||
          (h == (a^b)) || (h == (a^c)) || (h == (a^d)) || (h == (a^e)) || (h == (a^f)) || (h == (a^g)) || (h == (b^c)) ||
          (h == (b^d)) || (h == (b^e)) || (h == (b^f)) || (h == (b^g)) || (h == (c^d)) || (h == (c^e)) || (h == (c^f)) ||
          (h == (c^g)) || (h == (d^e)) || (h == (d^f)) || (h == (d^g)) || (h == (e^f)) || (h == (e^g)) || (h == (f^g)) ||
          (h == (a^b^c)) || (h == (a^b^d)) || (h == (a^b^e)) || (h == (a^b^f)) || (h == (a^b^g)) || (h == (a^c^d)) || (h == (a^c^e)) ||
          (h == (a^c^f)) || (h == (a^c^g)) || (h == (a^d^e)) || (h == (a^d^f)) || (h == (a^d^g)) || (h == (a^e^f)) || (h == (a^e^g)) ||
          (h == (a^f^g)) || (h == (b^c^d)) || (h == (b^c^e)) || (h == (b^c^f)) || (h == (b^c^g)) || (h == (b^d^e)) || (h == (b^d^f)) ||
          (h == (b^d^g)) || (h == (b^e^f)) || (h == (b^e^g)) || (h == (b^f^g)) || (h == (c^d^e)) || (h == (c^d^f)) || (h == (c^d^g)) ||
          (h == (c^e^f)) || (h == (c^e^g)) || (h == (c^f^g)) || (h == (d^e^f)) || (h == (d^e^g)) || (h == (d^f^g)) || (h == (e^f^g)) ||
          (h == (a^b^c^d)) || (h == (a^b^c^e)) || (h == (a^b^c^f)) || (h == (a^b^c^g)) || (h == (a^b^d^e)) || (h == (a^b^d^f)) || (h == (a^b^d^g)) ||
          (h == (a^b^e^f)) || (h == (a^b^e^g)) || (h == (a^b^f^g)) || (h == (a^c^d^e)) || (h == (a^c^d^f)) || (h == (a^c^d^g)) || (h == (a^c^e^f)) ||
          (h == (a^c^e^g)) || (h == (a^c^f^g)) || (h == (a^d^e^f)) || (h == (a^d^e^g)) || (h == (a^d^f^g)) || (h == (a^e^f^g)) || (h == (b^c^d^e)) ||
          (h == (b^c^d^f)) || (h == (b^c^d^g)) || (h == (b^c^e^f)) || (h == (b^c^e^g)) || (h == (b^c^f^g)) || (h == (b^d^e^f)) || (h == (b^d^e^g)) ||
          (h == (b^d^f^g)) || (h == (b^e^f^g)) || (h == (c^d^e^f)) || (h == (c^d^e^g)) || (h == (c^d^f^g)) || (h == (c^e^f^g)) || (h == (d^e^f^g)) ||
          (h == (a^b^c^d^e)) || (h == (a^b^c^d^f)) || (h == (a^b^c^d^g)) || (h == (a^b^c^e^f)) || (h == (a^b^c^e^g)) || (h == (a^b^c^f^g)) || (h == (a^b^d^e^f)) ||
          (h == (a^b^d^e^g)) || (h == (a^b^d^f^g)) || (h == (a^b^e^f^g)) || (h == (a^c^d^e^f)) || (h == (a^c^d^e^g)) || (h == (a^c^d^f^g)) || (h == (a^c^e^f^g)) ||
          (h == (a^d^e^f^g)) || (h == (b^c^d^e^f)) || (h == (b^c^d^e^g)) || (h == (b^c^d^f^g)) || (h == (b^c^e^f^g)) || (h == (b^d^e^f^g)) || (h == (c^d^e^f^g)) ||
          (h == (a^b^c^d^e^f)) || (h == (a^b^c^d^e^g)) || (h == (a^b^c^d^f^g)) ||
          (h == (a^b^c^e^f^g)) || (h == (a^b^d^e^f^g)) || (h == (a^c^d^e^f^g)) || (h == (b^c^d^e^f^g)) ||
          (h == (a^b^c^d^e^f^g));
}

// thread func /////////////////////////////////////////////////////////////////

void thread_func(std::uint32_t _start, std::uint32_t end, std::uint64_t & count)
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
                              if ( dep5(a,b,c,d,e) ) continue;
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

int main(void)
{
     std::uint32_t first, block_len = LEN / NTHREADS;
     std::vector<std::uint64_t> counts(NTHREADS, 0);
     std::vector<std::thread> threads(NTHREADS - 1);

     for ( std::uint32_t i{0}; i < NTHREADS - 1; i++ ) {
          first = block_len * i;
          threads[i] = std::thread(thread_func, first, first + block_len, std::ref(counts[i]));
     }

     first = block_len * (NTHREADS - 1);
     thread_func(first, LEN, std::ref(counts[NTHREADS - 1]));

     for (auto & t : threads) if ( t.joinable() ) t.join();

     std::cout << std::accumulate(counts.begin(), counts.end(), (std::uint64_t)0) << '\n';

     return 0;
}

# include <ag.h>

AG operator*(const matrix & A, const AG & ag1)
{
     AG ag2{ag1.spread.size(), (ag1.spread[0]).size()};
     for ( std::size_t i{0}; i<ag1.spread.size(); i++ ) {
          std::transform((ag1.spread[i]).begin(), (ag1.spread[i]).end(),
                         (ag2.spread[i]).begin(),
                         [&](const auto & x) -> std::vector<std::uint32_t> {
                              return A * x;
                         });
          std::sort((ag2.spread[i]).begin(), (ag2.spread[i]).end());
     }
     std::sort(ag2.spread.begin(), ag2.spread.end());
     return ag2;
}

std::ostream& operator<<(std::ostream & os, const AG & ag)
{
     std::for_each(ag.spread.begin(), ag.spread.end(), [&](const auto & x) {
          for ( const auto & y : x ) {
               for ( auto z : y ) os << z << " ";
               os << '\n';
          }
     });
     return os;
}

std::istream& operator>>(std::istream & is, AG & ag)
{
     std::for_each(ag.spread.begin(), ag.spread.end(), [&](auto & x){
          for ( auto & y : x )
               for ( auto & z : y )
                    is >> z;
          std::sort(x.begin(), x.end());
     });
     std::sort(ag.spread.begin(), ag.spread.end());
     return is;
}

bool operator==(const AG & ag1, const AG & ag2)
{
     return ag1.spread == ag2.spread;
}

bool orthogoval(const AG & ag1, const AG & ag2)
{
     std::size_t k1, k2, intersect, lb = (ag1.spread[0]).size();
     for ( const auto & x1 : ag1.spread )
          for ( const auto & x2 : ag2.spread ) {
               intersect = 0;
               for ( k1 = 0, k2 = 0; k1 < lb && k2 < lb; ) {
                    auto comp = x1[k1] <=> x2[k2];
                    if ( comp == 0 ) {
                         intersect++, k1++, k2++;
                    }
                    else if ( comp < 0 ) k1++;
                    else k2++;
                    if ( intersect > 2 ) return false;
               }
          }
     return true;
}

void AG::resize(std::size_t nb, std::size_t lb, std::size_t degree)
{
     spread.resize(nb);
     std::for_each(spread.begin(), spread.end(), [&](auto & x){
          x.resize(lb);
          std::for_each(x.begin(), x.end(), [&](auto & y){
               y.resize(degree);
          });
     });
}

AG& AG::operator=(const AG & ag)
{
     AG cpy{ag};
     std::swap(*this, cpy);
     return *this;
}

AG& AG::operator=(AG && ag)
{
     std::swap(spread, ag.spread);
     return *this;
}

AG::AG(std::size_t nb, std::size_t lb)
     : spread( nb )
{
     std::for_each(spread.begin(), spread.end(), [&](auto & x){
          x.resize(lb);
     });
}

AG::AG(const AG & ag)
     : spread{ ag.spread }
{}

AG::AG(AG && ag)
     : spread{ std::move(ag.spread) }
{}

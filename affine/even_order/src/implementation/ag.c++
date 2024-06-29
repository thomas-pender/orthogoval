# include <ag.h>

AG operator*(const matrix &A, const AG &ag1)
{
     std::size_t nb=ag1.spread.size(), lb=(ag1.spread[0]).size();
     std::size_t i,j;
     AG ag2{nb,lb};
     for (i=0; i<nb; i++) {
          for (j=0; j<lb; j++)
               ag2.spread[i][j]=A*ag1.spread[i][j];
          std::sort((ag2.spread[i]).begin(),(ag2.spread[i]).end());
     }
     std::sort(ag2.spread.begin(),ag2.spread.end());
     return ag2;
}

std::ostream& operator<<(std::ostream &os, const AG &ag)
{
     std::for_each(ag.spread.begin(),ag.spread.end(),[&](const auto &x){
               std::copy(x.begin(),x.end(),
                         std::ostream_iterator<std::uint32_t>(os," "));
               os << '\n';
          });
     return os;
}

std::istream& operator>>(std::istream &is, AG &ag)
{
     std::for_each(ag.spread.begin(),ag.spread.end(),[&](auto &x){
          for (std::size_t i{0}; i<x.size(); i++) is >> x[i];
          std::sort(x.begin(),x.end());
     });
     std::sort(ag.spread.begin(),ag.spread.end());
     return is;
}

bool operator==(const AG &ag1, const AG &ag2)
{
     return ag1.spread==ag2.spread;
}

bool operator<(const AG &ag1, const AG &ag2)
{
     return ag1.spread < ag2.spread;
}

bool orthogoval(const AG &ag1, const AG &ag2)
{
     std::size_t intersect, k1, k2,
          len=std::min((ag1.spread[0]).size(),(ag2.spread[0]).size());
     for (const auto &x1 : ag1.spread)
          for (const auto &x2 : ag2.spread) {
               intersect=0;
               for (k1=0, k2=0; k1<len && k2<len;) {
                    if (x1[k1]==x2[k2]) {
                         intersect++, k1++, k2++;
                    }
                    else if (x1[k1]<x2[k2]) k1++;
                    else k2++;
                    if (intersect>2) return false;
               }
          }
     return true;
}

void AG::resize(std::size_t nb, std::size_t lb)
{
     spread.resize(nb);
     std::for_each(spread.begin(),spread.end(),[&](auto &x){
          x.resize(lb);
     });
}

AG& AG::operator=(const AG &ag)
{
     AG cpy{ag};
     std::swap(*this,cpy);
     return *this;
}

AG& AG::operator=(AG &&ag)
{
     spread=std::move(ag.spread);
     return *this;
}

AG::AG(std::size_t nb, std::size_t lb)
     : spread(nb)
{
     std::for_each(spread.begin(),spread.end(),[this,lb](auto &x){
          x.resize(lb);
     });
}

AG::AG(const AG &ag)
     : spread{ag.spread}
{}

AG::AG(AG &&ag)
     : spread{std::move(ag.spread)}
{}

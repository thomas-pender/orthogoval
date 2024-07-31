# include <config.h>

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

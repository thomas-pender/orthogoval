# include <matrix.h>

std::vector<std::uint32_t> operator*(const matrix & A,
                                     const std::vector<std::uint32_t> & x)
{
     std::size_t i, j;
     std::uint32_t sum;
     std::vector<std::uint32_t> y(A.M.size());

     for ( i = 0; i < A.M.size(); i++ ) {
          sum = 0;
          for ( j = 0; j < A.M.size(); j++ )
               sum += (A.M[i][j] * x[j]) % A.modulus;
          y[i] = sum % A.modulus;
     }

     return y;
}

std::ostream& operator<<(std::ostream & os, const matrix & A)
{
     std::for_each(A.M.begin(), A.M.end(), [&](const auto & x){
          for ( auto y : x ) os << y << " ";
          os << '\n';
     });
     return os;
}

std::istream& operator>>(std::istream & is, matrix & A)
{
     std::for_each(A.M.begin(), A.M.end(), [&](auto & x){
          for ( auto & y : x ) is >> y;
     });
     return is;
}

void matrix::resize(std::size_t n)
{
     M.resize(n);
     std::for_each(M.begin(), M.end(), [&](auto & x ){
          x.resize(n);
     });
}

void matrix::set_modulus(std::uint32_t mod)
{
     modulus=mod;
}

matrix& matrix::operator=(const matrix & A)
{
     matrix cpy{A};
     std::swap(*this, cpy);
     return *this;
}

matrix& matrix::operator=(matrix && A)
{
     std::swap(M, A.M);
     std::swap(modulus, A.modulus);
     return *this;
}

matrix::matrix(uint32_t mod, std::size_t n)
     : modulus{ mod }, M( n )
{}

matrix::matrix(const matrix & A)
     : modulus{ A.modulus }, M{ A.M }
{}

matrix::matrix(matrix && A)
     : modulus{ A.modulus }, M{ std::move(A.M) }
{}

# include <matrix.h>

std::uint32_t operator*(const matrix &A, std::uint32_t v)
{
     std::uint32_t retval=0;
     for (std::size_t i{0}; i<A.M.size(); i++)
          if ((std::uint32_t)__builtin_popcountl(A.M[i]&v) & 1UL)
               retval |= (1UL << (A.M.size()-i-1));
     return retval;
}

std::ostream& operator<<(std::ostream &os, const matrix &A)
{
     std::copy(A.M.begin(),A.M.end(),
               std::ostream_iterator<std::uint32_t>(os," "));
     os << '\n';
     return os;
}

std::istream& operator>>(std::istream &is, matrix &A)
{
     std::for_each(A.M.begin(),A.M.end(),[&](auto &x){is >> x;});
     return is;
}

void matrix::resize(std::size_t n)
{
     M.resize(n);
}

void matrix::swap(std::vector<std::uint32_t> && v)
{
     M = std::exchange(v, {});
}

matrix& matrix::operator=(const matrix &A)
{
     matrix cpy{A};
     std::swap(*this,cpy);
     return *this;
}

matrix& matrix::operator=(matrix &&A)
{
     std::swap(M,A.M);
     return *this;
}

matrix::matrix(std::size_t n)
     : M(n,0)
{}

matrix::matrix(const std::vector<std::uint32_t> &v)
     : M{v}
{}

matrix::matrix(std::vector<std::uint32_t> &&v)
     : M{std::move(v)}
{}

matrix::matrix(const matrix &A)
     : M{A.M}
{}

matrix::matrix(matrix &&A)
     : M{std::exchange(A.M,{})}
{}

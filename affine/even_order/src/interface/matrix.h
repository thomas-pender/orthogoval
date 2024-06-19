# ifndef MATRIX_H
# define MATRIX_H

# include <main.h>

class matrix
{
     friend std::uint32_t operator*(const matrix&, std::uint32_t);
     friend std::ostream& operator<<(std::ostream&, const matrix&);
     friend std::istream& operator>>(std::istream&, matrix&);

public:
     void resize(std::size_t);

     matrix& operator=(const matrix&);
     matrix& operator=(matrix&&);

     matrix()=default;
     explicit matrix(std::size_t);
     matrix(const matrix&);
     matrix(matrix&&);
     ~matrix()=default;

private:
     std::vector<std::uint32_t> M;
};

# endif

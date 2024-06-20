# ifndef MATRIX_H
# define MATRIX_H

# include <main.h>

class matrix
{
     friend std::vector<std::uint32_t> operator*(const matrix&,
                                                const std::vector<std::uint32_t>&);
     friend std::ostream& operator<<(std::ostream&, const matrix&);
     friend std::istream& operator>>(std::istream&, matrix&);

public:
     void resize(std::size_t);
     void set_modulus(std::uint32_t);

     matrix& operator=(const matrix&);
     matrix& operator=(matrix&&);

     matrix()=default;
     matrix(std::uint32_t, std::size_t);
     matrix(const matrix&);
     matrix(matrix&&);
     ~matrix()=default;

private:
     std::uint32_t modulus;
     std::vector<std::vector<std::uint32_t>> M;
};

# endif

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <iostream>


class Matrix {
public:
    Matrix(size_t rows, size_t cols, double init = 0.0);

    double& operator()(size_t row, size_t col);
    double  operator()(size_t row, size_t col) const;

    size_t numRows() const;
    size_t numCols() const;

    Matrix transpose() const;


private:
    std::vector<double> data_;
    size_t numRows_;
    size_t numCols_;


};

Matrix operator+(const Matrix& a, const Matrix& b);
Matrix operator-(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, double scalar);
Matrix operator*(double scalar,const Matrix& a);

Matrix computeCovariance(const Matrix& returns);
Matrix make_identity(size_t N);






std::ostream& operator<<(std::ostream& os, const Matrix& m);


#endif
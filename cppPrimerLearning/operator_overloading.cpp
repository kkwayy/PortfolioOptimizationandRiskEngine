//
// Created by ethan on 19-Aug-26.
//

#include <iostream>
#include <vector>
#include <cstddef>
#include <cassert>

class Matrix {
public:
    Matrix(size_t rows, size_t cols, double init = 0.0);

    double &operator()(size_t row, size_t col); // writable
    double operator()(size_t row, size_t col) const; // read-only

    size_t numRows() const;

    size_t numCols() const;

private:
    std::vector<double> data_;
    size_t numRows_;
    size_t numCols_;
};

Matrix::Matrix(size_t rows, size_t cols, double init)
    : numRows_(rows), numCols_(cols), data_(rows * cols, init) {
}

size_t Matrix::numRows() const { return numRows_; }
size_t Matrix::numCols() const { return numCols_; }

// ---- YOU WRITE THESE TWO ----
double &Matrix::operator()(size_t row, size_t col) {
    assert(row < numRows_ && col < numCols_);
    return data_[row * numCols_ + col];
}

double Matrix::operator()(size_t row, size_t col) const {
    assert(row < numRows_ && col < numCols_);
    return data_[row * numCols_ + col];
}

// -----------------------------

int main() {
    Matrix m(2, 3, 0.0);
    m(1, 2) = 9.5; // WRITE through operator()
    std::cout << m(1, 2) << '\n'; // READ through operator() → 9.5
    std::cout << m.numRows() << " x " << m.numCols() << '\n'; // 2 x 3
    return 0;
}

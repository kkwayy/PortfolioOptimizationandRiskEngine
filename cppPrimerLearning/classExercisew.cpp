//
// Created by ethan on 14-Aug-26.
//


#include <vector>
#include <cstddef>   // for size_t
#include <iostream>
#include <cassert>






class Matrix {

public:

    Matrix(size_t rows, size_t col, double init=0.0);


    double& getPosition(size_t row, size_t col);
    double getPosition(size_t row, size_t col) const;


    size_t numRows() const;
    size_t numCols() const;


private:
    std::vector<double> data_;
    size_t numRows_;
    size_t numCols_;

};

Matrix::Matrix(size_t rows, size_t col, double init):
numRows_(rows),numCols_(col),data_(rows*col, init)
{
}

size_t Matrix::numRows() const {
    return numRows_;
}


size_t Matrix::numCols() const {
    return  numCols_;
}


double& Matrix::getPosition(size_t row, size_t col) {
    assert(row < numRows_ && col < numCols_);
    size_t index = row * numCols_ + col;
    return data_[index];
}

double  Matrix::getPosition(size_t row, size_t col) const {
    size_t index=(row*numCols_ + col);

    return  data_[index];
}



int main() {
    Matrix m(2, 3, 0.0);
    m.getPosition(1, 2) = 9.5;              // valid → works
    std::cout << m.getPosition(1, 2) << '\n';
    m.getPosition(5, 5) = 1.0;              // INVALID → assert fires, program aborts
    return 0;
}





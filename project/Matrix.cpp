
#include "Matrix.h"    // note: QUOTES, not <> — it's your header, not a system one
#include <cassert>     // needed here because the bodies use assert

// --- you write the five bodies below, moved from your old file ---
// Matrix::Matrix(...)      (the constructor with the initialiser list)
// Matrix::numRows() ...
// Matrix::numCols() ...
// Matrix::operator()(...)  (writable)
// Matrix::operator()(...) const  (read-only)


Matrix::Matrix(size_t rows, size_t cols, double init) //Constructor - with the intialiser list
    : numRows_(rows), numCols_(cols), data_(rows * cols, init) {
}

size_t Matrix::numRows() const { return numRows_; }
size_t Matrix::numCols() const { return numCols_; }


double &Matrix::operator()(size_t row, size_t col) { // write
    assert(row < numRows_ && col < numCols_);
    return data_[row * numCols_ + col];
}

double Matrix::operator()(size_t row, size_t col) const {
    assert(row < numRows_ && col < numCols_);
    return data_[row * numCols_ + col];
}

Matrix Matrix::transpose() const {
    Matrix result(numCols(),numRows(),0.0);
    for (size_t r =0 ; r < numRows(); r++)
        for (size_t  c = 0 ; c < numCols(); c++)
            result(c,r)=(*this)(r,c);

    return result;
}


Matrix operator+(const Matrix& a, const Matrix& b) {
    // 1. assert same dimensions
    // 2. Matrix result(?, ?, 0.0);   ← what dimensions?
    // 3. nested loop over rows and cols:
    //       result(r, c) = a(r, c) + b(r, c);
    // 4. return result;

    assert(a.numCols() ==b.numCols() && a.numRows() == b.numRows());

    Matrix result(a.numRows(), a.numCols(),0.0);

    for (size_t r =0 ; r < a.numRows(); r++)
        for (size_t  c = 0 ; c < a.numCols(); c++)
            result(r,c)= a(r,c) + b(r,c);

    return result;

}

Matrix operator*(const Matrix& a, const Matrix& b) {
    // 1. assert inner dimensions match:  a.numCols() == b.numRows()
    // 2. result is (a.numRows() × b.numCols())
    // 3. triple loop: for i, for j, { sum=0; for k: sum += a(i,k)*b(k,j); result(i,j)=sum; }
    // 4. return result

    assert(a.numCols() == b.numRows() );

    Matrix result(a.numRows(),b.numCols(),0.0);

    for (size_t r =0 ; r < a.numRows(); r++) {
        for (size_t  c = 0 ; c < b.numCols(); c++) {
            double dot_product =0.0;

                for (size_t k = 0 ; k < a.numCols() ; k++)
                    dot_product += a(r,k) * b( k, c);


            result(r,c)= dot_product;
        }
    }
    return result;


}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    // for each row r:
    //     for each col c:
    //         os << m(r, c) << ' ';      // element then a space
    //     os << '\n';                    // newline after the row
    // return os;

    for (size_t r =0; r < m.numRows() ; r++) {
        for (size_t c = 0 ; c <m.numCols(); c++) {
            os << m(r,c) <<' ';

        }

        os << '\n';

    }

    return os;
}


Matrix operator-(const Matrix& a, const Matrix& b) {
    // 1. assert same dimensions
    // 2. Matrix result(?, ?, 0.0);   ← what dimensions?
    // 3. nested loop over rows and cols:
    //       result(r, c) = a(r, c) - b(r, c);
    // 4. return result;

    assert(a.numCols() ==b.numCols() && a.numRows() == b.numRows());

    Matrix result(a.numRows(), a.numCols(),0.0);

    for (size_t r =0 ; r < a.numRows(); r++)
        for (size_t  c = 0 ; c < a.numCols(); c++)
            result(r,c)= a(r,c) - b(r,c);

    return result;

}


Matrix operator*(const Matrix& a,  double scalar) {
    Matrix result(a.numRows(), a.numCols(),0.0);

    for (size_t r =0 ; r < a.numRows(); r++)
        for (size_t  c = 0 ; c < a.numCols(); c++)
            result(r,c)= a(r,c) * scalar;

    return result;

}

Matrix operator*(double scalar,const Matrix& a) { return a * scalar;}

Matrix computeCovariance(const Matrix& returns) {
    // T = returns.numRows()  (time observations)
    // N = returns.numCols()  (assets)
    //
    // 1. CENTER: make a copy of returns, then for each column j:
    //       - compute mean of column j
    //       - subtract it from every entry in column j
    // 2. Xc^T * Xc   (transpose + operator*)  → N×N
    // 3. scale by 1.0 / (T - 1)
    // return the N×N covariance matrix

    Matrix X = returns;

    size_t T= returns.numRows();
    size_t N = returns.numCols();

    std::vector<double> means;


    for (size_t j =0 ; j < N; j++) {
        double total= 0.0;

        for (size_t x = 0 ; x< T ; x++) {
            double value = X(x,j);
            total+= value;
        }

        double mean= total/T;
        means.push_back(mean);
    }

    for (size_t y= 0 ; y < N ; y++) {
        double current_mean=means[y];

        for (size_t x = 0 ; x< T ; x++) {
            X(x,y)= X(x,y) - current_mean;
        }
    }

    Matrix X_t = X.transpose();
    Matrix result = X_t * X;
    result= (1.0/(T-1)) * result;

    return result;
}

Matrix make_identity(size_t N) {
    Matrix result(N,N, 0.0);

    for (size_t n = 0; n< N; n++) {
        result(n,n)=1.0;
    }

    return result;
}


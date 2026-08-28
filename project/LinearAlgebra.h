
#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include <vector>
#include <cstddef>
#include <iostream>



void luDecompose(const Matrix& A, Matrix& L, Matrix& U);
std::vector<double> forwardSub(const Matrix& L, const std::vector<double>& b);
std::vector<double> backwardSub(const Matrix& U, const std::vector<double>& y);
std::vector<double> solve(const Matrix& A, const std::vector<double>& b);
std::vector<double> gmvWeights(const Matrix& returns);
std::vector<double> targetReturnWeights(const Matrix& returns, double targetReturn);

#endif

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
double portfolioVariance(const std::vector<double>& w, const Matrix& Sigma);
void testPortfolioVariance(const Matrix& returns);
double portfolioVarianceMatmul(const std::vector<double>& w, const Matrix& Sigma);
std::vector<std::pair<double,double>> efficientFrontier(
    const Matrix& returns, double Rmin, double Rmax, size_t steps);

#endif
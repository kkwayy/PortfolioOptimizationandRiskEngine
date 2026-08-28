#include "Matrix.h"
#include "DataLoader.h"
#include "LinearAlgebra.h"
#include <iostream>

int main() {
    auto data = loadCSV("prices.csv");
    Matrix prices = buildMatrix(data);
    Matrix returns = computeReturns(prices);

    size_t N = returns.numCols(), T = returns.numRows();

    // --- means (needed for the return checks) ---
    std::vector<double> mu(N, 0.0);
    for (size_t j = 0; j < N; j++) {
        double colSum = 0.0;
        for (size_t t = 0; t < T; t++) colSum += returns(t, j);
        mu[j] = colSum / T;
    }

    // --- GMV ---
    std::vector<double> wgmv = gmvWeights(returns);
    std::cout << "GMV weights: ";
    double sGmv = 0.0;
    for (double wi : wgmv) { std::cout << wi << ' '; sGmv += wi; }
    std::cout << " | sum = " << sGmv << '\n';

    // --- Test 1: target-return on real data ---
    double target = 0.0005;
    std::vector<double> wt = targetReturnWeights(returns, target);
    double sumT = 0.0, retT = 0.0;
    for (size_t i = 0; i < N; i++) { sumT += wt[i]; retT += mu[i] * wt[i]; }
    std::cout << "\nTarget-return: sum = " << sumT
              << " | achieved = " << retT << " (target " << target << ")\n";

    // --- Test 2: cross-check (GMV return as target should reproduce GMV) ---
    double gmvReturn = 0.0;
    for (size_t i = 0; i < N; i++) gmvReturn += mu[i] * wgmv[i];
    std::vector<double> wcross = targetReturnWeights(returns, gmvReturn);
    std::cout << "Cross-check (target = GMV return " << gmvReturn << "):\n";
    std::cout << "  GMV weights:  ";
    for (double x : wgmv) std::cout << x << ' ';
    std::cout << "\n  target-ret w: ";
    for (double x : wcross) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
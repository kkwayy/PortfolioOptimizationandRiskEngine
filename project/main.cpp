#include "Matrix.h"
#include "DataLoader.h"
#include "LinearAlgebra.h"
#include <iostream>
#include <cmath>
#include <fstream>

int main() {
    auto data = loadCSV("prices.csv");
    Matrix prices = buildMatrix(data);
    Matrix returns = computeReturns(prices);

    size_t N = returns.numCols(), T = returns.numRows();

    // Sigma is the covariance matrix (N x N, square). Compute it ONCE up front
    // and reuse it — every risk calculation below needs Sigma, never `returns`.
    Matrix Sigma = computeCovariance(returns);   // # THE FIX: variance needs Sigma, not returns

    // --- means (needed for the return checks) ---
    std::vector<double> mu(N, 0.0);
    for (size_t j = 0; j < N; j++) {
        double colSum = 0.0;
        for (size_t t = 0; t < T; t++) colSum += returns(t, j);
        mu[j] = colSum / T;
    }
    std::cout << "Per-asset mean returns (mu): ";
    for (size_t j = 0; j < N; j++) std::cout << mu[j] << ' ';
    std::cout << '\n';

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

    // --- portfolio variance / volatility of the target-return portfolio ---
    double variance = portfolioVariance(wt, Sigma);   // # THE FIX: pass Sigma, not returns
    std::cout << "\ntarget-ret variance = " << variance
              << " | sigma = " << std::sqrt(variance) << '\n';

    // --- run the verification harness (Check 1: routes agree, Check 2: GMV sigma) ---
    testPortfolioVariance(returns);

    double Rmin = 0.0;       // a bit below GMV's 0.000356 — shows the lower (dominated) branch
    double Rmax = 0.0017;    // just under AAPL's 0.00183 — the efficient branch runs up to here
    size_t steps = 50;


    auto frontier = efficientFrontier(returns, 0.0, 0.0017, 50);
    std::cout << "\nsigma,return\n";
    for (auto& p : frontier)
        std::cout << p.first << ',' << p.second << '\n';



    // ... after you compute `frontier`:
    std::ofstream out("frontier.csv");
    out << "sigma,return\n";
    for (auto& p : frontier)
        out << p.first << ',' << p.second << '\n';
    out.close();
    std::cout << "wrote frontier.csv (" << frontier.size() << " points)\n";



    return 0;


}
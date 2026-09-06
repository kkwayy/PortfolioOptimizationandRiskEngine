#include "Matrix.h"
#include "DataLoader.h"
#include "LinearAlgebra.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

// Portfolio Optimization & Risk Engine — demo entry point.
//
// Loads a price CSV, computes returns and the covariance matrix, then:
//   1. solves the global-minimum-variance (GMV) portfolio,
//   2. solves a target-return portfolio,
//   3. sweeps the efficient frontier and writes it to frontier.csv.
//
// Verification/oracle tests live in tests.cpp (the separate `tests` target).

int main() {
    // --- load data ---
    auto data      = loadCSV("prices.csv");
    Matrix prices  = buildMatrix(data);
    Matrix returns = computeReturns(prices);      // (T-1) x N : daily simple returns
    Matrix Sigma   = computeCovariance(returns);  // N x N covariance matrix

    const size_t N = returns.numCols();
    const size_t T = returns.numRows();

    // Column order MUST match prices.csv. Edit if you change the asset set.
    std::vector<std::string> assets = {"AAPL", "JPM", "XOM", "PG", "JNJ"};

    // --- per-asset mean (daily) returns, for reporting achieved returns ---
    std::vector<double> mu(N, 0.0);
    for (size_t j = 0; j < N; ++j) {
        double s = 0.0;
        for (size_t t = 0; t < T; ++t) s += returns(t, j);
        mu[j] = s / T;
    }

    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);

    // --- 1. Global Minimum Variance portfolio ---
    std::vector<double> wGmv = gmvWeights(returns);
    double gmvRet = 0.0, gmvSum = 0.0;
    for (size_t i = 0; i < N; ++i) { gmvRet += mu[i] * wGmv[i]; gmvSum += wGmv[i]; }
    double gmvSigma = std::sqrt(portfolioVariance(wGmv, Sigma));

    std::cout << "=== Global Minimum Variance portfolio ===\n";
    for (size_t i = 0; i < N; ++i)
        std::cout << "  " << assets[i] << ": " << wGmv[i] << "\n";
    std::cout << "  weights sum : " << gmvSum   << "\n";
    std::cout << "  exp. return : " << gmvRet   << "\n";
    std::cout << "  risk (sigma): " << gmvSigma << "\n\n";

    // --- 2. Target-return portfolio ---
    const double target = 0.0008;   // daily; must lie within the assets' return range
    std::vector<double> wTgt = targetReturnWeights(returns, target);
    double tgtRet = 0.0, tgtSum = 0.0;
    for (size_t i = 0; i < N; ++i) { tgtRet += mu[i] * wTgt[i]; tgtSum += wTgt[i]; }
    double tgtSigma = std::sqrt(portfolioVariance(wTgt, Sigma));

    std::cout << "=== Target-return portfolio (target = " << target << ") ===\n";
    for (size_t i = 0; i < N; ++i)
        std::cout << "  " << assets[i] << ": " << wTgt[i] << "\n";
    std::cout << "  weights sum : " << tgtSum   << "\n";
    std::cout << "  exp. return : " << tgtRet   << " (target " << target << ")\n";
    std::cout << "  risk (sigma): " << tgtSigma << "\n\n";

    // --- 3. Efficient frontier -> CSV ---
    const double Rmin = 0.0, Rmax = 0.0017;   // straddles the GMV return
    const size_t steps = 50;
    auto frontier = efficientFrontier(returns, Rmin, Rmax, steps);

    std::ofstream out("frontier.csv");
    out << "sigma,return\n";
    out.setf(std::ios::fixed);
    out.precision(10);
    for (const auto& p : frontier)
        out << p.first << "," << p.second << "\n";
    out.close();

    std::cout << "Wrote frontier.csv (" << frontier.size()
              << " points). Plot with:  python plot_frontier.py\n";

    return 0;
}
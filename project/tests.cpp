//
// Created by ethan on 06-Sep-26.
//

#include "Matrix.h"
#include "DataLoader.h"
#include "LinearAlgebra.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// Verification harness for the Portfolio Optimization & Risk Engine.
// Each check is compared against a hand-computed oracle or a theoretical
// invariant. Build as the `tests` target and run; exit code is nonzero
// if any check fails (so it can gate a commit / CI later).

static int failures = 0;

static void check(const std::string& name, bool ok) {
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (!ok) ++failures;
}

// Never compare doubles with ==; two routes to the same value differ in the
// last bit or two. Test "close enough" within a tolerance instead.
static bool close(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) < tol;
}

int main() {
    std::cout.setf(std::ios::fixed);
    std::cout.precision(10);

    // --- Oracle 1: LU solve against a hand-computed system -----------------
    // A x = b with known solution x = [1, -2, 1].
    {
        Matrix A(3, 3, 0.0);
        A(0,0)=2; A(0,1)=1; A(0,2)=1;
        A(1,0)=4; A(1,1)=3; A(1,2)=3;
        A(2,0)=8; A(2,1)=7; A(2,2)=9;
        std::vector<double> b = {1, 1, 3};
        std::vector<double> x = solve(A, b);
        check("LU solve reproduces hand-computed x = [1, -2, 1]",
              close(x[0], 1.0) && close(x[1], -2.0) && close(x[2], 1.0));
    }

    // --- Load real data for the remaining checks --------------------------
    auto data      = loadCSV("prices.csv");
    Matrix returns = computeReturns(buildMatrix(data));
    Matrix Sigma   = computeCovariance(returns);
    const size_t N = returns.numCols();
    const size_t T = returns.numRows();

    // --- Oracle 2: covariance is symmetric with a positive diagonal -------
    {
        bool sym = true, posdiag = true;
        for (size_t i = 0; i < N; ++i) {
            if (Sigma(i,i) <= 0.0) posdiag = false;
            for (size_t j = 0; j < N; ++j)
                if (!close(Sigma(i,j), Sigma(j,i))) sym = false;
        }
        check("Covariance matrix is symmetric", sym);
        check("Covariance diagonal is strictly positive", posdiag);
    }

    // --- Oracle 3: portfolioVariance agrees across two independent routes --
    {
        std::vector<double> w = gmvWeights(returns);
        check("portfolioVariance: double-loop == matmul route",
              close(portfolioVariance(w, Sigma), portfolioVarianceMatmul(w, Sigma)));
    }

    // --- Oracle 4: GMV weights satisfy the budget constraint --------------
    {
        std::vector<double> w = gmvWeights(returns);
        double s = 0.0; for (double wi : w) s += wi;
        check("GMV weights sum to 1", close(s, 1.0, 1e-6));
    }

    // --- Oracle 5: target-return portfolio hits target and sums to 1 ------
    {
        const double target = 0.0008;
        std::vector<double> w = targetReturnWeights(returns, target);
        std::vector<double> mu(N, 0.0);
        for (size_t j = 0; j < N; ++j) {
            double sc = 0.0; for (size_t t = 0; t < T; ++t) sc += returns(t, j);
            mu[j] = sc / T;
        }
        double s = 0.0, r = 0.0;
        for (size_t i = 0; i < N; ++i) { s += w[i]; r += mu[i] * w[i]; }
        check("Target-return weights sum to 1",            close(s, 1.0,   1e-6));
        check("Target-return portfolio achieves its target", close(r, target, 1e-6));
    }

    // --- Oracle 6: GMV is the global minimum-variance portfolio -----------
    // No target-return portfolio may have lower variance than GMV.
    {
        std::vector<double> wGmv = gmvWeights(returns);
        double vGmv = portfolioVariance(wGmv, Sigma);
        bool ok = true;
        for (double R = 0.0; R <= 0.0017; R += 0.0001) {
            std::vector<double> w = targetReturnWeights(returns, R);
            if (portfolioVariance(w, Sigma) < vGmv - 1e-12) ok = false;
        }
        check("GMV has the lowest variance across the frontier", ok);
    }

    std::cout << "\n"
              << (failures == 0 ? "All checks passed."
                                : std::to_string(failures) + " check(s) FAILED.")
              << "\n";
    return failures == 0 ? 0 : 1;
}
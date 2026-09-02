//
// Created by ethan on 27-Aug-26.
//

#include "Matrix.h"
#include <cassert>
#include <cmath>

void luDecompose(const Matrix& A, Matrix& L, Matrix& U) {
    // Assume A is square (n × n). L starts as identity, U starts as a copy of A.
    // for k = 0 to n-1:           (pivot column)
    //     for i = k+1 to n-1:     (rows below the pivot)
    //         factor = U(i,k) / U(k,k)
    //         L(i,k) = factor
    //         for j = k to n-1:   (eliminate across the row)
    //             U(i,j) = U(i,j) - factor * U(k,j)

    assert(A.numRows() == A.numCols());

    size_t n = A.numCols();


    L = make_identity(n);
    U= A;


    for (size_t k= 0 ; k < n; k++) {
        for ( size_t i = k+1 ; i < n; i++ ) {

            double factor = U(i,k)/U(k,k);

            L(i,k) = factor;


            for (size_t j = k ; j < n ; j++) {
               U(i,j) = U(i,j) - (factor * U(k,j));
            }

        }
    }


}

// solve Ly = b, L lower-triangular with 1s on diagonal, return y
std::vector<double> forwardSub(const Matrix& L, const std::vector<double>& b) {
    // for i = 0 to n-1:
    //     sum = b[i]
    //     for j = 0 to i-1:                 (the entries LEFT of the diagonal)
    //         sum = sum - L(i,j) * y[j]
    //     y[i] = sum                        (diagonal is 1, so no division)

    size_t n = L.numRows();
    std::vector<double> y(n, 0.0);          // result, n zeros
    for (size_t i = 0 ; i < n ; i++) {
        double sum= b[i];

        for (size_t j =0; j < i; j ++) {
            sum= sum - (L(i,j) * y[j]);
        }
        y[i]= sum;
    }
    return y;
}

// solve Ux = y, U upper-triangular, return x
std::vector<double> backwardSub(const Matrix& U, const std::vector<double>& y) {
    size_t n = U.numRows();
    std::vector<double> x(n, 0.0);
    // for i from n-1 down to 0 (mind the size_t countdown!):
    //     sum = y[i]
    //     for j = i+1 to n-1:          (entries RIGHT of the diagonal)
    //         sum = sum - U(i,j) * x[j]
    //     x[i] = sum / U(i,i)          (divide by the diagonal!)


    for (size_t i = n; i-- >0;) {
        double sum = y[i];

        for (size_t j = i+1; j < n ; j++) {
            sum = sum - U(i,j) * x[j];
        }
        x[i]= sum/U(i,i);

    }
    return x;
}

std::vector<double> solve(const Matrix& A, const std::vector<double>& b) {
    Matrix L(A.numRows(), A.numCols(), 0.0);
    Matrix U(A.numRows(), A.numCols(), 0.0);
    luDecompose(A, L, U);
    std::vector<double> y = forwardSub(L, b);
    std::vector<double> x = backwardSub(U, y);
    return x;
}

std::vector<double> gmvWeights(const Matrix& returns) {
    // 1. Sigma = computeCovariance(returns)          → N×N
    // 2. ones = vector of N ones
    // 3. x = solve(Sigma, ones)                       → Σ⁻¹1
    // 4. total = sum of x's entries                   → 1ᵀx
    // 5. w[i] = x[i] / total   for each i
    // return w

    size_t N= returns.numCols();

    std::vector<double> w(N,0.0);

    Matrix sigma = computeCovariance(returns);

    std::vector<double> ones(N,1);

    std::vector<double>  x = solve(sigma,ones);
    double total=0.0;

    for (size_t i= 0 ; i< x.size(); i++) {
        total+= x[i];
    }

    for (size_t i= 0 ; i< x.size(); i++) {
        w[i]=x[i]/total;
    }


    return w;

}
std::vector<double> targetReturnWeights(
    const Matrix& returns,
    double targetReturn
) {
    size_t T = returns.numRows();
    size_t N = returns.numCols();

    std::vector<double> w(N, 0.0);

    Matrix sigma = computeCovariance(returns);

    // Calculate expected returns
    std::vector<double> means;

    for (size_t j = 0; j < N; j++) {
        double total = 0.0;

        for (size_t x = 0; x < T; x++) {
            total += returns(x, j);
        }

        means.push_back(total / T);
    }

    std::vector<double> ones(N, 1.0);

    // d1 = Sigma^-1 * 1
    std::vector<double> d1 = solve(sigma, ones);

    // d2 = Sigma^-1 * mu
    std::vector<double> d2 = solve(sigma, means);

    // A = 1' Sigma^-1 1
    double A = 0.0;
    for (size_t i = 0; i < N; i++) {
        A += d1[i];
    }

    // B = 1' Sigma^-1 mu
    double B = 0.0;
    for (size_t i = 0; i < N; i++) {
        B += d2[i];
    }

    // C = mu' Sigma^-1 mu
    double C = 0.0;
    for (size_t i = 0; i < N; i++) {
        C += means[i] * d2[i];
    }

    // Delta = AC - B^2
    double D = A * C - B * B;

    // Lagrange solution coefficients
    double a = (C - B * targetReturn) / D;
    double b = (A * targetReturn - B) / D;

    // w = a Sigma^-1 1 + b Sigma^-1 mu
    for (size_t i = 0; i < N; i++) {
        w[i] = a * d1[i] + b * d2[i];
    }

    return w;
}
double portfolioVariance(const std::vector<double>& w, const Matrix& Sigma) {
    assert(Sigma.numCols() == Sigma.numRows());
    double variance = 0.0;
    for (size_t i = 0; i < Sigma.numRows(); i++) {
        for (size_t j = 0; j < Sigma.numCols(); j++) {
            variance += w[i] * Sigma(i, j) * w[j];   // w[i], not w(i,0)
        }
    }
    return variance;
}

double portfolioVarianceMatmul(const std::vector<double>& w, const Matrix& Sigma) {
    size_t N = w.size();
    Matrix wCol(N, 1, 0.0);
    for (size_t i = 0; i < N; i++) wCol(i, 0) = w[i];
    Matrix quad = wCol.transpose() * (Sigma * wCol);   // (1xN)(NxN)(Nx1) -> 1x1
    assert(quad.numRows() == 1 && quad.numCols() == 1);
    return quad(0, 0);
}
void testPortfolioVariance(const Matrix& returns) {
    Matrix Sigma = computeCovariance(returns);

    // CHECK 1 — same number, two independent routes.
    // Use GMV's weights as the test input (any valid w works, but reusing
    // GMV means this input does double duty for Check 2).
    std::vector<double> w = gmvWeights(returns);

    double vLoop   = portfolioVariance(w, Sigma);        // your double-loop
    double vMatmul = portfolioVarianceMatmul(w, Sigma);  // the matmul route

    std::cout << "Check 1 (double-loop):  " << vLoop   << "\n";
    std::cout << "Check 1 (matmul):       " << vMatmul << "\n";
    // Floating point: never test doubles with ==. Compare within a tolerance.
    assert(std::abs(vLoop - vMatmul) < 1e-9);
    std::cout << "  -> routes agree.\n";

    // CHECK 2 — the GMV tie-back (the frontier's ground-truth anchor).
    // sqrt of GMV's variance = the x-coordinate the frontier's LEFTMOST
    // point must land on when you plot it later. Write this number down.
    double gmvSigma = std::sqrt(vLoop);
    std::cout << "Check 2 (GMV sigma):    " << gmvSigma << "\n";
    std::cout << "  -> the frontier's left tip must equal this.\n";
}

std::vector<std::pair<double,double>> efficientFrontier(
    const Matrix& returns, double Rmin, double Rmax, size_t steps) {
    Matrix Sigma = computeCovariance(returns);

    std::vector<std::pair<double,double>> pairs;

    for (size_t i =0 ; i<steps; i++) {
        double step_size=(Rmax - Rmin) / (steps - 1);

        double target_return = Rmin + (i* step_size);
        std::vector<double> wt=targetReturnWeights(returns, (target_return));

        double sigma = std::sqrt(portfolioVariance(wt, Sigma));

        pairs.push_back({sigma, target_return});

    }
    return pairs;
}

# Portfolio Optimization & Risk Engine

A mean-variance portfolio optimization engine written from scratch in modern C++ — a custom linear-algebra core, a market-data pipeline, and Markowitz optimizers derived from first principles. It takes historical price data and computes the portfolio allocation that minimises risk, either outright or for a chosen target return.

The linear algebra is implemented from the ground up rather than pulled from a library, so every layer — from element access to matrix multiplication to the LU solver and the optimizers built on top — is transparent and self-contained.

> **Status:** the core engine is complete and validated on real US equity data. The optimizers (global-minimum-variance and target-return) work end to end; risk-analysis extensions (efficient frontier, covariance shrinkage, Monte Carlo VaR) are in progress. See [Roadmap](#roadmap).

---

## Motivation

Classical portfolio optimization (Markowitz mean-variance) tells you how to split capital across assets to get the best trade-off between expected return and risk. It's a clean, well-understood method — but it rests on assumptions (clean, reliable return data; well-estimated covariances) that hold far better in deep, liquid markets than in the sparse, noisy data typical of emerging markets.

This engine is built with that gap in mind: a correct, transparent implementation validated on liquid US-equity data, and extensible toward the robustness techniques (covariance shrinkage, simulation-based risk) that matter most when data is imperfect.

---

## What it does

Given a CSV of historical prices for a set of assets, the engine:

1. Loads the prices and computes periodic returns.
2. Estimates the asset covariance matrix Σ (the core object capturing how assets move together).
3. Solves for optimal portfolio weights via one of two optimizers:
   - **Global Minimum Variance (GMV):** the lowest-risk portfolio achievable from the given assets.
   - **Target-return (mean-variance):** the lowest-risk portfolio that achieves a specified expected return.

Both reduce to solving a linear system involving Σ, which the engine does via its own LU decomposition — no matrix inverse is ever formed.

---

## Design goals

- **From-scratch linear algebra** — a `Matrix` type with its own arithmetic, transpose, and an LU-based linear solver, rather than a third-party library. The point is to understand and control every layer.
- **Solve, don't invert** — the optimizers need `Σ⁻¹` applied to a vector, which is a linear system `Σx = b`, solved by LU decomposition (cheaper and more numerically stable than forming a full inverse).
- **Value semantics done right** — the `Matrix` follows the Rule of Zero: it delegates memory management to `std::vector`, so copies are independent and moves are cheap, with no hand-written resource management.
- **Clean separation of concerns** — linear-algebra core, data pipeline, and optimizers are independent components with narrow interfaces.
- **Correctness first** — bounds-checked access in development builds, dimension checks on every operation, and numerical routines tested against hand-computed oracles.

---

## Architecture

```
CSV (prices)
     │  DataLoader: loadCSV → buildMatrix
     ▼
Matrix (prices)          time × assets
     │  computeReturns
     ▼
Matrix (returns)         (time−1) × assets
     │  computeCovariance
     ▼
Covariance matrix Σ      assets × assets
     │  gmvWeights / targetReturnWeights   (solve Σx = b via LU)
     ▼
Optimal weights w
```

**Components**

- **`Matrix`** — a dense matrix backed by a flat `std::vector<double>`, with `(i, j)` element access, arithmetic operators, transpose, and streaming output.
- **`DataLoader`** — reads a price CSV (`time × assets`) into a `Matrix`; computes period returns; estimates the covariance matrix.
- **`LinearAlgebra`** — LU decomposition, forward/backward substitution, and a `solve(A, b)` routine; the GMV and target-return optimizers.

---

## Roadmap

| Component | Status |
|---|---|
| **Matrix core** — element access, `+ − ×` (matrix & scalar), transpose, stream output | ✅ Complete |
| **Data pipeline** — CSV loader, price→returns, matrix bridge | ✅ Complete |
| **Covariance estimation** — sample covariance from the returns matrix | ✅ Complete |
| **Linear solver** — LU decomposition + forward/backward substitution | ✅ Complete |
| **Global Minimum Variance solver** | ✅ Complete |
| **Target-return (mean-variance) solver** | ✅ Complete |
| **Efficient frontier** | 🔨 In progress |
| **Covariance shrinkage** (robustness for noisy data) | 📋 Planned |
| **Monte Carlo Value-at-Risk** (Cholesky + simulation) | 📋 Planned |

---

## The math

- **Expected portfolio return:** `wᵀμ` — weights `w`, expected asset returns `μ`.
- **Portfolio variance (risk):** `wᵀΣw` — where `Σ` is the asset covariance matrix.
- **Global Minimum Variance:** minimise `wᵀΣw` subject to weights summing to 1, giving the closed form `w = Σ⁻¹1 / (1ᵀΣ⁻¹1)`.
- **Target-return portfolio:** minimise `wᵀΣw` subject to weights summing to 1 *and* `wᵀμ = R*`, solved via Lagrange multipliers as a 2×2 system in the multipliers.
- Both closed forms are derived from first principles and reduce to solving `Σx = b`, handled by the LU solver.

---

## Validation

Validated on real US equity data (a diversified basket of large-cap stocks across sectors, pulled via `yfinance`):

- GMV weights sum to 1 and are economically sensible — the optimizer tilts toward the lowest-volatility names, exactly as minimum-variance theory predicts.
- Target-return weights satisfy both constraints simultaneously (sum to 1 *and* achieve the specified return).
- The two optimizers cross-validate: feeding the GMV portfolio's own return as the target to the target-return solver reproduces the GMV weights exactly — independent confirmation that both are correct.

---

## Building & running

Requires a C++20 compiler and CMake.

```bash
cmake -B build
cmake --build build --target engine
./build/engine
```

The engine reads a price CSV in `time × assets` layout — a header row, a date column, then one column of closing prices per asset:

```
Date,AAPL,JPM,XOM,PG,JNJ
2023-01-03,123.6,132.1,104.8,148.2,171.5
2023-01-04,125.1,133.0,106.2,147.9,172.0
```

Clean multi-ticker CSVs are assembled offline in Python (`pull_prices.py`, using `yfinance`); the engine consumes the finished file. This keeps data-wrangling in the tool built for it and the numerical core in C++.

---

## Tech

- **C++20** — value semantics, RAII, operator overloading, move semantics.
- **CMake** build.
- **Python** (`yfinance`) for offline data assembly only.
- No third-party numerical dependencies — the linear algebra is implemented in-repo.

---

*This project is under active development; the risk-analysis extensions are being added through the current term.*

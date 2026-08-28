"""
Pull real US-equity closing prices into the exact CSV shape the engine expects:
    Date,TICKER1,TICKER2,...
    2024-01-02,185.6,374.6,...

Run locally:
    pip install yfinance
    python pull_prices.py

Produces prices.csv (time x assets, one close column per ticker).
"""

import yfinance as yf

# --- pick your assets and window -------------------------------------------
# Deliberately chosen to be reasonably DIVERSE (different sectors) so the
# covariance matrix is well-conditioned and the LU solve behaves. Avoid
# near-identical stocks (e.g. two big-cap tech names that move together) for
# a first test — highly correlated assets make Sigma near-singular.
tickers = ["AAPL", "JPM", "XOM", "PG", "JNJ"]   # tech, bank, energy, staples, healthcare
start   = "2023-01-01"
end     = "2024-01-01"
# ---------------------------------------------------------------------------

# auto_adjust=True gives split/dividend-adjusted closes (what you want for returns)
data = yf.download(tickers, start=start, end=end, auto_adjust=True)["Close"]

# keep column order stable and drop any rows with missing values
data = data[tickers].dropna()

data.to_csv("prices.csv")

print(f"Wrote prices.csv: {data.shape[0]} days x {data.shape[1]} assets")
print("Columns:", list(data.columns))
print(data.head())
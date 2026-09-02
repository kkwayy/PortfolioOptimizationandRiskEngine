import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("frontier.csv")

plt.figure(figsize=(8, 6))
plt.plot(df["sigma"], df["return"], "-", color="#888", zorder=1)
plt.scatter(df["sigma"], df["return"], s=12, zorder=2)

# mark the GMV nose — the minimum-sigma point
gmv = df.loc[df["sigma"].idxmin()]
plt.scatter(gmv["sigma"], gmv["return"], color="red", s=60, zorder=3, label="GMV (min variance)")
plt.annotate("GMV", (gmv["sigma"], gmv["return"]),
             textcoords="offset points", xytext=(10, -5))

plt.xlabel("Risk (σ)")
plt.ylabel("Expected return")
plt.title("Efficient Frontier — 5-asset portfolio")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("frontier.png", dpi=150)
plt.show()
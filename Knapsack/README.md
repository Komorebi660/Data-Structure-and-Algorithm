## 经典动态规划问题——背包问题

- [01背包问题](./01_Knapsack.cpp)

$dp[i][j] = max \{dp[i-1][j], dp[i-1][j-w[i]]+v[i]\}$

- [完全背包问题](./Complete_Knapsack.cpp)

$dp[i][j] = max\{dp[i-1][j], dp[i][j-w[i]]+v[i]\}$

- [多重背包问题](./Muti_Knapsack.cpp)

利用**二进制**的思想转化为01背包问题
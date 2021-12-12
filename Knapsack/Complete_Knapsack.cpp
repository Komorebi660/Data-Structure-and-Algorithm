#include <stdio.h>

int W[200];
int V[200];
int dp[200][10001]; //dp[i][j]表示从0...i中选取物品总重不超过j的最大value

void knapsack(int n, int w)
{
    //初始化
    for (int i = 0; i < n; i++)
        dp[i][0] = 0;
    for (int i = 0; i <= w; i++)
    {
        if (i < W[0])
            dp[0][i] = 0;
        else
            dp[0][i] = V[0];
    }

    //dp[i][j]=max{dp[i-1][j], dp[i][j-w[i]]+v[i]}
    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j <= w; j++)
        {
            if (W[i] > j)
                dp[i][j] = dp[i - 1][j];
            //可以继续选择i物品
            else
            {
                if (dp[i - 1][j] < (dp[i][j - W[i]] + V[i]))
                    dp[i][j] = dp[i][j - W[i]] + V[i];
                else
                    dp[i][j] = dp[i - 1][j];
            }
        }
    }

    printf("%d\n", dp[n - 1][w]);
    return;
}

int main()
{
    int n, w;
    scanf("%d%d", &n, &w);
    for (int i = 0; i < n; i++)
        scanf("%d%d%d", &W[i], &V[i]);
    knapsack(n, w);
    return 0;
}
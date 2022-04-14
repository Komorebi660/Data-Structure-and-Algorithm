/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "Multi_Knapsack.cpp"
 *************************************************************/
#include <stdio.h>

int W[32 * 200];
int V[32 * 200];
int NUM;
int dp[32 * 200][10001]; //dp[i][j]表示从0...i中选取物品总重不超过j的最大value

//将相同重量、价值的物品拆成2^0、2^1、2^2...2^k个物品
void binary_divide(int weight, int value, int num)
{
    for (int i = 0; i < 32; i++)
    {
        int index = 1 << i; //2^i
        if (num < index)
        {
            W[NUM] = num * weight;
            V[NUM] = num * value;
            NUM++;
            break;
        }
        else
        {
            W[NUM] = index * weight;
            V[NUM] = index * value;
            NUM++;
            num -= index;
        }
    }
    return;
}

//01背包问题
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

    //dp[i][j]=max{dp[i-1][j], dp[i-1][j-w[i]]+v[i]}
    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j <= w; j++)
        {
            if (W[i] > j)
                dp[i][j] = dp[i - 1][j];
            else
            {
                if (dp[i - 1][j] < (dp[i - 1][j - W[i]] + V[i]))
                    dp[i][j] = dp[i - 1][j - W[i]] + V[i];
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
    NUM = 0;
    for (int i = 0; i < n; i++)
    {
        int weight, value, num;
        scanf("%d%d%d", &weight, &value, &num);
        binary_divide(weight, value, num);
    }
    knapsack(NUM, w);
    return 0;
}
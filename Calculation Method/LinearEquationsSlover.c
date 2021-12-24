/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//使用双精度浮点在本题中没有误差，故改为单精度观察现象
//#define double float

//Gauss列主元法求解线性方程组
void Gauss(double *a, double *b, int n)
{
    double *_a = (double *)malloc(sizeof(double) * n * n);
    memcpy(_a, a, sizeof(double) * n * n);
    double *_b = (double *)malloc(sizeof(double) * n);
    memcpy(_b, b, sizeof(double) * n);

    for (int i = 0; i < n; i++)
    {
        //找第i列的最大元素
        int k = i;
        for (int j = i + 1; j < n; j++)
        {
            if (fabs(*(_a + j * n + i)) > fabs(*(_a + k * n + i)))
                k = j;
        }
        //交换第k行和第i行
        if (k != i)
        {
            for (int j = i; j < n; j++)
            {
                double temp = *(_a + i * n + j);
                *(_a + i * n + j) = *(_a + k * n + j);
                *(_a + k * n + j) = temp;
            }
            double temp = *(_b + i);
            *(_b + i) = *(_b + k);
            *(_b + k) = temp;
        }
        //特殊情况：第i个主元素为0，则跳过
        if (*(_a + i * n + i) == 0.0)
            continue;
        //逐行消元
        for (int j = i + 1; j < n; j++)
        {
            double temp = (*(_a + j * n + i)) / (*(_a + i * n + i));
            for (int k = i; k < n; k++)
                *(_a + j * n + k) -= *(_a + i * n + k) * temp;
            *(_b + j) -= *(_b + i) * temp;
        }
    }

    //输出上三角矩阵
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%- 8.4lf ", *(_a + i * n + j));
        printf("| %- 8.4lf\n", *(_b + i));
    }
    //求解答案
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = i + 1; j < n; j++)
            *(_b + i) -= (*(_b + j)) * (*(_a + i * n + j));
        *(_b + i) = *(_b + i) / (*(_a + i * n + i));
    }

    //输出结果
    printf("-----RESULT-----\n");
    for (int i = 0; i < n; i++)
        printf("x%d = %- 10.6lf\n", i + 1, _b[i]);
    printf("\n");

    free(_a);
    free(_b);
    return;
}

//Doolittle分解法求解线性方程组
void Doolittle(double *a, double *b, int n)
{
    //初始化U和L
    double *u = (double *)malloc(sizeof(double) * n * n);
    memset(u, 0, sizeof(double) * n * n);
    double *l = (double *)malloc(sizeof(double) * n * n);
    memset(l, 0, sizeof(double) * n * n);

    //计算U和L
    for (int k = 0; k < n; k++)
    {
        double temp;
        for (int j = k; j < n; j++)
        {
            temp = 0.0;
            for (int r = 0; r < k; r++)
                temp += (*(l + k * n + r)) * (*(u + r * n + j));
            *(u + k * n + j) = *(a + k * n + j) - temp;
        }
        for (int i = k + 1; i < n; i++)
        {
            temp = 0.0;
            for (int r = 0; r < k; r++)
                temp += (*(l + i * n + r)) * (*(u + r * n + k));
            *(l + i * n + k) = (*(a + i * n + k) - temp) / (*(u + k * n + k));
        }
        *(l + k * n + k) = 1.0;
    }
    //输出U和L
    printf("U:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%- 8.4lf ", *(u + i * n + j));
        printf("\n");
    }
    printf("L:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%- 8.4lf ", *(l + i * n + j));
        printf("\n");
    }
    //解方程组Ly=b
    double *y = (double *)malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
    {
        double temp = 0.0;
        for (int j = 0; j < i; j++)
            temp += (*(l + i * n + j)) * (*(y + j));
        *(y + i) = *(b + i) - temp;
    }
    //解方程组Ux=y
    double *x = (double *)malloc(sizeof(double) * n);
    for (int i = n - 1; i >= 0; i--)
    {
        double temp = 0.0;
        for (int j = i + 1; j < n; j++)
            temp += (*(u + i * n + j)) * (*(x + j));
        *(x + i) = (*(y + i) - temp) / (*(u + i * n + i));
    }
    //输出结果
    printf("-----RESULT-----\n");
    for (int i = 0; i < n; i++)
        printf("x%d = %- 10.6lf\n", i + 1, x[i]);
    printf("\n");
    free(u);
    free(l);
    free(y);
    free(x);
    return;
}

//Crout分解法求解线性方程组
void Crout(double *a, double *b, int n)
{
    //初始化U和L
    double *u = (double *)malloc(sizeof(double) * n * n);
    memset(u, 0, sizeof(double) * n * n);
    double *l = (double *)malloc(sizeof(double) * n * n);
    memset(l, 0, sizeof(double) * n * n);

    //计算U和L
    for (int k = 0; k < n; k++)
    {
        double temp;
        for (int i = k; i < n; i++)
        {
            temp = 0.0;
            for (int r = 0; r < k; r++)
                temp += (*(l + i * n + r)) * (*(u + r * n + k));
            *(l + i * n + k) = *(a + i * n + k) - temp;
        }
        for (int j = k + 1; j < n; j++)
        {
            temp = 0.0;
            for (int r = 0; r < k; r++)
                temp += (*(l + k * n + r)) * (*(u + r * n + j));
            *(u + k * n + j) = (*(a + k * n + j) - temp) / (*(l + k * n + k));
        }
        *(u + k * n + k) = 1.0;
    }
    //输出U和L
    printf("U:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%- 8.4lf ", *(u + i * n + j));
        printf("\n");
    }
    printf("L:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%- 8.4lf ", *(l + i * n + j));
        printf("\n");
    }
    //解方程组Ly=b
    double *y = (double *)malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
    {
        double temp = 0.0;
        for (int j = 0; j < i; j++)
            temp += (*(l + i * n + j)) * (*(y + j));
        *(y + i) = (*(b + i) - temp) / (*(l + i * n + i));
    }
    //解方程组Ux=y
    double *x = (double *)malloc(sizeof(double) * n);
    for (int i = n - 1; i >= 0; i--)
    {
        double temp = 0.0;
        for (int j = i + 1; j < n; j++)
            temp += (*(u + i * n + j)) * (*(x + j));
        *(x + i) = *(y + i) - temp;
    }
    //输出结果
    printf("-----RESULT-----\n");
    for (int i = 0; i < n; i++)
        printf("x%d = %- 10.6lf\n", i + 1, x[i]);
    printf("\n");
    free(u);
    free(l);
    free(y);
    free(x);
    return;
}

int main()
{
    double a1[5][5] = {{1.0 / 9.0, 1.0 / 8.0, 1.0 / 7.0, 1.0 / 6.0, 1.0 / 5.0},
                       {1.0 / 8.0, 1.0 / 7.0, 1.0 / 6.0, 1.0 / 5.0, 1.0 / 4.0},
                       {1.0 / 7.0, 1.0 / 6.0, 1.0 / 5.0, 1.0 / 4.0, 1.0 / 3.0},
                       {1.0 / 6.0, 1.0 / 5.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 2.0},
                       {1.0 / 5.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 2.0, 1.0}};
    double b1[5] = {1.0, 1.0, 1.0, 1.0, 1.0};
    double a2[4][4] = {{7.2, 2.3, -4.4, 0.5},
                       {1.3, 6.3, -3.5, 2.8},
                       {5.6, 0.9, 8.1, -1.3},
                       {1.5, 0.4, 3.7, 5.9}};
    double b2[4] = {15.1, 1.8, 16.6, 36.9};

    printf("Example 1:\n");
    printf("Doolittle Method:\n");
    Doolittle((double *)a1, b1, 5);
    printf("Crout Method:\n");
    Crout((double *)a1, b1, 5);
    printf("Gauss Method:\n");
    Gauss((double *)a1, b1, 5);

    printf("Example 2:\n");
    printf("Doolittle Method:\n");
    Doolittle((double *)a2, b2, 4);
    printf("Crout Method:\n");
    Crout((double *)a2, b2, 4);
    printf("Gauss Method:\n");
    Gauss((double *)a2, b2, 4);
    return 0;
}
/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
            printf("%-10.6lf ", *(_a + i * n + j));
        printf("| %-10.6lf\n", *(_b + i));
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
        printf("x%d = %-10.6lf\n", i + 1, _b[i]);
    printf("\n");

    free(_a);
    free(_b);
    return;
}

//多项式最小二乘拟合
//n个采样点(x,y)；m次多项式
void Slove(double *x, double *y, int n, int m)
{
    double *a = (double *)malloc(sizeof(double) * m * m);
    double *b = (double *)malloc(sizeof(double) * m);

    double *A = (double *)malloc(sizeof(double) * n * m);
    double *A_T = (double *)malloc(sizeof(double) * m * n);

    //生成A和A^T
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            *(A + i * m + j) = pow(x[i], (double)j);
            *(A_T + j * n + i) = *(A + i * m + j);
        }
    }

    //计算A^T*A
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            *(a + i * m + j) = 0.0;
            for (int k = 0; k < n; k++)
            {
                *(a + i * m + j) += (*(A_T + i * n + k)) * (*(A + k * m + j));
            }
            printf("%-10.6lf ", *(a + i * m + j));
        }
        printf("\n");
    }
    printf("\n");

    //计算A^T*b
    for (int i = 0; i < m; i++)
    {
        *(b + i) = 0.0;
        for (int j = 0; j < n; j++)
        {
            *(b + i) += (*(A_T + i * n + j)) * (*(y + j));
        }
        printf("%-10.6lf ", *(b + i));
    }
    printf("\n\n");

    Gauss(a, b, m);
    return;
}
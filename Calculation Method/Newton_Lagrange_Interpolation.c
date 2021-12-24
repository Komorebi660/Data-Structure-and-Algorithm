/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double Lagrange(double *x, double *y, int n, double x0)
{
    double fx = 0.0;
    for (int i = 0; i < n; i++)
    {
        double tmp = 1.0;
        for (int j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            else
                tmp *= ((x0 - x[j]) / (x[i] - x[j]));
        }
        fx += (tmp * y[i]);
    }
    return fx;
}

double Newton(double *x, double *y, int n, double x0)
{
    //计算差商表
    double *g = (double *)malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
    {
        g[i] = y[i];
        printf("%10.6lf ", g[i]);
    }
    printf("\n");
    //计算i阶差商
    for (int i = 1; i < n; i++)
    {
        //逆序计算
        for (int j = n - 1; j >= i; j--)
        {
            g[j] = (g[j] - g[j - 1]) / (x[j] - x[j - i]);
        }
        //输出
        for (int j = i; j < n; j++)
        {
            printf("%10.6lf ", g[j]);
        }
        printf("\n");
    }

    //Newton插值
    double fx = g[0];
    double tmp = 1.0;
    for (int i = 1; i < n; i++)
    {
        tmp *= (x0 - x[i - 1]);
        fx += tmp * g[i];
    }
    free(g);
    return fx;
}

int main()
{
    double x[4] = {-2.0, 0.0, 1.0, 2.0};
    double y[4] = {17.0, 1.0, 2.0, 19.0};
    double result;

    printf("Lagrange:\n");
    result = Lagrange(x, y, 4, 0.9);
    printf("%lf\n", result);

    printf("\nNewton:\n");
    printf("=========================================\n");
    result = Newton(x, y, 4, 0.9);
    printf("=========================================\n");
    printf("%lf\n", result);

    return 0;
}
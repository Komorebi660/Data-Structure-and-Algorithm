/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define N 20

struct pair
{
    double x;
    double y;
};

int mycmp(const void *a, const void *b)
{
    double a_tmp = ((struct pair *)a)->x;
    double b_tmp = ((struct pair *)b)->x;
    if (a_tmp > b_tmp)
        return 1;
    else
        return -1;
}

double m0_1 = -0.333333;
double mn_1 = -0.0340136;
double M0_1 = -0.222222;
double Mn_1 = 0.0110139;
double f1(double x)
{
    return x / (x * x + x + 1.0);
}

double m0_2 = -0.167157;
double mn_2 = 0.784373;
double M0_2 = -0.524788;
double Mn_2 = -3.94616;
double f2(double x)
{
    return (x + sin(2.0 * x)) / (1.0 + exp(-x));
}

//追赶法求三对角矩阵
//a[0...n-1]:主对角；b[0..n-2]:上对角；c[1..n-1]:下对角；f:y；n:维数
void Tridiagonal(double *a, double *b, double *c, double *f, double *x, int n)
{
    double *u = (double *)malloc(sizeof(double) * n);
    double *v = (double *)malloc(sizeof(double) * n);
    double *y = (double *)malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            u[0] = a[0];
            v[0] = b[0] / u[0];
            y[0] = f[0] / u[0];
        }
        else
        {
            u[i] = a[i] - c[i] * v[i - 1];
            v[i] = b[i] / u[i];
            y[i] = (f[i] - c[i] * y[i - 1]) / u[i];
        }
    }
    for (int i = n - 1; i >= 0; i--)
    {
        if (i == n - 1)
            x[i] = y[i];
        else
            x[i] = y[i] - v[i] * x[i + 1];
    }
    free(u);
    free(v);
    free(y);
    return;
}

//追赶法求三对角矩阵
//(x,y):插值采样点；n:维数；mo,mn:边界点；con:二阶导作为边界还是一阶导作为边界
void CubicSplineInterpolation(double *x, double *y, int n, double m0, double mn, bool con)
{
    printf("h:\n");
    double *h = (double *)malloc(sizeof(double) * n);
    for (int i = 0; i < n - 1; i++)
    {
        h[i] = x[i + 1] - x[i];
        printf("%lf ", h[i]);
    }
    printf("\n");

    printf("(u, v):\n");
    double *u = (double *)malloc(sizeof(double) * n);
    double *v = (double *)malloc(sizeof(double) * n);
    for (int i = 1; i < n - 1; i++)
    {
        u[i] = h[i] / (h[i] + h[i - 1]);
        v[i] = 1.0 - u[i];
        printf("(%lf, %lf)  ", u[i], v[i]);
    }
    printf("\n");
    u[0] = 1.0;
    v[0] = 0.0;
    u[n - 1] = 0.0;
    v[n - 1] = 1.0;

    double *a = (double *)malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
        a[i] = 2.0;

    double *d = (double *)malloc(sizeof(double) * n);
    for (int i = 0; i < n - 1; i++)
        d[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
    for (int i = n - 2; i >= 1; i--)
        d[i] = 6.0 * (d[i] - d[i - 1]) / (x[i + 1] - x[i - 1]);

    double *M = (double *)malloc(sizeof(double) * n);
    if (con)
    {
        d[1] -= v[1] * m0;
        d[n - 2] -= v[n - 2] * mn;
        printf("d:\n");
        for (int i = 1; i <= n - 2; i++)
            printf("%lf ", d[i]);
        printf("\n");

        Tridiagonal(a + 1, u + 1, v + 1, d + 1, M + 1, n - 2);
        M[n - 1] = mn;
        M[0] = m0;
        printf("M:\n");
        for (int i = 0; i < n; i++)
            printf("%lf ", M[i]);
    }
    else
    {
        d[0] = 6.0 * ((y[1] - y[0]) / (x[1] - x[0]) - m0) / h[0];
        d[n - 1] = 6.0 * (mn - (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2])) / h[n - 2];
        printf("d:\n");
        for (int i = 0; i <= n - 1; i++)
            printf("%lf ", d[i]);
        printf("\n");

        Tridiagonal(a, u, v, d, M, n);
        printf("M:\n");
        for (int i = 0; i < n; i++)
            printf("%lf ", M[i]);
    }
    printf("\n");

    printf("result:\n");
    printf("{");
    for (int i = 0; i < n - 1; i++)
    {
        printf("{");
        printf("%lf*(%lf-x)^3", M[i] / (6.0 * h[i]), x[i + 1]);
        printf("%+lf*(x-(%lf))^3", M[i + 1] / (6.0 * h[i]), x[i]);
        printf("%+lf*(%lf-x)", (y[i] / h[i]) - (M[i] * h[i] / 6.0), x[i + 1]);
        printf("%+lf*(x-(%lf))", (y[i + 1] / h[i]) - (M[i + 1] * h[i] / 6.0), x[i]);
        printf(",");
        if (i == n - 2)
            printf("%lf< x < %lf}", x[i], x[i + 1]);
        else
            printf("%lf< x < %lf},", x[i], x[i + 1]);
    }
    printf("}\n");
    free(h);
    free(u);
    free(v);
    free(a);
    free(d);
    free(M);
    return;
}

void run(bool uniformity)
{
    srand(time(NULL));
    struct pair dots[N];
    double x[N];
    double y[N];
    if (uniformity) //均匀采样
    {
        printf("Example 1:\n");
        printf("============================================================================================\n");
        double index = 4.0 / (N - 1);
        printf("{");
        for (int i = 0; i < N; i++)
        {
            x[i] = -2.0 + index * i;
            y[i] = f1(x[i]);

            if (i == N - 1)
                printf("{%lf, %lf}", x[i], y[i]);
            else
                printf("{%lf, %lf},", x[i], y[i]);
        }
        printf("}\n\n");

        printf("Second derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, M0_1, Mn_1, true);
        printf("\nFirst derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, m0_1, mn_1, false);

        printf("\nExample 2:\n");
        printf("============================================================================================\n");
        index = 6.0 / (N - 1);
        printf("{");
        for (int i = 0; i < N; i++)
        {
            x[i] = -2.0 + index * i;
            y[i] = f2(x[i]);

            if (i == N - 1)
                printf("{%lf, %lf}", x[i], y[i]);
            else
                printf("{%lf, %lf},", x[i], y[i]);
        }
        printf("}\n\n");

        printf("Second derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, M0_2, Mn_2, true);
        printf("\nFirst derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, m0_2, mn_2, false);
    }
    else
    {
        printf("Example 1:\n");
        printf("============================================================================================\n");
        dots[0].x = -2.0;
        dots[0].y = f1(-2.0);
        dots[N - 1].x = 2.0;
        dots[N - 1].y = f1(2.0);
        for (int i = 1; i < N - 1; i++) //生成随机采样点
        {
            dots[i].x = -2.0 + rand() / (RAND_MAX + 0.0) * 4.0;
            dots[i].y = f1(dots[i].x);
        }
        qsort(dots, N, sizeof(struct pair), mycmp); //从小到大排列

        printf("{");
        for (int i = 0; i < N; i++)
        {
            if (i == N - 1)
                printf("{%lf, %lf}", dots[i].x, dots[i].y);
            else
                printf("{%lf, %lf},", dots[i].x, dots[i].y);
            x[i] = dots[i].x;
            y[i] = dots[i].y;
        }
        printf("}\n\n");

        printf("Second derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, M0_1, Mn_1, true);
        printf("\nFirst derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, m0_1, mn_1, false);

        printf("\nExample 2:\n");
        printf("============================================================================================\n");
        dots[0].x = -2.0;
        dots[0].y = f2(-2.0);
        dots[N - 1].x = 4.0;
        dots[N - 1].y = f2(4.0);
        for (int i = 1; i < N - 1; i++) //生成随机采样点
        {
            dots[i].x = -2.0 + rand() / (RAND_MAX + 0.0) * 6.0;
            dots[i].y = f2(dots[i].x);
        }
        qsort(dots, N, sizeof(struct pair), mycmp); //从小到大排列

        printf("{");
        for (int i = 0; i < N; i++)
        {
            if (i == N - 1)
                printf("{%lf, %lf}", dots[i].x, dots[i].y);
            else
                printf("{%lf, %lf},", dots[i].x, dots[i].y);
            x[i] = dots[i].x;
            y[i] = dots[i].y;
        }
        printf("}\n\n");

        printf("Second derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, M0_2, Mn_2, true);
        printf("\nFirst derivative boundary:\n");
        CubicSplineInterpolation(x, y, N, m0_2, mn_2, false);
    }
    return;
}

int main()
{
    /*
    printf("------- uniform sampling -------\n");
    run(true);
   
    printf("\n\n------- random sampling -------\n");
    run(false);
     */

    double x[4] = {-2.0, -1.0, 1.0, 2.0};
    double y[4] = {-4.0, 3.0, 5.0, 12.0};
    CubicSplineInterpolation(x, y, 4, 0.0, 0.0, true);
    return 0;
}
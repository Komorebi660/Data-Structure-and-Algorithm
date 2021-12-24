/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 1e-6
#define M 8

double f(double x)
{
    return 4.0 / (1.0 + x * x);
}

//求f在区间[a, b]上的复化梯形积分
void CompoundTrapezoid(double (*f)(double), double a, double b)
{
    printf("N  \tResult\n");
    double h = (b - a) / 2.0;
    int n = 2;
    double T2 = h * (f(a) / 2.0 + f(b) / 2.0 + f((a + b) / 2.0));
    double T1 = T2 + 100.0;
    printf("%-5d\t%-10.8lf\n", n, T2);
    while (fabs(T1 - T2) > EPSILON)
    {
        T1 = T2;

        h = h / 2.0;
        double H = 0.0;
        for (int i = 1; i <= n; i++)
            H += f(a + (2 * i - 1) * h);

        T2 = T1 / 2.0 + h * H;
        n = n << 1;
        printf("%-5d\t%-10.8lf\n", n, T2);
    }
    return;
}

//求f在区间[a, b]上的复化Simpson积分
void CompoundSimpson(double (*f)(double), double a, double b)
{
    printf("N  \tResult\n");
    double h = (b - a) / 2.0;
    int n = 2;
    double T2 = h * (f(a) + 4.0 * f((a + b) / 2.0) + f(b)) / 3.0;
    double T1 = T2 + 100.0;
    printf("%-5d\t%-10.8lf\n", n, T2);
    while (fabs(T1 - T2) > EPSILON)
    {
        T1 = T2;

        double H2 = 0.0;
        for (int i = 1; i <= n >> 1; i++)
            H2 += f(a + (2 * i - 1) * h);

        h = h / 2.0;
        double H1 = 0.0;
        for (int i = 1; i <= n; i++)
            H1 += f(a + (2 * i - 1) * h);

        T2 = T1 / 2.0 + H1 * h * 4.0 / 3.0 - H2 * h * 2.0 / 3.0;
        n = n << 1;
        printf("%-5d\t%-10.8lf\n", n, T2);
    }
    return;
}

//利用Romberg算法求f在区间[a, b]上的积分
void Romberg(double (*f)(double), double a, double b)
{
    double h = b - a;
    int n = 1;
    double *R0 = (double *)malloc(sizeof(double) * M);
    double *R1 = (double *)malloc(sizeof(double) * M);
    R0[0] = h * (f(a) + f(b)) / 2.0;
    printf("%10.8lf\n", R0[0]);
    for (int k = 1; k < M; k++)
    {
        h = h / 2.0;
        double temp = 0.0;
        for (int i = 1; i <= n; i++)
            temp += f(a + (2 * i - 1) * h);
        R1[0] = R0[0] / 2.0 + h * temp;

        int m = 4;
        for (int j = 1; j <= k; j++)
        {
            R1[j] = R1[j - 1] + (R1[j - 1] - R0[j - 1]) / (m - 1);
            m = m << 2;
        }
        for (int i = 0; i <= k; i++)
            printf("%10.8lf  ", R1[i]);

        if (fabs(R0[k - 1] - R1[k]) < EPSILON)
            break;
        memcpy(R0, R1, sizeof(double) * M);
        printf("\n");
        n = n << 1;
    }
    return;
}

int main()
{
    printf("----Trapezoid----\n");
    CompoundTrapezoid(f, 0.0, 1.0);
    printf("\n----Simpson----\n");
    CompoundSimpson(f, 0.0, 1.0);
    printf("\n----Romberg----\n");
    Romberg(f, 0.0, 1.0);
    return 0;
}
/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 1e-6

double f(double x, double y)
{
    return y * y * cos(x);
}

void Euler_Forward(double (*f)(double, double), double a, double b, double y, double h)
{
    double x = a;
    double y0 = y;
    printf("%10.5lf %10.5lf\n", x, y0);
    double y1 = y0 + h * f(x, y0);
    while (x < b)
    {
        y0 = y1;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y0);
        y1 = y0 + h * f(x, y0);
    }
    return;
}

void Euler_Backward(double (*f)(double, double), double a, double b, double y, double h)
{
    double x = a;
    double y0 = y;
    printf("%10.5lf %10.5lf\n", x, y0);
    double y1 = 0.0;
    double y1_ = 0.0;
    while (x < b)
    {
        y1 = y0 + h * f(x, y0);
        x += h;
        y1_ = y0 + h * f(x, y1);
        while (fabs(y1_ - y1) > EPSILON)
        {
            y1 = y1_;
            y1_ = y0 + h * f(x, y1);
        }
        printf("%10.5lf %10.5lf\n", x, y1_);
        y0 = y1_;
    }
    return;
}

void Euler_OPT(double (*f)(double, double), double a, double b, double y, double h)
{
    double x = a;
    double y0 = y;
    printf("%10.5lf %10.5lf\n", x, y0);
    double y1 = 0.0;
    double y1_ = 0.0;
    while (x < b)
    {
        double x_ = x + h;
        y1 = y0 + h * f(x, y0);
        y1_ = y0 + h * f(x, y0) / 2.0 + h * f(x_, y1) / 2.0;
        while (fabs(y1_ - y1) > EPSILON)
        {
            y1 = y1_;
            y1_ = y0 + h * f(x, y0) / 2.0 + h * f(x_, y1) / 2.0;
        }
        x += h;
        printf("%10.5lf %10.5lf\n", x, y1_);
        y0 = y1_;
    }
    return;
}

void Runge_Kutta(double (*f)(double, double), double a, double b, double y0, double h)
{
    printf("Pattern 1 for 2nd order accuracy:\n");
    double x = a;
    double y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h, y + h * k1);
        y = y + h * (k1 + k2) / 2.0;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }

    printf("Pattern 2 for 2nd order accuracy:\n");
    x = a;
    y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h / 2.0, y + h * k1 / 2.0);
        y = y + h * k2;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }

    printf("Pattern 1 for 3rd order accuracy:\n");
    x = a;
    y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h / 2.0, y + h * k1 / 2.0);
        double k3 = f(x + h, y - h * k1 + h * k2 * 2.0);
        y = y + h * (k1 + 4.0 * k2 + k3) / 6.0;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }

    printf("Pattern 2 for 3rd order accuracy:\n");
    x = a;
    y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h / 3.0, y + h * k1 / 3.0);
        double k3 = f(x + h * 2.0 / 3.0, y + h * k2 * 2.0 / 3.0);
        y = y + h * (k1 + 3.0 * k3) / 4.0;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }

    printf("Pattern 3 for 3rd order accuracy:\n");
    x = a;
    y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h / 2.0, y + h * k1 / 2.0);
        double k3 = f(x + h * 3.0 / 4.0, y + h * k2 * 3.0 / 4.0);
        y = y + h * (2.0 * k1 + 3.0 * k2 + 4.0 * k3) / 9.0;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }

    printf("Pattern 1 for 4th order accuracy:\n");
    x = a;
    y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h / 2.0, y + h * k1 / 2.0);
        double k3 = f(x + h / 2.0, y + h * k2 / 2.0);
        double k4 = f(x + h, y + h * k3);
        y = y + h * (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }

    printf("Pattern 2 for 4th order accuracy:\n");
    x = a;
    y = y0;
    printf("%10.5lf %10.5lf\n", x, y0);
    while (x < b)
    {
        double k1 = f(x, y);
        double k2 = f(x + h / 3.0, y + h * k1 / 3.0);
        double k3 = f(x + h * 2.0 / 3.0, y + h * k1 / 3.0 + h * k2);
        double k4 = f(x + h, y + h * k1 - h * k2 + h * k3);
        y = y + h * (k1 + 3.0 * k2 + 3.0 * k3 + k4) / 8.0;
        x += h;
        printf("%10.5lf %10.5lf\n", x, y);
    }
    return;
}

int main()
{
    printf("------Euler Forward:------\n");
    Euler_Forward(f, 0.0, 0.5, 1.0, 0.1);
    printf("------Euler Backward:------\n");
    Euler_Backward(f, 0.0, 0.5, 1.0, 0.1);
    printf("------Euler Optimal:------\n");
    Euler_OPT(f, 0.0, 0.5, 1.0, 0.1);
    printf("------Runge-Kutta:------\n");
    Runge_Kutta(f, 0.0, 0.5, 1.0, 0.1);
    return 0;
}
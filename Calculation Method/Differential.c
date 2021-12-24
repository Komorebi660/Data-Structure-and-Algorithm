/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 1e-6

double f(double x)
{
    return pow(x, 3.0) + 2.0 * sin(x) - exp(x);
}

//求f在x处的导数
double Differential(double (*f)(double), double x)
{
    printf("ITERATION    h\t     RESULT\n");
    double h = 1.0;
    int i = 1;
    double temp1 = (f(x + h) - f(x - h)) / (2 * h);
    printf("%4d\t  %1.4lf    %-10.6lf\n", i++, h, temp1);
    h = h / 2.0;
    double temp2 = (f(x + h) - f(x - h)) / (2 * h);
    while (fabs(temp2 - temp1) > EPSILON)
    {
        temp1 = temp2;
        printf("%4d\t  %1.4lf    %-10.6lf\n", i++, h, temp1);
        h = h / 2.0;
        temp2 = (f(x + h) - f(x - h)) / (2 * h);
    }
    return temp2;
}

int main()
{
    Differential(f, 1.0);
    return 0;
}
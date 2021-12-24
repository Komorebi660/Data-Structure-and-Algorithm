/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <math.h>

//maxmium iteration paths
#define MAX_ITERATION_NUM 100
//output pattern string
#define PATTERN "iteration  %-2d: f(% .6lf ) = % .6lf\n"
//end flag of iteration
#define EPSILON pow(10.0, -3.0)
//start of Newton Iteration Method
#define X0(start, end) (start + end) / 2.0

//example 1
double f1(double x)
{
    return (pow((x - 1.0), 3.0) - pow(x, 2.0) + x);
}
double f1_(double x)
{
    return (3.0 * pow((x - 1.0), 2.0) - 2.0 * x + 1.0);
}
double start1 = 2.0;
double end1 = 3.0;
//example 2
double f2(double x)
{
    return (pow(sin(x), 3.0) + pow(cos(x), 3.0));
}
double f2_(double x)
{
    return (3.0 * pow(sin(x), 2.0) * cos(x) - 3.0 * pow(cos(x), 2.0) * sin(x));
}
double start2 = 2.0;
double end2 = 3.0;
//example 3
double f3(double x)
{
    return (exp(x) * log(x) - pow(x, 2.0));
}
double f3_(double x)
{
    return (exp(x) * log(x) + exp(x) / x - 2.0 * x);
}
double start3 = 1.0;
double end3 = 2.0;
//example 4
double f4(double x)
{
    return (pow((x - 2.0), 5.0) - sin(x));
}
double f4_(double x)
{
    return (5.0 * pow((x - 2.0), 4.0) - cos(x));
}
double start4 = 2.0;
double end4 = 3.0;
//example 5
double f5(double x)
{
    return (cos(x) - exp(x));
}
double f5_(double x)
{
    return (-sin(x) - exp(x));
}
double start5 = -2.0;
double end5 = -1.0;

class EquationSlover
{
private:
    int method_select;
    double (*f)(double);
    double (*f_)(double);
    double start_value;
    double end_value;
    double result;
    bool Dichotomy();
    bool NewtonIteration();
    bool StringInterception();

public:
    EquationSlover(int, double (*)(double), double (*)(double), double, double);
    void calculate();
};
//initial
EquationSlover::EquationSlover(int method, double (*func)(double), double (*func_)(double), double start, double end)
{
    method_select = method;
    f = func;
    f_ = func_;
    start_value = start;
    end_value = end;
    result = 0.0;
}

bool EquationSlover::Dichotomy()
{
    double start = start_value;
    double end = end_value;
    double f_start = f(start_value);
    double f_end = f(end_value);

    printf("start point  : f(% .6lf ) = % .6lf\n", start, f_start);
    printf("end   point  : f(% .6lf ) = % .6lf\n", end, f_end);

    //exception condition
    if (start > end)
    {
        result = -1.0;
        return false;
    }
    if (f_start * f_end > 0.0)
    {
        result = -1.0;
        return false;
    }

    int i = 1;
    //optimal at endpoint
    if (fabs(f_start) <= EPSILON)
    {
        printf(PATTERN, i, start, f_start);
        result = start;
        return true;
    }
    if (fabs(f_end) <= EPSILON)
    {
        printf(PATTERN, i, end, f_end);
        result = end;
        return true;
    }

    //main loop
    for (int j = 0; j < MAX_ITERATION_NUM; j++)
    {
        //calculate next iteration
        double mid = (start + end) / 2.0;
        double f_mid = f(mid);
        printf(PATTERN, i, mid, f_mid);
        //printf("{%d, %.6lf}, ", i, fabs(f_mid));
        i++;

        //judge for every condition
        if (fabs(f_mid) < EPSILON)
        {
            result = mid;
            return true;
        }
        else if (f_start * f_mid < 0.0)
        {
            end = mid;
            f_end = f_mid;
        }
        else
        {
            start = mid;
            f_start = f_mid;
        }
    }
    return false;
}

bool EquationSlover::NewtonIteration()
{
    double x0 = X0(start_value, end_value);
    for (int i = 1; i < MAX_ITERATION_NUM; i++)
    {
        double f_x0 = f(x0);
        double f__x0 = f_(x0);
        double x1 = x0 - f_x0 / f__x0;

        printf(PATTERN, i, x0, f_x0);
        //printf("{%d, %.6lf}, ", i, fabs(f_x0));

        if (fabs(f_x0) < EPSILON)
        {
            result = x1;
            return true;
        }
        //iteration
        x0 = x1;
    }
    return false;
}

bool EquationSlover::StringInterception()
{
    double x0 = start_value;
    double x1 = end_value;
    for (int i = 1; i < MAX_ITERATION_NUM; i++)
    {
        double f_x0 = f(x0);
        double f_x1 = f(x1);
        double x_next = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);

        printf(PATTERN, i, x0, f_x0);
        //printf("{%d, %.6lf}, ", i, fabs(f_x0));

        if (fabs(f_x0) < EPSILON)
        {
            result = x1;
            printf(PATTERN, i + 1, x1, f_x1);
            return true;
        }
        //iteration
        x0 = x1;
        x1 = x_next;
    }
    return false;
}

//output the result
void EquationSlover::calculate()
{
    if (method_select == 0)
    {
        printf("Dichotomy Method:\n");
        if (!Dichotomy())
            printf("CAN NOT FIGURE OUT!\n");
        else
            printf("ITERATION OVER!  result = %.10lf\n", result);
    }
    else if (method_select == 1)
    {
        printf("Newton Iteration Method:\n");
        if (!NewtonIteration())
            printf("CAN NOT FIGURE OUT!\n");
        else
            printf("ITERATION OVER!  result = %.10lf\n", result);
    }
    else if (method_select == 2)
    {
        printf("String Interception Method:\n");
        if (!StringInterception())
            printf("CAN NOT FIGURE OUT!\n");
        else
            printf("ITERATION OVER!  result = %.10lf\n", result);
    }
    else
    {
        printf("Dichotomy Method:\n");
        if (!Dichotomy())
            printf("CAN NOT FIGURE OUT!\n");
        else
            printf("ITERATION OVER!  result = %.10lf\n", result);

        printf("\nNewton Iteration Method:\n");
        if (!NewtonIteration())
            printf("CAN NOT FIGURE OUT!\n");
        else
            printf("ITERATION OVER!  result = %.10lf\n", result);

        printf("\nString Interception Method:\n");
        if (!StringInterception())
            printf("CAN NOT FIGURE OUT!\n");
        else
            printf("ITERATION OVER!  result = %.10lf\n", result);
    }
}

int main(void)
{
    //EquationSlover test(0, f1, f1_, start1, end1);
    //EquationSlover test(1, f2, f2_, start2, end2);
    //EquationSlover test(2, f3, f3_, start3, end3);
    //EquationSlover test(3, f4, f4_, start4, end4);
    EquationSlover test(3, f5, f5_, start5, end5);
    test.calculate();
    return 0;
}
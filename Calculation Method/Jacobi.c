/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 1e-2

void Jacobi(double *a, int n)
{
    double m = 0.0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            m += (*(a + i * n + j)) * (*(a + i * n + j));
        }
    }

    while (2 * m > EPSILON)
    {
        //选取 max{ |a[i][j]| }
        int p = 0, q = 1;
        double max = fabs(*(a + 1)); // |a[0][1]|
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {

                if (fabs(*(a + i * n + j)) > max)
                {
                    p = i;
                    q = j;
                    max = fabs(*(a + i * n + j));
                }
            }
        }

        //计算旋转角度
        double s = (*(a + q * n + q) - *(a + p * n + p)) / (2 * (*(a + p * n + q)));
        double t, c, d;
        if (s == 0.0)
            t = 1.0;
        else if (s < 0.0)
            t = -s - sqrt(s * s + 1.0);
        else
            t = -s + sqrt(s * s + 1.0);
        c = 1.0 / sqrt(t * t + 1.0);
        d = t / sqrt(t * t + 1.0);
        printf("%8.4lf %8.4lf %8.4lf %8.4lf\n", s, t, c, d);

        //更新矩阵
        for (int i = 0; i < n; i++)
        {
            if (i == p)
                *(a + p * n + p) -= (t * (*(a + p * n + q)));
            else if (i == q)
                *(a + q * n + q) += (t * (*(a + p * n + q)));
            else
            {
                double temp1, temp2;
                temp1 = c * (*(a + p * n + i)) - d * (*(a + q * n + i));
                temp2 = c * (*(a + q * n + i)) + d * (*(a + p * n + i));
                *(a + i * n + p) = temp1;
                *(a + p * n + i) = *(a + i * n + p);
                *(a + i * n + q) = temp2;
                *(a + q * n + i) = *(a + i * n + q);
            }
        }
        *(a + p * n + q) = 0.0;
        *(a + q * n + p) = 0.0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                printf("%8.4lf ", *(a + i * n + j));
            printf("\n");
        }
        printf("\n");

        //计算非对角元的平方和
        m = 0.0;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                m += (*(a + i * n + j)) * (*(a + i * n + j));
            }
        }
    }
    for (int i = 0; i < n; i++)
        printf("%8.4lf\n", *(a + i * n + i));
    return;
}

int main()
{
    double a[3][3] = {{1.0, -1.0, 0.0}, {-1.0, 2.0, 2.0}, {0.0, 2.0, 3.0}};
    Jacobi((double *)a, 3);
    return 0;
}

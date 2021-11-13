/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "SOR.cpp"
* Function:  Calculate Linear Equations by Iteration
*************************************************************/
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

#define EPSILON 1e-5

void SOR(double *a, int n, double *b, double *x, double w)
{
    //////////////////////////////////////////////////////////
    // 利用SOR迭代法求解线性方程组Ax=b，w=1时为Gauss-Seidel迭代
    // a: 系数矩阵A
    // n: 矩阵维度为nxn
    // b: 非齐次项
    // x: 初始值，未来运算结果也存储在x中
    // w: 迭代权重
    //////////////////////////////////////////////////////////

    //为了计算迭代终止条件需要保存上一次的结果
    double *buffer = (double *)malloc(sizeof(double) * n);
    memcpy(buffer, x, sizeof(double) * n);
    //迭代
    while (1)
    {
        double max = 0.0;
        //求这一轮的x
        for (int i = 0; i < n; i++)
        {
            //x[i]存储了上一轮的结果，这一轮x[i]的权重是(1-w)，所以只需要减去w*x[i]即可
            *(x + i) -= w * (*(x + i));
            for (int j = 0; j < n; j++)
            {
                if (i == j)
                    continue;
                else
                    *(x + i) += w * (*(x + j)) * (-(*(a + i * n + j)) / (*(a + i * n + i)));
            }
            *(x + i) += w * (*(b + i)) / (*(a + i * n + i));
            printf("% -10.6lf ", *(x + i));

            //求|x(k+1)-x(k)|的最大值，确定跳出循环的时机
            double temp = fabs(*(x + i) - *(buffer + i));
            if (temp > max)
                max = temp;
        }
        printf("\n");
        //判断是否达到迭代终止条件
        if (max < EPSILON)
            break;
        memcpy(buffer, x, sizeof(double) * n);
    }
}

int main()
{
    double a[3][3] = {{2.0, -1.0, -1.0}, {1.0, 5.0, -1.0}, {1.0, 1.0, 10.0}};
    double b[3] = {-5.0, 8.0, 11.0};
    double x[3] = {1.0, 1.0, 1.0};
    SOR((double *)a, 3, b, x, 1);
    printf("Done!\n");
    return 0;
}
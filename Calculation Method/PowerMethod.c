/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define N 100
#define EPSILON 1e-5

//判断x是否收敛(计算||x1-x2||)
bool Convergence(double *x1, double *x2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (fabs(x1[i] - x2[i]) > EPSILON)
            return false;
    }
    return true;
}

//判断x1?=x2
bool IfEqual(double *x1, double *x2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (fabs(x1[i] - x2[i]) > EPSILON)
            return false;
    }
    return true;
}

//判断x1?=-x2
bool IfReverse(double *x1, double *x2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (fabs(x1[i] + x2[i]) > EPSILON)
            return false;
    }
    return true;
}

//幂法求最大特征值
void PowerIteration_MAX(double *a, int n, double *x0)
{
    double *limit_p = (double *)malloc(sizeof(double) * n); //X(2k)的极限
    bool if_p_limit = false;
    double *limit_q = (double *)malloc(sizeof(double) * n); //X(2k+1)的极限
    bool if_q_limit = false;

    double *x1 = (double *)malloc(sizeof(double) * n); //X(k)
    for (int i = 0; i < n; i++)                        //X(0)初始化
        x1[i] = x0[i];
    double *x2 = (double *)malloc(sizeof(double) * n); //X(K+1)
    double *y = (double *)malloc(sizeof(double) * n);  //Y(k)

    double *temp = (double *)malloc(sizeof(double) * n); //用于交换的临时变量
    for (int _ = 0; _ < N; _++)
    {
        printf("%2d  ", _);
        // 求||X(k)||
        int max_i = 0;
        double max = fabs(x1[0]);
        for (int i = 1; i < n; i++)
        {
            if (fabs(x1[i]) > max)
            {
                max_i = i;
                max = fabs(x1[i]);
            }
        }
        // Y(k) = X(k) / ||X(k)||
        for (int i = 0; i < n; i++)
        {
            y[i] = x1[i] / max;
            printf("% 10.6lf ", y[i]);
        }
        // X(k+1) = AY(k)
        printf("| ");
        for (int i = 0; i < n; i++)
        {
            x2[i] = 0.0;
            for (int j = 0; j < n; j++)
                x2[i] += y[j] * (*(a + i * n + j));
            printf("% 10.6lf ", x2[i]);
        }
        printf("\n");

        /*通过迭代序列的收敛性判断特征值及特征向量*/
        if (_ == 0)
        {
            memcpy(limit_p, x1, sizeof(double) * n);
            memcpy(limit_q, x2, sizeof(double) * n);
        }
        else if (_ % 2 == 0)
        {
            //判断偶序列是否收敛
            if (Convergence(limit_p, x1, n))
            {
                if_p_limit = true;
                //奇序列和偶序列均收敛，则分情况计算特征值
                if (if_q_limit)
                {
                    if (IfEqual(x1, x2, n))
                    {
                        printf("\nEigenvalue = %.4lf\n", fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else if (IfReverse(x1, x2, n))
                    {
                        printf("\nEigenvalue = -%.4lf\n", fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else
                    {
                        double *x3 = (double *)malloc(sizeof(double) * n);
                        printf("%2d  ", _ + 1);
                        for (int i = 0; i < n; i++)
                        {
                            x3[i] = 0.0;
                            for (int j = 0; j < n; j++)
                                x3[i] += x2[j] * (*(a + i * n + j));
                            printf("% 10.6lf ", x3[i]);
                        }
                        printf("\n");
                        double c = sqrt(x3[max_i] / y[max_i]);
                        printf("\nEigenvalue 1 = %.4lf\n", c);
                        printf("Eigenvector 1 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] + c * x2[i]);
                        printf("}\n");
                        printf("\nEigenvalue 2 = -%.4lf\n", c);
                        printf("Eigenvector 2 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] - c * x2[i]);
                        printf("}\n");
                    }
                    return;
                }
            }
            memcpy(limit_p, x1, sizeof(double) * n);
        }
        else
        {
            //判断奇序列是否收敛
            if (Convergence(limit_q, x1, n))
            {
                if_q_limit = true;
                //奇序列和偶序列均收敛，则分情况计算特征值
                if (if_p_limit)
                {
                    if (IfEqual(x1, x2, n))
                    {
                        printf("\nEigenvalue = %.4lf\n", fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else if (IfReverse(x1, x2, n))
                    {
                        printf("\nEigenvalue = -%.4lf\n", fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else
                    {
                        double *x3 = (double *)malloc(sizeof(double) * n);
                        printf("%2d  ", _ + 1);
                        for (int i = 0; i < n; i++)
                        {
                            x3[i] = 0.0;
                            for (int j = 0; j < n; j++)
                                x3[i] += x2[j] * (*(a + i * n + j));
                            printf("% 10.6lf ", x3[i]);
                        }
                        printf("\n");
                        double c = sqrt(x3[max_i] / y[max_i]);
                        printf("\nEigenvalue 1 = %.4lf\n", c);
                        printf("Eigenvector 1 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] + c * x2[i]);
                        printf("}\n");
                        printf("\nEigenvalue 2 = -%10.4lf\n", c);
                        printf("Eigenvector 2 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] - c * x2[i]);
                        printf("}\n");
                    }
                    return;
                }
            }
            memcpy(limit_q, x1, sizeof(double) * n);
        }

        memcpy(temp, x1, sizeof(double) * n);
        memcpy(x1, x2, sizeof(double) * n);
        memcpy(x2, temp, sizeof(double) * n);
    }

    free(temp);
    free(x1);
    free(x2);
    free(y);
    return;
}

//反幂法求最小特征值
void PowerIteration_MIN(double *a, int n, double *x0)
{
    /*对A作LU分解*/
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
    /*
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
    */

    //迭代求解
    double *limit_p = (double *)malloc(sizeof(double) * n); //X(2k)的极限
    bool if_p_limit = false;
    double *limit_q = (double *)malloc(sizeof(double) * n); //X(2k+1)的极限
    bool if_q_limit = false;

    double *x1 = (double *)malloc(sizeof(double) * n); //X(k)
    for (int i = 0; i < n; i++)                        //X(0)初始化
        x1[i] = x0[i];
    double *x2 = (double *)malloc(sizeof(double) * n); //X(K+1)
    double *y = (double *)malloc(sizeof(double) * n);  //Y(k)

    double *temp = (double *)malloc(sizeof(double) * n); //用于交换的临时变量
    for (int _ = 0; _ < N; _++)
    {
        printf("%2d  ", _);
        // 求||X(k)||
        int max_i = 0;
        double max = fabs(x1[0]);
        for (int i = 1; i < n; i++)
        {
            if (fabs(x1[i]) > max)
            {
                max_i = i;
                max = fabs(x1[i]);
            }
        }
        // Y(k) = X(k) / ||X(k)||
        for (int i = 0; i < n; i++)
        {
            y[i] = x1[i] / max;
            printf("% 10.6lf ", y[i]);
        }
        // 求解线性方程组LU*X(k+1) = Y(k)
        printf("| ");
        //解方程组Lz=b
        double *z = (double *)malloc(sizeof(double) * n);
        for (int i = 0; i < n; i++)
        {
            double temp = 0.0;
            for (int j = 0; j < i; j++)
                temp += (*(l + i * n + j)) * (*(z + j));
            *(z + i) = *(y + i) - temp;
        }
        //解方程组Ux=z
        for (int i = n - 1; i >= 0; i--)
        {
            double temp = 0.0;
            for (int j = i + 1; j < n; j++)
                temp += (*(u + i * n + j)) * (*(x2 + j));
            *(x2 + i) = (*(z + i) - temp) / (*(u + i * n + i));
        }
        //输出结果
        for (int i = 0; i < n; i++)
            printf("% 10.6lf ", x2[i]);
        free(z);
        printf("\n");

        /*通过迭代序列的收敛性判断特征值及特征向量*/
        if (_ == 0)
        {
            memcpy(limit_p, x1, sizeof(double) * n);
            memcpy(limit_q, x2, sizeof(double) * n);
        }
        else if (_ % 2 == 0)
        {
            //判断偶序列是否收敛
            if (Convergence(limit_p, x1, n))
            {
                if_p_limit = true;
                //奇序列和偶序列均收敛，则分情况计算特征值
                if (if_q_limit)
                {
                    if (IfEqual(x1, x2, n))
                    {
                        printf("\nEigenvalue = %.4lf\n", 1.0 / fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else if (IfReverse(x1, x2, n))
                    {
                        printf("\nEigenvalue = -%.4lf\n", 1.0 / fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else
                    {
                        double *x3 = (double *)malloc(sizeof(double) * n);
                        printf("%2d  ", _ + 1);
                        for (int i = 0; i < n; i++)
                        {
                            x3[i] = 0.0;
                            for (int j = 0; j < n; j++)
                                x3[i] += x2[j] * (*(a + i * n + j));
                            printf("% 10.6lf ", x3[i]);
                        }
                        printf("\n");
                        double c = sqrt(x3[max_i] / y[max_i]);
                        printf("\nEigenvalue 1 = %.4lf\n", 1.0 / c);
                        printf("Eigenvector 1 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] + c * x2[i]);
                        printf("}\n");
                        printf("\nEigenvalue 2 = -%.4lf\n", 1.0 / c);
                        printf("Eigenvector 2 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] - c * x2[i]);
                        printf("}\n");
                    }
                    return;
                }
            }
            memcpy(limit_p, x1, sizeof(double) * n);
        }
        else
        {
            //判断奇序列是否收敛
            if (Convergence(limit_q, x1, n))
            {
                if_q_limit = true;
                //奇序列和偶序列均收敛，则分情况计算特征值
                if (if_p_limit)
                {
                    if (IfEqual(x1, x2, n))
                    {
                        printf("\nEigenvalue = %.4lf\n", 1.0 / fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else if (IfReverse(x1, x2, n))
                    {
                        printf("\nEigenvalue = -%.4lf\n", 1.0 / fabs(x2[max_i]));
                        printf("Eigenvector = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", y[i]);
                        printf("}\n");
                    }
                    else
                    {
                        double *x3 = (double *)malloc(sizeof(double) * n);
                        printf("%2d  ", _ + 1);
                        for (int i = 0; i < n; i++)
                        {
                            x3[i] = 0.0;
                            for (int j = 0; j < n; j++)
                                x3[i] += x2[j] * (*(a + i * n + j));
                            printf("% 10.6lf ", x3[i]);
                        }
                        printf("\n");
                        double c = sqrt(x3[max_i] / y[max_i]);
                        printf("\nEigenvalue 1 = %.4lf\n", 1.0 / c);
                        printf("Eigenvector 1 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] + c * x2[i]);
                        printf("}\n");
                        printf("\nEigenvalue 2 = -%10.4lf\n", 1.0 / c);
                        printf("Eigenvector 2 = { ");
                        for (int i = 0; i < n; i++)
                            printf("%.4lf ", x3[i] - c * x2[i]);
                        printf("}\n");
                    }
                    return;
                }
            }
            memcpy(limit_q, x1, sizeof(double) * n);
        }

        memcpy(temp, x1, sizeof(double) * n);
        memcpy(x1, x2, sizeof(double) * n);
        memcpy(x2, temp, sizeof(double) * n);
    }

    free(temp);
    free(x1);
    free(x2);
    free(y);
    free(u);
    free(l);
    return;
}

int main()
{
    double a[2][2] = {{5.0, 3.0}, {-2.0, 0.0}};
    double x0[2] = {1.0, 1.0};
    PowerIteration_MIN((double *)a, 2, x0);
    return 0;
}
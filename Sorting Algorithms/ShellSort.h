/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "ShellSort.h"
* Function:  ShellSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int ShellSort(T *a, int n, int (*cmp)(const T, const T))
{
    /*******************************************/
    // 功能:    将数组a排序
    // 输入:    待排序列a
    //          数组长度n
    //          比较规则cmp
    // 输出：   排序过程中移动或比较元素次数
    //          排好序的数组a
    /*******************************************/
    long long int count = 0;
    int d = n / 2; //初始增量
    while (d >= 1)
    {
        for (int i = d; i < n; i++) //从第d+1个元素开始进行插入排序
        {
            T temp = a[i];
            int j;
            for (j = i - d; j >= 0; j = j - d)
            {
                count++;
                if (cmp(a[j], temp) > 0)
                    a[j + d] = a[j]; //向后移动
                else
                    break;
            }
            //插入
            a[j + d] = temp;
        }
        d = d / 2; //增量变为原来的一半
    }
    return count;
}
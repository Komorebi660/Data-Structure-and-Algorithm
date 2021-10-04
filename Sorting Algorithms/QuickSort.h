/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "QuickSort.h"
* Function:  QuickSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int QuickSort(T *a, int n, int (*cmp)(const T, const T))
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
    int i = 0, j = n - 1;
    T flag = a[0]; //初始基准为0
    while (i < j)
    {
        //从右往左开始，大于基准数的不动，否则与基准数对换
        while (i < j && (cmp(a[j], flag) > 0))
            j--;
        if (i < j)
        {
            T temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            i++;
            count += 1;
        }
        //从左往右开始，不大于基准数的不动，否则与基准数对换
        while (i < j && (cmp(a[i], flag) <= 0))
            i++;
        if (i < j)
        {
            T temp = a[j];
            a[j] = a[i];
            a[i] = temp;
            j--;
            count += 1;
        }
    }
    //对基准数左边的数重复上述操作
    if (i > 1)
        count += QuickSort(a, i, cmp);
    //对基准数右边的数重复上述操作
    if (n - i - 1 > 1)
        count += QuickSort(a + i + 1, n - i - 1, cmp);
    return count;
}
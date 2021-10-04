/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "InsertionSort.h"
* Function:  InsertionSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int InsertionSort(T *a, int n, int (*cmp)(const T, const T))
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
    T temp;
    for (int i = 1; i < n; i++)
    {
        temp = a[i];
        int j;
        for (j = i - 1; j >= 0; j--)
        {
            count++;
            if (cmp(a[j], temp) > 0)
                a[j + 1] = a[j]; //向后移动
            else
                break;
        }
        //插入
        a[j + 1] = temp;
    }
    return count;
}
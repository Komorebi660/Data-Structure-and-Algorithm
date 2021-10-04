/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "SelectionSort.h"
* Function:  SelectionSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int SelectionSort(T *a, int n, int (*cmp)(const T, const T))
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
    for (int i = 0; i < n; i++)
    {
        int k = i;
        for (int j = i; j < n; j++)
        {
            count++;
            if (cmp(a[j], a[k]) < 0)
                k = j;
        }
        T temp = a[i];
        a[i] = a[k];
        a[k] = temp;
    }
    return count;
}
/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "BInsertionSort.h"
* Function:  Binary InsertionSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int BInsertionSort(T *a, int n, int (*cmp)(const T, const T))
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
        int low = 0;
        int mid = i / 2;
        int high = i - 1;
        temp = a[i];
        //二分查找插入位置
        while (low <= high)
        {
            count++;
            mid = (low + high) / 2;
            if (cmp(a[mid], temp) > 0)
                high = mid - 1;
            else
                low = mid + 1;
        }
        //a[low...i-1]的元素统一后移
        for (int j = i; j > low; j--)
        {
            count++;
            a[j] = a[j - 1];
        }
        //插入元素
        a[low] = temp;
    }
    return count;
}
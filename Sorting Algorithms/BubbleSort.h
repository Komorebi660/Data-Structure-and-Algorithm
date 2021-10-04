/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: BbubbleSort.h"
* Function:  BubbleSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int BubbleSort(T *a, int n, int (*cmp)(const T, const T))
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
    int flag = 0;
    T temp;
    for (int i = 0; i < n - 1; i++)
    {
        flag = 0;
        for (int j = 0; j < n - 1 - i; j++)
        {
            count++;
            if (cmp(a[j], a[j + 1]) > 0)
            {
                temp = a[j + 1];
                a[j + 1] = a[j];
                a[j] = temp;
                flag = 1;
            }
        }
        //本次搜寻没有比较，说明数组已排好
        if (flag == 0)
            break;
    }
    return count;
}
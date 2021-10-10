/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "CountSort.h"
* Function:  CountSort Algorithm
*************************************************************/
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

void CountSort(int *a, int n, int max)
{
    /*******************************************/
    // 功能:    将数组a排序
    // 输入:    待排序列a
    //          数组长度n
    //          数组中元素的最大值max
    // 输出：    排好序的数组a
    /*******************************************/

    //辅助数组
    int *count_arr = (int *)malloc(sizeof(int) * (max + 1));
    int *sorted_arr = (int *)malloc(sizeof(int) * (n + 1));

    //清空！不能用memset()?
    for (int i = 0; i < max + 1; i++)
        count_arr[i] = 0;
    for (int i = 0; i < n + 1; i++)
        sorted_arr[i] = 0;

    //统计数值出现的次数
    for (int i = 0; i < n; i++)
        count_arr[a[i]] = count_arr[a[i]] + 1;
    //对所有的计数累加
    for (int i = 1; i < max; i++)
        count_arr[i] = count_arr[i] + count_arr[i - 1];
    //逆向遍历源数组（保证稳定性），根据计数数组中对应的值填充到sorted_arr中
    for (int i = n - 1; i >= 0; i--)
    {
        sorted_arr[count_arr[a[i]] - 1] = a[i];
        count_arr[a[i]]--;
    }
    free(count_arr);
    //将排好序数组拷贝
    memcpy(a, sorted_arr, sizeof(int) * n);
    free(sorted_arr);
    return;
}
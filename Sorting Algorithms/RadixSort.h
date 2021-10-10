/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* Function:  RadixSort Algorithm
*************************************************************/
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

void _CountSort(int *a, int len, int radix)
{
    /*******************************************/
    // 功能:    将数组a按计数radix排序
    // 输入:    待排序列a
    //          数组长度len
    //          本次排序所用基数radix
    // 输出：   排好序的数组a
    /*******************************************/

    //辅助数组
    int *count_arr = (int *)malloc(sizeof(int) * 10);
    int *sorted_arr = (int *)malloc(sizeof(int) * len);

    //清空
    for (int i = 0; i < 10; i++)
        count_arr[i] = 0;
    for (int i = 0; i < len; i++)
        sorted_arr[i] = 0;

    //统计出现的次数
    for (int i = 0; i < len; i++)
        count_arr[(a[i] / radix) % 10] += 1;
    //对所有的计数累加
    for (int i = 1; i < 10; i++)
        count_arr[i] = count_arr[i] + count_arr[i - 1];
    //逆向遍历源数组（保证稳定性），根据计数数组中对应的值填充到sorted_arr中
    for (int i = len - 1; i >= 0; i--)
    {
        sorted_arr[count_arr[(a[i] / radix) % 10] - 1] = a[i];
        count_arr[(a[i] / radix) % 10]--;
    }
    free(count_arr);
    //将排好序数组拷贝
    memcpy(a, sorted_arr, sizeof(int) * len);
    free(sorted_arr);
    return;
}

void RadixSort(int *a, int len)
{
    /*******************************************/
    // 功能:    将数组a排序
    // 输入:    待排序列a
    //          数组长度len
    // 输出：   排好序的数组a
    /*******************************************/
    //先求出最大值
    int max = -1;
    for (int i = 0; i < len; i++)
    {
        if (a[i] > max)
            max = a[i];
    }

    //对数组中元素的每一位进行计数排序
    for (int radix = 1; max / radix > 0; radix *= 10)
        _CountSort(a, len, radix);

    return;
}
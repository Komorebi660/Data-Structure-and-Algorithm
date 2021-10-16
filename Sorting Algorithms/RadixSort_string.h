/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* Function:  RadixSort Algorithm -- string version
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max_len = 0; //指示字符串的最大长度

int get_index(char *s, int index)
{
    /*******************************************/
    // 功能:    获取s[index]的值，超出部分返回0
    // 输入:    字符串指针s
    //          下标index
    // 输出：   s[index]
    /*******************************************/
    int n = strlen(s);
    int offset = max_len - index;
    if (n - offset < 0)
        return 0;
    else
        return s[n - offset] - 48;
}

void _CountSort(char **a, int len, int max, int index, int (*get)(char *s, int index))
{
    /*******************************************/
    // 功能:    将数组a按get()计数排序
    // 输入:    待排序列a
    //          数组长度len
    //          数据范围max
    //          本次排序所用下标index
    //          获取s[index]的函数指针
    // 输出：   按要求排好序的数组a
    /*******************************************/
    //辅助数组
    int *count_arr = (int *)malloc(sizeof(int) * max);
    char **sorted_arr = (char **)malloc(sizeof(char *) * len);

    //清空
    for (int i = 0; i < max; i++)
        count_arr[i] = 0;
    for (int i = 0; i < len; i++)
        sorted_arr[i] = NULL;

    //统计出现的次数
    for (int i = 0; i < len; i++)
        count_arr[get(a[i], index)] += 1;
    //对所有的计数累加
    for (int i = 1; i < max; i++)
        count_arr[i] = count_arr[i] + count_arr[i - 1];
    //逆向遍历源数组（保证稳定性），根据计数数组中对应的值填充到sorted_arr中
    for (int i = len - 1; i >= 0; i--)
    {
        sorted_arr[count_arr[get(a[i], index)] - 1] = a[i];
        count_arr[get(a[i], index)]--;
    }
    free(count_arr);
    //将排好序数组拷贝
    memcpy(a, sorted_arr, sizeof(char *) * len);
    free(sorted_arr);
    return;
}

void RadixSort(char **a, int len)
{
    /*******************************************/
    // 功能:    将数组a排序
    // 输入:    待排序列a
    //          数组长度len
    // 输出：   排好序的数组a
    /*******************************************/
    //先求出最大长度
    max_len = -1;
    for (int i = 0; i < len; i++)
    {
        int n = strlen(a[i]);
        if (n > max_len)
            max_len = n;
    }

    //对字符串中元素的每一位(从高到低)进行计数排序
    for (int index = max_len - 1; index >= 0; index--)
        _CountSort(a, len, 10, index, get_index);

    return;
}
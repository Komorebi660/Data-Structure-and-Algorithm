/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "MergeSort.h"
* Function:  MergeSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int Merge(T *a, int low, int mid, int high, int (*cmp)(const T, const T))
{
    /*******************************************/
    // 功能:    将a[low,mid]与a[mid+1,high]进行合并
    // 输入:    待排序列a
    //          数组开始low
    //          数组结束high
    //          数组分割位置mid
    //          比较规则cmp
    // 输出：   排序过程中移动或比较元素次数
    /*******************************************/
    long long int count = 0;
    T *temp = (T *)malloc((high - low + 1) * sizeof(T));
    //限定范围
    int left_low = low;
    int left_high = mid;
    int right_low = mid + 1;
    int right_high = high;

    //开始合并
    int k;
    for (k = 0; left_low <= left_high && right_low <= right_high; k++)
    { // 比较两个指针所指向的元素
        count++;
        if (cmp(a[left_low], a[right_low]) <= 0)
            temp[k] = a[left_low++];
        else
            temp[k] = a[right_low++];
    }
    //若第一个序列有剩余，直接复制出来合并到temp序列尾
    if (left_low <= left_high)
        memcpy(temp + k, a + left_low, (left_high - left_low + 1) * sizeof(T));
    //若第二个序列有剩余，直接复制出来合并到temp序列尾
    if (right_low <= right_high)
        memcpy(temp + k, a + right_low, (right_high - right_low + 1) * sizeof(T));
    //将结果复制到原始数组
    for (int i = 0; i < high - low + 1; i++)
        a[low + i] = temp[i];

    free(temp);
    return count;
}

template <typename T>
long long int MergeSort(T *a, int low, int high, int (*cmp)(const T, const T))
{
    /*******************************************/
    // 功能:    将数组a排序
    // 输入:    待排序列a
    //          数组起始下标low
    //          数组结束下标high
    //          比较规则cmp
    // 输出：   排序过程中移动或比较元素次数
    //          排好序的数组a
    /*******************************************/
    long long int count = 0;
    int mid = (low + high) / 2;
    //递归终止
    if (low == high)
        return 0;
    //对前半部分作归并排序
    count += MergeSort(a, low, mid, cmp);
    //对后半部分作归并排序
    count += MergeSort(a, mid + 1, high, cmp);
    //两部分归并
    count += Merge(a, low, mid, high, cmp);
    return count;
}
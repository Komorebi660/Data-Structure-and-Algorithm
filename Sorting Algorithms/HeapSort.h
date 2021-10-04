/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "HeapSort.h"
* Function:  HeapSort Algorithm
*************************************************************/
#include <iostream>
using namespace std;

template <typename T>
long long int HeapAdjust(T *a, int start, int end, int (*cmp)(const T, const T))
{
    /*******************************************/
    // 功能:    将a[start...end]调整为大顶堆
    // 输入:    待调整序列a
    //          开始节点下标start
    //          结束节点下标end
    //          比较规则cmp
    // 输出：   调整过程中移动或比较的次数
    /*******************************************/

    long long int count = 0;
    T temp = a[start];
    //从start节点开始调整堆，一直到叶子结点结束
    //由于起始元素下标为0，所以孩子节点要加1
    for (int i = 2 * start + 1; i <= end; i = i * 2 + 1)
    {
        count++;
        //找到值最大的孩子结点
        if ((i + 1 <= end) && (cmp(a[i], a[i + 1]) < 0))
            i++;
        //如果待调整结点比最大的孩子结点的值还大，则已找到正确位置，退出循环
        if ((cmp(temp, a[i]) >= 0))
            break;
        //如果当前结点的值比孩子结点中最大的值小，则将最大的值移至该结点
        a[start] = a[i];
        start = i; //指向其孩子结点，作为下次循环的双亲节点
    }
    a[start] = temp; //添加到正确的位置
    return count;
}

template <typename T>
long long int HeapSort(T *a, int n, int (*cmp)(const T, const T))
{
    /*******************************************/
    // 功能:    将数组a排序
    // 输入:    待排序列a
    //          数组长度n
    //          比较规则cmp
    // 输出：   排序过程中移动或比较的次数
    //          排好序的数组a
    /*******************************************/
    long long int count = 0;
    //构建大顶堆,由于数组下标从0开始，所以要减一
    if (n == 1)
        return 0;
    for (int i = (n / 2) - 1; i >= 0; i--)
    {
        //对于有孩子结点的根结点开始调整
        count += HeapAdjust(a, i, n - 1, cmp);
    }
    //不断从堆顶取出元素，与数组末尾元素交换，然后调整
    for (int i = n - 1; i > 0; i--)
    {
        T temp = a[i];
        a[i] = a[0];
        a[0] = temp;

        count += HeapAdjust(a, 0, i - 1, cmp);
    }
    return count;
}

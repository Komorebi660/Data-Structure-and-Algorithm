/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "Select.h"
 * Function:  Select the i-th number in array
 *************************************************************/
#include <iostream>
#include <time.h>
using namespace std;

#define MAX_LEN 10
#define MAX_NUM 100

//自定义比较函数
int cmp(const int a, const int b)
{
    return a - b;
}

template <typename T>
T Select(T *a, int n, int tag, int (*cmp)(const T, const T))
{
    /*******************************************/
    // 功能:    找到数组a中第tag大的数
    // 输入:    待搜索序列a
    //          数组长度n
    //          查找位置tag
    //          比较规则cmp
    // 输出：   查找结果
    /*******************************************/
    //输入参数错误
    if (tag > n)
        return -1;

    int i = 0, j = n - 1;
    T flag = a[0]; //初始基准为a[0]
    //找到基准元素所在位置i(以0开始)
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
        }
    }
    //如果目标等于i，则已找到
    if (tag - 1 == i)
        return a[i];
    //如果目标位置小于i，对左半边继续递归
    else if (tag - 1 < i)
        return Select(a, i, tag, cmp);
    //如果目标位置大于i，对右半边继续递归，注意此时是找第tag-i-1位元素
    else
        return Select(a + i + 1, n - i - 1, tag - i - 1, cmp);
}

int main()
{
    int unsort[MAX_LEN] = {0};
    int sorted[MAX_LEN] = {0};
    int test[MAX_LEN] = {0};
    srand(time(NULL));
    //数组初始化(生成随机数)
    for (int i = 0; i < MAX_LEN; i++)
    {
        //rand()函数的范围为0~32767(即RAND_MAX)
        //要生成比它大的数应先生成0~1的随机浮点数，再乘以你想要的数的最大值
        unsort[i] = rand() / (RAND_MAX + 0.0) * MAX_NUM;
        test[i] = unsort[i];
    }

    for (int i = 1; i <= MAX_LEN; i++)
        sorted[i - 1] = Select(test, MAX_LEN, i, cmp);

    printf("Unsort\t\tSelected\n");
    for (int i = 0; i < MAX_LEN; i++)
        printf("%-10d  %-10d\n", unsort[i], sorted[i]);

    return 0;
}
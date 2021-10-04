/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "sort_test.cpp"
* Function:  Test the correction of all of the sorting algorithms
*************************************************************/
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BInsertionSort.h"
#include "InsertionSort.h"
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "ShellSort.h"
#include "HeapSort.h"
#include "CountSort.h"

#define MAX_LEN 1000
#define MAX_NUM 10000

struct Test
{
    int a;
    char s[100];
} t[8][MAX_LEN] = {0};
long long int count[8] = {0};

//自定义比较函数
int cmp(const struct Test a, const struct Test b)
{
    return a.a - b.a;
}

//测试通用排序算法
void test_general_sort()
{
    int temp[MAX_LEN] = {0};
    srand(time(NULL));
    //数组初始化(生成随机数)
    for (int i = 0; i < MAX_LEN; i++)
    {
        //rand()函数的范围为0~32767(即RAND_MAX)
        //要生成比它大的数应先生成0~1的随机浮点数，再乘以你想要的数的最大值
        temp[i] = rand() / (RAND_MAX + 0.0) * MAX_NUM;
        for (int j = 0; j < 8; j++)
            t[j][i].a = temp[i];
    }

    //调用排序算法
    count[0] = BInsertionSort<struct Test>(t[0], MAX_LEN, cmp);
    count[1] = InsertionSort<struct Test>(t[1], MAX_LEN, cmp);
    count[2] = BubbleSort<struct Test>(t[2], MAX_LEN, cmp);
    count[3] = SelectionSort<struct Test>(t[3], MAX_LEN, cmp);
    count[4] = MergeSort<struct Test>(t[4], 0, MAX_LEN - 1, cmp);
    count[5] = QuickSort<struct Test>(t[5], MAX_LEN, cmp);
    count[6] = ShellSort<struct Test>(t[6], MAX_LEN, cmp);
    count[7] = HeapSort<struct Test>(t[7], MAX_LEN, cmp);

    //输出结果
    printf("Before Sort\t\tBInSSort\t\tInsSort\t\t\tBubbleSort\t\tSelSort");
    printf("\t\t\tMergeSort\t\tQuickSort\t\tShellSort\t\tHeapSort\n");
    for (int i = 0; i < MAX_LEN; i++)
        printf("%-15d %-15d %-15d %-15d %-15d %-15d %-15d %-15d %-15d\n",
               temp[i], t[0][i].a, t[1][i].a, t[2][i].a, t[3][i].a, t[4][i].a, t[5][i].a, t[6][i].a, t[7][i].a);
    printf("total count:    %-15ld %-15ld %-15ld %-15ld %-15ld %-15ld %-15ld %-15ld\n",
           count[0], count[1], count[2], count[3], count[4], count[5], count[6], count[7]);
    return;
}

//测试计数排序
void test_count_sort()
{
    int temp[MAX_LEN] = {0};
    int array[MAX_LEN] = {0};
    long long int count_ = 0;
    srand(time(NULL));
    //数组初始化(生成随机数)
    for (int i = 0; i < MAX_LEN; i++)
    {
        //rand()函数的范围为0~32767(即RAND_MAX)
        //要生成比它大的数应先生成0~1的随机浮点数，再乘以你想要的数的最大值
        temp[i] = rand() / (RAND_MAX + 0.0) * MAX_NUM;
        array[i] = temp[i];
    }
    count_ = CountSort(array, MAX_LEN, MAX_NUM);
    printf("Before Sort\t\tCountSort\n");
    for (int i = 0; i < MAX_LEN; i++)
        printf("%-15d %-15d\n", temp[i], array[i]);
    printf("total count: %ld\n", count_);
    return;
}

int main()
{
    //test_count_sort();
    test_general_sort();
    return 0;
}
/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "HuffmanTree_Test.cpp"
* Function:  Test the correction of "HuffmanTree.h"
*************************************************************/
#include <iostream>
#include <time.h>
#include "HuffmanTree.h"

#define MAX_LEN 100

int main()
{
    int weight[MAX_LEN] = {0};
    srand(time(NULL));
    //数组初始化(生成随机数)
    for (int i = 0; i < MAX_LEN; i++)
        weight[i] = rand() / (RAND_MAX + 0.0) * (MAX_LEN * 10);

    HuffmanTree tree;
    tree.CreatHuffmanTree(weight, MAX_LEN);
    tree.GenHuffmanCode();
    tree.PrintTree();
    tree.PrintCode();
}
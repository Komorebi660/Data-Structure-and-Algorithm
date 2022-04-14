/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "TestDataGen.cpp"
 * Function:  Generate test data
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <ctime>
using namespace std;

#define MAX 80 //移动步数(近似问题的困难程度)

vector<vector<int>> start;  //初始状态
vector<vector<int>> target; //目标状态

int main()
{
    srand((int)time(NULL));
    //生成初始状态
    vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    FILE *fp;
    fp = fopen("./input.txt", "w");
    if (fp == NULL)
    {
        printf("Create input file failed!\n");
        exit(-1);
    }
    for (int i = 0; i < 5; i++)
    {
        vector<int> tmp;
        for (int j = 0; j < 5; j++)
        {
            int index = rand() % data.size();
            fprintf(fp, "%d ", data[index]);
            tmp.push_back(data[index]);
            data.erase(data.begin() + index);
        }
        start.push_back(tmp);
        fprintf(fp, "\n");
    }
    fclose(fp);

    //查找空格位置
    bool find = false;
    int x = 0, y = 0;
    for (x = 0; x < start.size(); x++)
    {
        for (y = 0; y < start[x].size(); y++)
        {
            if (start[x][y] == 0)
            {
                find = true;
                break;
            }
        }
        if (find)
            break;
    }

    //模拟移动, 生成目标状态
    target.assign(start.begin(), start.end());
    for (int _ = 0; _ < MAX; _++)
    {
        int op = rand() % 4;
        // U
        if (op == 0)
        {
            if (x - 1 >= 0)
            {
                target[x][y] = target[x - 1][y];
                target[x - 1][y] = 0;
                x = x - 1;
                y = y;
            }
        }
        // D
        else if (op == 1)
        {
            if (x + 1 < 5)
            {
                target[x][y] = target[x + 1][y];
                target[x + 1][y] = 0;
                x = x + 1;
                y = y;
            }
        }
        // R
        else if (op == 2)
        {
            if (y + 1 < 5)
            {
                target[x][y] = target[x][y + 1];
                target[x][y + 1] = 0;
                x = x;
                y = y + 1;
            }
        }
        // L
        else if (op == 3)
        {
            if (y - 1 >= 0)
            {
                target[x][y] = target[x][y - 1];
                target[x][y - 1] = 0;
                x = x;
                y = y - 1;
            }
        }
    }

    //写入文件
    fp = fopen("./target.txt", "w");
    if (fp == NULL)
    {
        printf("Create target file failed!\n");
        exit(-1);
    }
    for (int i = 0; i < target.size(); i++)
    {
        for (int j = 0; j < target[i].size(); j++)
        {
            fprintf(fp, "%d ", target[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    printf("Data Generation Done!\n");

    return 0;
}

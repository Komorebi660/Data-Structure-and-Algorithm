/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "CheckAnswer.cpp"
 * Function:  Check the correction of A* Search
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;

vector<vector<int>> start;  //初始状态
vector<vector<int>> target; //目标状态

int main()
{
    printf("Checking Answer...\n");
    /*读入数据*/
    FILE *fp;
    fp = fopen("./output.txt", "r");
    if (fp == NULL)
    {
        printf("Open output file failed!\n");
        exit(-1);
    }
    char op[100] = {0};
    fscanf(fp, "%s", op);
    int n = strlen(op);
    fclose(fp);

    fp = fopen("./input.txt", "r");
    if (fp == NULL)
    {
        printf("Open input file failed!\n");
        exit(-1);
    }
    for (int i = 0; i < 5; i++)
    {
        vector<int> tmp;
        for (int j = 0; j < 5; j++)
        {
            int tmp_d;
            fscanf(fp, "%d", &tmp_d);
            tmp.push_back(tmp_d);
        }
        start.push_back(tmp);
    }
    fclose(fp);

    fp = fopen("./target.txt", "r");
    if (fp == NULL)
    {
        printf("Open target file failed!\n");
        exit(-1);
    }
    for (int i = 0; i < 5; i++)
    {
        vector<int> tmp;
        for (int j = 0; j < 5; j++)
        {
            int tmp_d;
            fscanf(fp, "%d", &tmp_d);
            tmp.push_back(tmp_d);
        }
        target.push_back(tmp);
    }
    fclose(fp);

    //查找空格位置
    bool find = false;
    int x = 0, y = 0;
    for (x = 0; x < start.size(); x++)
    {
        for (y = 0; y < start[x].size(); y++)
        {
            //寻找宇宙飞船的位置
            if (start[x][y] == 0)
            {
                find = true;
                break;
            }
        }
        if (find)
            break;
    }

    for (int i = 0; i < n; i++)
    {
        if (op[i] == 'U')
        {
            if (x - 1 >= 0)
            {
                start[x][y] = start[x - 1][y];
                start[x - 1][y] = 0;
                x = x - 1;
                y = y;
            }
            else
                printf("Try to enter broader in step %d!\n", i + 1);
        }
        else if (op[i] == 'D')
        {
            if (x + 1 < 5)
            {
                start[x][y] = start[x + 1][y];
                start[x + 1][y] = 0;
                x = x + 1;
                y = y;
            }
            else
                printf("Try to enter broader in step %d!\n", i + 1);
        }
        else if (op[i] == 'R')
        {
            if (y + 1 < 5)
            {
                start[x][y] = start[x][y + 1];
                start[x][y + 1] = 0;
                x = x;
                y = y + 1;
            }
            else
                printf("Try to enter broader in step %d!\n", i + 1);
        }
        else if (op[i] == 'L')
        {
            if (y - 1 >= 0)
            {
                start[x][y] = start[x][y - 1];
                start[x][y - 1] = 0;
                x = x;
                y = y - 1;
            }
            else
                printf("Try to enter broader in step %d!\n", i + 1);
        }
        /*
        printf("step %d:\n", i + 1);
        for (int _i = 0; _i < start.size(); _i++)
        {
            for (int _j = 0; _j < start[_i].size(); _j++)
            {
                printf("%d ", start[_i][_j]);
            }
            printf("\n");
        }
        printf("\n");
        */
    }

    if (start == target)
        printf("Pass!\n");
    else
        printf("Failed!\n");
    return 0;
}

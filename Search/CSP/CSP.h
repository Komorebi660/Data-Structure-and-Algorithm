/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "CSP.h"
 * Function:  Head file for CSP
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <queue>
using namespace std;

class CSP
{
public:
    int total_worker;             //总的工人数
    int least_worker;             //每天的最少工人数
    int *senior;                  //存放哪些工人是高级工人
    vector<vector<int>> conflict; //标记不想在同一天值班的工人
    int *schedule;                //排班表
    int *assigned;                //是否已经赋值

    CSP(int _total_worker, int _least_worker, int *_senior, vector<vector<int>> &_conflict)
    {
        /*   初始化CSP问题
         *   total_worker:  总的工人数
         *   least_worker:  每天的最少工人数
         *   senior:        存放哪些工人是高级工人
         *   conflict:      标记不想在一天值班的工人, 共有三组
         */
        this->total_worker = _total_worker;
        this->least_worker = _least_worker;
        this->senior = _senior;
        this->conflict.assign(_conflict.begin(), _conflict.end());

        this->schedule = (int *)malloc(sizeof(int) * _total_worker * 7);
        this->assigned = (int *)malloc(sizeof(int) * _total_worker * 7);
        memset(this->schedule, 0, sizeof(int) * _total_worker * 7);
        memset(this->assigned, 0, sizeof(int) * _total_worker * 7);
    }
    void CSPSolver();

private:
    bool Recursive_Search(int *remain_value);
    bool Check_Conflict(int i, int j, int *remain_value);
};
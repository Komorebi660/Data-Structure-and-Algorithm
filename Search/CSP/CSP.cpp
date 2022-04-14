/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "CSP.cpp"
 * Function:  Using backtracking algorithm to solve CSP
 *************************************************************/
#include "CSP.h"
using namespace std;

void CSP::CSPSolver()
{
    //变量的剩余赋值 remain_value[ij][0]、remain_value[ij][1]
    int *remain_value = (int *)malloc(sizeof(int) * this->total_worker * 7 * 2);
    for (int i = 0; i < this->total_worker * 7; i++)
    {
        *(remain_value + i * 2 + 0) = 1;
        *(remain_value + i * 2 + 1) = 1;
    }

    //递归求解CSP
    if (Recursive_Search(remain_value))
    {
        for (int j = 0; j < 7; j++)
        {
            for (int i = 0; i < this->total_worker; i++)
            {
                if (*(this->schedule + i * 7 + j) == 1)
                    printf("%d ", i + 1);
            }
            printf("\n");
        }
    }
    free(remain_value);
    return;
}

bool CSP::Recursive_Search(int *remain_value)
{
    //检查是否所有变量均已赋值
    bool finish = true;
    int i = 0, j = 0, min = 10; //依据MRV选取的变量
    for (int _i = 0; _i < this->total_worker; _i++)
    {
        for (int _j = 0; _j < 7; _j++)
        {
            if (*(this->assigned + _i * 7 + _j) == 0)
            {
                finish = false;
                //计算该点的剩余值
                int remain = *(remain_value + _i * 14 + _j * 2 + 0) + *(remain_value + _i * 14 + _j * 2 + 1);
                if (remain < min)
                {
                    min = remain;
                    i = _i;
                    j = _j;
                }
            }
        }
    }
    //若所有变量均已赋值, 则已经找到解
    if (finish)
        return true;

    ///////* 赋值 *///////
    *(this->assigned + i * 7 + j) = 1;
    int *curr_remain_value = (int *)malloc(sizeof(int) * (this->total_worker) * 7 * 2);
    //如果0可以取, 选取0进行试探
    if (*(remain_value + i * 14 + j * 2 + 0) == 1)
    {
        *(this->schedule + i * 7 + j) = 0;
        memcpy(curr_remain_value, remain_value, sizeof(int) * (this->total_worker) * 7 * 2);
        *(curr_remain_value + i * 14 + j * 2 + 1) = 0;

        if (!Check_Conflict(i, j, curr_remain_value) && Recursive_Search(curr_remain_value))
            return true;
    }
    //如果1可以取, 选取1试探
    if (*(remain_value + i * 14 + j * 2 + 1) == 1)
    {
        *(this->schedule + i * 7 + j) = 1;
        memcpy(curr_remain_value, remain_value, sizeof(int) * this->total_worker * 7 * 2);
        *(curr_remain_value + i * 14 + j * 2 + 0) = 0;

        if (!Check_Conflict(i, j, curr_remain_value) && Recursive_Search(curr_remain_value))
            return true;
    }
    //没有满足约束的赋值, 返回false
    *(this->assigned + i * 7 + j) = 0;
    *(this->schedule + i * 7 + j) = 0;
    free(curr_remain_value);
    return false;
}

//检查冲突并更新remain_value
bool CSP::Check_Conflict(int x, int y, int *remain_value)
{
    //每个工人每周必须休息2天或2天以上
    int worked = 0; //该工人一周确定值班的天数
    for (int j = 0; j < 7; j++)
        worked += (*(this->schedule + x * 7 + j)) * (*(this->assigned + x * 7 + j));
    if (worked == 5)
    {
        //其它未赋值的必须为0
        for (int j = 0; j < 7; j++)
            if (*(this->assigned + x * 7 + j) == 0)
                *(remain_value + x * 14 + j * 2 + 1) = 0;
    }
    if (worked > 5)
        return true;

    //每个工人每周不可以连续休息3天
    if (y - 2 >= 0 &&
        *(this->assigned + x * 7 + y - 2) == 1 &&
        *(this->schedule + x * 7 + y - 2) == 0 &&
        *(this->assigned + x * 7 + y - 1) == 1 &&
        *(this->schedule + x * 7 + y - 1) == 0 &&
        *(this->schedule + x * 7 + y) == 0)
        return true;
    if (y - 1 >= 0 && y + 1 <= 6 &&
        *(this->assigned + x * 7 + y - 1) == 1 &&
        *(this->schedule + x * 7 + y - 1) == 0 &&
        *(this->schedule + x * 7 + y) == 0 &&
        *(this->assigned + x * 7 + y + 1) == 1 &&
        *(this->schedule + x * 7 + y + 1) == 0)
        return true;
    if (y + 2 <= 6 &&
        *(this->schedule + x * 7 + y) == 0 &&
        *(this->assigned + x * 7 + y + 1) == 1 &&
        *(this->schedule + x * 7 + y + 1) == 0 &&
        *(this->assigned + x * 7 + y + 2) == 1 &&
        *(this->schedule + x * 7 + y + 2) == 0)
        return true;
    // y-1, y休息, y-2还未赋值
    if (y - 2 >= 0 &&
        *(this->assigned + x * 7 + y - 2) == 0 &&
        *(this->assigned + x * 7 + y - 1) == 1 &&
        *(this->schedule + x * 7 + y - 1) == 0 &&
        *(this->schedule + x * 7 + y) == 0)
        *(remain_value + x * 14 + (y - 2) * 2 + 0) = 0;
    // y-2, y休息, y-1还未赋值
    if (y - 2 >= 0 &&
        *(this->assigned + x * 7 + y - 1) == 0 &&
        *(this->assigned + x * 7 + y - 2) == 1 &&
        *(this->schedule + x * 7 + y - 2) == 0 &&
        *(this->schedule + x * 7 + y) == 0)
        *(remain_value + x * 14 + (y - 1) * 2 + 0) = 0;
    // y-1, y休息, y+1还未赋值
    if (y - 1 >= 0 && y + 1 <= 6 &&
        *(this->assigned + x * 7 + y - 1) == 1 &&
        *(this->schedule + x * 7 + y - 1) == 0 &&
        *(this->schedule + x * 7 + y) == 0 &&
        *(this->assigned + x * 7 + y + 1) == 0)
        *(remain_value + x * 14 + (y + 1) * 2 + 0) = 0;
    // y+1, y休息, y-1还未赋值
    if (y - 1 >= 0 && y + 1 <= 6 &&
        *(this->assigned + x * 7 + y + 1) == 1 &&
        *(this->schedule + x * 7 + y + 1) == 0 &&
        *(this->schedule + x * 7 + y) == 0 &&
        *(this->assigned + x * 7 + y - 1) == 0)
        *(remain_value + x * 14 + (y - 1) * 2 + 0) = 0;
    // y+1, y休息, y+2还未赋值
    if (y + 2 <= 6 &&
        *(this->schedule + x * 7 + y) == 0 &&
        *(this->assigned + x * 7 + y + 1) == 1 &&
        *(this->schedule + x * 7 + y + 1) == 0 &&
        *(this->assigned + x * 7 + y + 2) == 0)
        *(remain_value + x * 14 + (y + 2) * 2 + 0) = 0;
    // y+2, y休息, y+1还未赋值
    if (y + 2 <= 6 &&
        *(this->schedule + x * 7 + y) == 0 &&
        *(this->assigned + x * 7 + y + 2) == 1 &&
        *(this->schedule + x * 7 + y + 2) == 0 &&
        *(this->assigned + x * 7 + y + 1) == 0)
        *(remain_value + x * 14 + (y + 1) * 2 + 0) = 0;

    //每天至少要有least_worker值班
    int assigned_num = 0; //已经赋值的人数
    int worked_num = 0;   //确定值班的人数
    for (int i = 0; i < this->total_worker; i++)
        if (*(this->assigned + i * 7 + y) == 1)
        {
            assigned_num++;
            if (*(this->schedule + i * 7 + y) == 1)
                worked_num++;
        }
    //如果已经确定值班的人数+还未安排的人数 < 最小人数, 则发生冲突
    if (worked_num + (this->total_worker - assigned_num) < this->least_worker)
        return true;
    //如果已经确定值班的人数+还未安排的人数 = 最小人数, 则未安排工作的人必须值班
    else if (worked_num + (this->total_worker - assigned_num) == this->least_worker)
    {
        for (int i = 0; i < this->total_worker; i++)
            if (*(this->assigned + i * 7 + y) == 0)
                *(remain_value + i * 14 + y * 2 + 0) = 0;
    }

    //每天至少要有一名级别为senior的工人值班
    if (this->senior[x] == 1 && *(this->schedule + x * 7 + y) == 0)
    {
        int senior_assigned_worked = 0; //是senior且确定要值班
        int senior_not_assigned = 0;    //是senior但未安排
        for (int i = 0; i < this->total_worker; i++)
            //是senior且被安排值班
            if (this->senior[i] == 1 && *(this->assigned + i * 7 + y) == 1 && *(this->schedule + i * 7 + y) == 1)
                senior_assigned_worked++;
            //是senior但还未安排
            else if (this->senior[i] == 1 && *(this->assigned + i * 7 + y) == 0)
                senior_not_assigned++;
        //没有senior值班, 则发生冲突
        if (senior_assigned_worked == 0 && senior_not_assigned == 0)
            return true;
        //只有一个senior未被安排, 且其余senior均没有值班, 则他一定要值班
        else if (senior_assigned_worked == 0 && senior_not_assigned == 1)
        {
            for (int i = 0; i < this->total_worker; i++)
                if (this->senior[i] == 1 && *(this->assigned + i * 7 + y) == 0)
                {
                    *(remain_value + i * 14 + y * 2 + 0) = 0;
                    break;
                }
        }
    }

    //工人之间的矛盾检查
    for (int i = 0; i < this->conflict.size(); i++)
    {
        if (this->conflict[i][x] == 1 && *(this->schedule + x * 7 + y) == 1)
        {
            int j = 0; //冲突的员工
            for (j = 0; j < this->total_worker; j++)
                if (j != x && this->conflict[i][j] == 1)
                    break;
            //如果他也在这一天值班, 则发生冲突
            if (*(this->assigned + j * 7 + y) == 1 && *(this->schedule + j * 7 + y) == 1)
                return true;
            //如果他在这一天没有被安排, 则一定不能值班
            else if (*(this->assigned + j * 7 + y) == 0)
                *(remain_value + j * 14 + y * 2 + 1) = 0;
        }
    }

    //检查是否存在可用值
    for (int i = 0; i < this->total_worker; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (*(remain_value + i * 14 + j * 2 + 0) + *(remain_value + i * 14 + j * 2 + 1) == 0)
                return true;
        }
    }
    return false;
}

int main()
{
    int senior[10] = {1, 0, 0, 0, 0, 0, 0, 1, 0, 1};
    vector<int> c1 = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0};
    vector<int> c2 = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    vector<int> c3 = {0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
    vector<vector<int>> conflict = {c1, c2, c3};
    CSP csp(10, 5, senior, conflict);

    double start = clock();
    csp.CSPSolver();
    double end = clock();
    printf("Time cost = %.6lfs\n", (end - start) / CLOCKS_PER_SEC);
}

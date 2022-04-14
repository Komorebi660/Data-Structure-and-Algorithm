/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "MinMaxSearch.h"
 * Function:  Head file for Min-Max Search
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
using namespace std;

#define MAXN 15      //棋局大小 MAXN * MAXN
#define MAX_DEPTH 25 //搜索的最大深度

struct Action
{
    int x, y;  //这一步选择的位置
    int value; //这一步的效用
};

class AI
{
public:
    void Play();

private:
    int depth; //记录搜索深度
    struct Action MIN_Value(vector<vector<int>> state, int alpha, int beta);
    struct Action MAX_Value(vector<vector<int>> state, int alpha, int beta);
    int h(vector<vector<int>> state);
    bool Check_Winner(vector<vector<int>> state, int player);
    bool Terminal_Test(vector<vector<int>> state);
} ai;

int f(vector<vector<int>> state, int x, int y, int player); //辅助启发式函数计算棋局评估值

/* 启发式函数计算棋局的value */
int AI::h(vector<vector<int>> state)
{
    int evaluation = 0;
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            if (state[i][j] == 1)
                evaluation -= f(state, i, j, 1); //对手减
            else if (state[i][j] == 2)
                evaluation += f(state, i, j, 2); //自己加
        }
    }
    return evaluation;
}

struct Action AI::MAX_Value(vector<vector<int>> state, int alpha, int beta)
{
    //是否到达终止节点
    if (Terminal_Test(state))
    {
        struct Action tmp = {-1, -1, h(state)};
        return tmp;
    }

    depth++; //搜索深度加1
    int a = alpha;
    struct Action op = {-1, -1, INT_MIN};
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            //可以下在(i, j)处
            if (state[i][j] == 0)
            {
                state[i][j] = 2;
                struct Action tmp = MIN_Value(state, a, beta);
                if (tmp.value > op.value)
                {
                    op.value = tmp.value;
                    op.x = i;
                    op.y = j;
                }
                // beta剪枝
                if (op.value >= beta)
                    return op;
                //更新alpha
                a = max(op.value, a);
                state[i][j] = 0; //注意恢复
            }
        }
    }
    return op;
}

struct Action AI::MIN_Value(vector<vector<int>> state, int alpha, int beta)
{
    //是否到达终止节点
    if (Terminal_Test(state))
    {
        struct Action tmp = {-1, -1, h(state)};
        return tmp;
    }

    int b = beta;
    struct Action op = {-1, -1, INT_MAX};
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            //可以下在(i, j)处
            if (state[i][j] == 0)
            {
                state[i][j] = 1;
                struct Action tmp = MAX_Value(state, alpha, b);
                if (tmp.value < op.value)
                {
                    op.value = tmp.value;
                    op.x = i;
                    op.y = j;
                }
                // alpha剪枝
                if (op.value <= alpha)
                    return op;
                //更新beta
                b = min(op.value, b);
                state[i][j] = 0; //注意恢复
            }
        }
    }
    return op;
}

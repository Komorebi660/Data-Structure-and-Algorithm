/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "MinMaxSearch.cpp"
 * Function:  Min-Max Search with Alpha-Beta pruning
 *************************************************************/
/* Min-MAx搜索 + Alpha-Beta剪枝 五子棋AI*/
#include "MinMaxSearch.h"
using namespace std;

/* 检查player是否获胜 */
bool AI::Check_Winner(vector<vector<int>> state, int player)
{
    //检查Player的横向
    for (int i = 0; i < MAXN; i++)
    {
        int count = 0;
        for (int j = 0; j < MAXN; j++)
        {
            if (state[i][j] == player)
                count++;
            else
                count = 0;
            //连续出现5次
            if (count == 5)
                return true;
        }
    }
    //检查Player的纵向
    for (int j = 0; j < MAXN; j++)
    {
        int count = 0;
        for (int i = 0; i < MAXN; i++)
        {
            if (state[i][j] == player)
                count++;
            else
                count = 0;
            //连续出现5次
            if (count == 5)
                return true;
        }
    }
    //检查Player的对角线
    int count[MAXN][MAXN];
    memset(count, 0, sizeof(int) * MAXN * MAXN);
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            if (state[i][j] == player)
            {
                if (i - 1 >= 0 && j - 1 >= 0)
                    count[i][j] = count[i - 1][j - 1] + 1;
                else
                    count[i][j] = 1;
            }
            else
                count[i][j] = 0;
            //连续出现5次
            if (count[i][j] == 5)
                return true;
        }
    }
    //检查Player的反对角线
    memset(count, 0, sizeof(int) * MAXN * MAXN);
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = MAXN - 1; j >= 0; j--)
        {
            if (state[i][j] == player)
            {
                if (i - 1 >= 0 && j + 1 < MAXN)
                    count[i][j] = count[i - 1][j + 1] + 1;
                else
                    count[i][j] = 1;
            }
            else
                count[i][j] = 0;
            //连续出现5次
            if (count[i][j] == 5)
                return true;
        }
    }

    return false;
}

/* 检验是否达到搜索终点 */
bool AI::Terminal_Test(vector<vector<int>> state)
{
    //首先检查是否没有空余位
    bool no_free = true;
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            if (state[i][j] == 0)
                no_free = false;
        }
        if (!no_free)
            break;
    }
    if (no_free)
        return true;
    //然后检查是否到达迭代深度
    if (depth >= MAX_DEPTH)
        return true;
    //最后检查是否有赢家
    if (Check_Winner(state, 1) || Check_Winner(state, 2))
        return true;

    return false;
}

/*辅助启发式函数计算棋局评估值*/
int f(vector<vector<int>> state, int x, int y, int player)
{
    /*
     *   state: 棋局状态
     *   x, y: 这枚棋子的位置
     *   player: 这枚棋子属于哪个玩家
     *
     *   输出这枚棋子的效用
     */
    int value = 0;
    int myself = player;               //自己
    int enemy = (myself == 1) ? 2 : 1; //对手
    //以该点为中心计算四个方向长度为9的字串
    vector<string> s;
    s.resize(4);
    for (int i = -4; i <= 4; i++)
    {
        //纵向
        if (x + i >= 0 && x + i < MAXN)
        {
            if (state[x + i][y] == myself)
                s[0] += "1";
            else if (state[x + i][y] == enemy)
                s[0] += "0";
            else
                s[0] += "-";
        }
        //横向
        if (y + i >= 0 && y + i < MAXN)
        {
            if (state[x][y + i] == myself)
                s[1] += "1";
            else if (state[x][y + i] == enemy)
                s[1] += "0";
            else
                s[1] += "-";
        }
        //对角线
        if (x + i >= 0 && x + i < MAXN && y + i >= 0 && y + i < MAXN)
        {
            if (state[x + i][y + i] == myself)
                s[2] += "1";
            else if (state[x + i][y + i] == enemy)
                s[2] += "0";
            else
                s[2] += "-";
        }
        //反对角线
        if (x + i >= 0 && x + i < MAXN && y - i >= 0 && y - i < MAXN)
        {
            if (state[x + i][y - i] == myself)
                s[3] += "1";
            else if (state[x + i][y - i] == enemy)
                s[3] += "0";
            else
                s[3] += "-";
        }
    }

    //匹配模式
    for (int k = 0; k < 4; k++)
    {
        if (s[k].find("11111") != string::npos) //成五
            value += 5000000;
        if (s[k].find("-1111-") != string::npos) //活四
            value += 100000;
        if (s[k].find("-11110") != string::npos ||
            s[k].find("01111-") != string::npos ||
            s[k].find("01-111") != string::npos ||
            s[k].find("1-111") != string::npos ||
            s[k].find("111-1") != string::npos ||
            s[k].find("11-11") != string::npos) //冲四
            value += 10000;
        if (s[k].find("-111-") != string::npos) //单活三
            value += 8000;
        if (s[k].find("1-11") != string::npos ||
            s[k].find("11-1") != string::npos) //跳活三
            value += 7000;
        if (s[k].find("--1110") != string::npos ||
            s[k].find("0111--") != string::npos ||
            s[k].find("-11-10") != string::npos ||
            s[k].find("01-11-") != string::npos ||
            s[k].find("011-1-") != string::npos ||
            s[k].find("-1-110") != string::npos ||
            s[k].find("11--1") != string::npos ||
            s[k].find("1--11") != string::npos ||
            s[k].find("1-1-1") != string::npos ||
            s[k].find("0-111-0") != string::npos) //眠三
            value += 500;
        if (s[k].find("--11--") != string::npos ||
            s[k].find("-1-1-") != string::npos ||
            s[k].find("1--1") != string::npos) //活二
            value += 50;
        if (s[k].find("011---") != string::npos ||
            s[k].find("---110") != string::npos ||
            s[k].find("--1-10") != string::npos ||
            s[k].find("01-1--") != string::npos ||
            s[k].find("-1--10") != string::npos ||
            s[k].find("01--1-") != string::npos ||
            s[k].find("1---1") != string::npos) //眠二
            value += 10;
    }

    //离中间越近权重越高
    int weight = MAXN / 2 - max(abs(x - MAXN / 2), abs(y - MAXN / 2));
    return value * weight;
}

void AI::Play()
{
    //初始化棋局
    vector<vector<int>> state;
    for (int i = 0; i < MAXN; i++)
    {
        vector<int> tmp;
        for (int j = 0; j < MAXN; j++)
            tmp.push_back(0);
        state.push_back(tmp);
    }
    //依次下棋
    int step = 0;
    while (true)
    {
        //用户下一步
        step++;
        int x = -1, y = -1;
        while (true)
        {
            printf("Player 1, please enter the position of chess:");
            scanf("%d%d", &x, &y);
            if (x > MAXN || x < 1 || y > MAXN || y < 1)
                printf("The position should be in [1, %d]x[1, %d]\n", MAXN, MAXN);
            else if (state[x - 1][y - 1] != 0)
                printf("ERROR: Invalid Position!\n");
            else
            {
                x = x - 1;
                y = y - 1;
                state[x][y] = 1;
                break;
            }
        }
        if (Check_Winner(state, 1))
        {
            printf("=======================\n");
            printf("Player 1 Win!\n");
            printf("=======================\n");
            return;
        }

        //检测平局
        if (step == MAXN * MAXN)
        {
            printf("=======================\n");
            printf("DRAW!\n");
            printf("=======================\n");
            return;
        }

        //机器下一步
        step++;
        if (step == 2)
        {
            //第一步做特殊处理, 靠近对手下的棋子
            vector<struct Action> pos;
            struct Action temp;
            //判断8个方向是否可行
            if (x - 1 >= 0)
            {
                temp.x = x - 1;
                temp.y = y;
                pos.push_back(temp);
            }
            if (x + 1 < MAXN)
            {
                temp.x = x + 1;
                temp.y = y;
                pos.push_back(temp);
            }
            if (y - 1 >= 0)
            {
                temp.x = x;
                temp.y = y - 1;
                pos.push_back(temp);
            }
            if (y + 1 < MAXN)
            {
                temp.x = x;
                temp.y = y + 1;
                pos.push_back(temp);
            }
            if (x - 1 >= 0 && y - 1 >= 0)
            {
                temp.x = x - 1;
                temp.y = y - 1;
                pos.push_back(temp);
            }
            if (x - 1 >= 0 && y + 1 < MAXN)
            {
                temp.x = x - 1;
                temp.y = y + 1;
                pos.push_back(temp);
            }
            if (x + 1 < MAXN && y - 1 >= 0)
            {
                temp.x = x + 1;
                temp.y = y - 1;
                pos.push_back(temp);
            }
            if (x + 1 < MAXN && y + 1 < MAXN)
            {
                temp.x = x + 1;
                temp.y = y + 1;
                pos.push_back(temp);
            }
            //随机选取一个位置
            srand((int)time(NULL));
            int index = rand() % pos.size();
            state[pos[index].x][pos[index].y] = 2;
        }
        else
        {
            depth = 0;
            // MIN-MAX搜索
            struct Action result = MAX_Value(state, INT_MIN, INT_MAX);
            state[result.x][result.y] = 2;
        }
        if (Check_Winner(state, 2))
        {
            printf("=======================\n");
            printf("Player 2 Win!\n");
            printf("=======================\n");
            return;
        }

        //输出棋局
        printf("   |");
        for (int i = 0; i < MAXN; i++)
            printf("%2d |", i + 1);
        printf("\n");
        for (int i = 0; i < MAXN; i++)
        {
            printf("%2d |", i + 1);
            for (int j = 0; j < MAXN; j++)
                if (state[i][j] == 0)
                    printf("   |");
                else if (state[i][j] == 1)
                    printf(" x |");
                else if (state[i][j] == 2)
                    printf(" o |");
            printf("\n");
        }

        //检测平局
        if (step == MAXN * MAXN)
        {
            printf("=======================\n");
            printf("DRAW!\n");
            printf("=======================\n");
            return;
        }
    }
    return;
}

int main()
{
    ai.Play();
    return 0;
}
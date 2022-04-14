/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "A_star.h"
 * Function:  Head file for A* Search
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <queue>
#include <vector>
#include <stack>
using namespace std;

vector<vector<int>> target_state; //目标状态

/*用于回溯操作序列的节点*/
struct op_node
{
    char op;                //该节点的操作U、D、L、R
    struct op_node *parent; //该节点的父节点, 便于回溯
};

/*搜索时的每个节点*/
class Node
{
public:
    int g;                     // 过往的代价
    int h;                     // 估计将来的代价(启发式函数)
    int f;                     // g+h
    vector<vector<int>> state; //该节点的状态
    char op;                   //该节点相对其父节点的操作
    struct op_node *parent;    //该节点在访问树上的父节点

    //初始化节点
    Node(int _g, char _op, const vector<vector<int>> &_state, int (*_h)(vector<vector<int>>))
    {
        this->state.assign(_state.begin(), _state.end());
        this->g = _g;
        this->h = _h(this->state);
        this->f = this->g + this->h;
        this->op = _op;
    }

    //为优先队列排序重载运算符
    bool operator<(const Node &n) const
    {
        return this->f > n.f;
    }
};

int h(vector<vector<int>> curr_state)
{
    /*启发式函数(曼哈顿距离)*/

    int distance = 0;
    //遍历当前格点的所有数字, 计算到目标位置的曼哈顿距离之和
    for (int i = 0; i < curr_state.size(); i++)
    {
        for (int j = 0; j < curr_state[i].size(); j++)
        {
            //忽略空白格子
            if (curr_state[i][j] <= 0)
                continue;
            //查找在target中的位置
            for (int i_ = 0; i_ < target_state.size(); i_++)
            {
                for (int j_ = 0; j_ < target_state[i_].size(); j_++)
                {
                    //计算曼哈顿距离
                    if (curr_state[i][j] == target_state[i_][j_])
                    {
                        int dx = abs(i - i_);
                        int dy = abs(j - j_);
                        distance += (dx + dy);
                    }
                }
            }
        }
    }
    return distance;
}

void ExpendNode(Node current_node, priority_queue<Node> &open_list, struct op_node *curr_op, int (*h)(vector<vector<int>>))
{
    /*
     *   拓展current_node节点
     *
     *   current_node:  当前的节点
     *   open_list:     拓展但未访问节点列表
     *   curr_op:       当前的操作节点, 用于链接
     *   h:             启发式函数
     *
     *   拓展节点并加入到open_list
     */

    //查找空格位置
    bool find = false;
    int i = 0, j = 0;
    for (i = 0; i < current_node.state.size(); i++)
    {
        for (j = 0; j < current_node.state[i].size(); j++)
        {
            //寻找宇宙飞船的位置
            if (current_node.state[i][j] == 0)
            {
                find = true;
                break;
            }
        }
        if (find)
            break;
    }

    //移动数字
    // U: (i-1,j)--(i,j)
    if (i - 1 >= 0)
    {
        vector<vector<int>> tmp(current_node.state);
        tmp[i][j] = tmp[i - 1][j];
        tmp[i - 1][j] = 0;
        Node tmp_node(current_node.g + 1, 'U', tmp, h);
        tmp_node.parent = curr_op; //链接到父节点
        open_list.push(tmp_node);
    }

    // D: (i+1,j)--(i,j)
    if (i + 1 < 5)
    {
        vector<vector<int>> tmp(current_node.state);
        tmp[i][j] = tmp[i + 1][j];
        tmp[i + 1][j] = 0;
        Node tmp_node(current_node.g + 1, 'D', tmp, h);
        tmp_node.parent = curr_op; //链接到父节点
        open_list.push(tmp_node);
    }

    // L: (i,j-1)--(i,j)
    if (j - 1 >= 0)
    {
        vector<vector<int>> tmp(current_node.state);
        tmp[i][j] = tmp[i][j - 1];
        tmp[i][j - 1] = 0;
        Node tmp_node(current_node.g + 1, 'L', tmp, h);
        tmp_node.parent = curr_op; //链接到父节点
        open_list.push(tmp_node);
    }

    // R: (i,j+1)--(i,j)
    if (j + 1 < 5)
    {
        vector<vector<int>> tmp(current_node.state);
        tmp[i][j] = tmp[i][j + 1];
        tmp[i][j + 1] = 0;
        Node tmp_node(current_node.g + 1, 'R', tmp, h);
        tmp_node.parent = curr_op; //链接到父节点
        open_list.push(tmp_node);
    }
    return;
}

void PrintOut(struct op_node *curr_op)
{
    /*
     *   从curr_op开始向上回溯, 输出移动序列
     *
     *   curr_op: 最终的操作节点
     *
     *   输出移动序列
     */

    struct op_node *temp = curr_op;
    /*回溯移动序列*/
    stack<char> op_stack;
    while (temp->parent != NULL)
    {
        op_stack.push(temp->op); //利用栈倒序
        temp = temp->parent;
    }
    FILE *fp;
    fp = fopen("output.txt", "w");
    if (fp == NULL)
    {
        printf("Create output file failed!\n");
        exit(-1);
    }
    while (!op_stack.empty())
    {
        fprintf(fp, "%c", op_stack.top());
        printf("%c", op_stack.top());
        op_stack.pop();
    }
    return;
}

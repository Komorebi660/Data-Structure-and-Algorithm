/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "A_star.cpp"
 * Function:  A* Search
 *************************************************************/
#include "A_star.h"
using namespace std;

extern vector<vector<int>> target_state; //目标状态

void A_Star(const vector<vector<int>> &start, const vector<vector<int>> &target, int (*h)(vector<vector<int>>))
{
    /*
     *   A*算法
     *
     *   start:     初始状态
     *   target:    目标状态
     *   h:         启发式函数
     *
     *   输出移动序列
     */
    target_state.assign(target.begin(), target.end()); //设定目标状态

    //初始化
    priority_queue<Node> open_list; //优先队列存储扩展的节点
    Node init_node(0, 0, start, h);
    init_node.parent = NULL;
    open_list.push(init_node);
    vector<struct op_node *> visited; //维护已访问节点, 便于free malloc的空间

    while (!open_list.empty())
    {
        /*从优先队列中弹出f最小的状态*/
        Node current_node = open_list.top();
        open_list.pop();
        struct op_node *curr_op = (struct op_node *)malloc(sizeof(struct op_node)); //分配空间存储
        curr_op->op = current_node.op;
        curr_op->parent = current_node.parent;
        visited.push_back(curr_op);

        /*检查该状态是否符合要求*/
        if (current_node.state == target)
        {
            //输出移动序列
            PrintOut(curr_op);
            //释放malloc空间
            for (int i = 0; i < visited.size(); i++)
                free(visited[i]);
            return;
        }

        /*扩展结点*/
        ExpendNode(current_node, open_list, curr_op, h);
    }
}

int main()
{
    /*读入数据*/
    FILE *fp;
    vector<vector<int>> start;
    fp = fopen("input.txt", "r");
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

    vector<vector<int>> target;
    fp = fopen("target.txt", "r");
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

    printf("A* Algorithm: ");
    double _start = clock();
    A_Star(start, target, h);
    double _end = clock();
    printf("\nTime=%.6lfs\n", (_end - _start) / CLOCKS_PER_SEC);
    return 0;
}
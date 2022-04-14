/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "SimpleConnectGraph.cpp"
 *************************************************************/
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <queue>
using namespace std;

#define MAXN 5001
#define MAXM 100001

struct VNode
{
    int vex;
    struct VNode *next;
};

struct Graph
{
    int vexnum;
    int edgenum;
    struct VNode *vhead;
};

int dfn[MAXN], low[MAXN], stack[MAXN];
int belong[MAXN];   //belong[i]表示节点i在哪一个强连通片内
bool instack[MAXN]; //instack[i]表示节点i是否在栈中
int bn, dn, top;    //bn:强连通片数目，dn:时间戳，top:栈顶指针
//从u开始计算强连通分量
void Tarjan(struct Graph G, int u)
{
    //设定初值并压入栈中
    dfn[u] = low[u] = ++dn; //从1开始，0用于判断是否被访问
    stack[++top] = u;
    instack[u] = 1;

    //遍历每一条邻边
    for (struct VNode *p = G.vhead[u].next; p != NULL; p = p->next)
    {
        int v = p->vex;
        if (dfn[v] == 0) //未被访问
        {
            Tarjan(G, v);
            low[u] = min(low[u], low[v]);
        }
        else if (instack[v]) //已经在栈中
        {
            low[u] = min(low[u], dfn[v]);
        }
    }

    //找到强连通分量
    if (dfn[u] == low[u])
    {
        //删除从节点一直到u
        int v;
        do
        {
            v = stack[top--];
            belong[v] = bn; //节点v属于第bn个强连通分量
            instack[v] = 0;
        } while (u != v);
        ++bn;
    }
}

int deg[MAXN]; //deg[i]缩点以后的图中第i个节点的入度
//拓扑排序，对每个节点的入度做判断
bool TopoSort(struct Graph G)
{
    queue<int> q;
    for (int i = 0; i < G.vexnum; ++i)
    {
        if (deg[i] == 0)
            q.push(i);
    }
    if (q.size() > 1)
        return false;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (struct VNode *p = G.vhead[u].next; p != NULL; p = p->next)
        {
            int v = p->vex;
            if (--deg[v] == 0)
                q.push(v);
        }
        if (q.size() > 1)
            return false;
    }
    return true;
}

//求解最大的单连通分支
void slove(struct Graph G)
{
    /* 求图中的所有强连通分量 */
    top = bn = dn = 0;
    memset(instack, 0, sizeof(bool) * MAXN);
    memset(dfn, 0, sizeof(int) * MAXN);
    for (int i = 0; i < G.vexnum; ++i)
    {
        //存在未被访问的节点
        if (dfn[i] == 0)
            Tarjan(G, i);
    }

    /* 缩点，变成DAG */
    memset(deg, 0, sizeof(int) * MAXN);
    struct Graph G_; //缩点以后的图
    G_.vexnum = bn;
    G_.vhead = (struct VNode *)malloc(sizeof(struct VNode) * bn);
    for (int i = 0; i < bn; i++)
    {
        G_.vhead[i].vex = i;
        G_.vhead[i].next = NULL;
    }
    //遍历原图中的所有边
    for (int i = 0; i < G.vexnum; ++i)
    {
        int u = belong[i];
        for (struct VNode *p = G.vhead[i].next; p != NULL; p = p->next)
        {
            int v = belong[p->vex];

            //属于同一个连通片
            if (u == v)
                continue;

            struct VNode *temp = (struct VNode *)malloc(sizeof(struct VNode));
            temp->next = G_.vhead[u].next;
            temp->vex = v;
            G_.vhead[u].next = temp;
            ++deg[v]; //v的入读+1
        }
    }

    /* 通过拓扑排序判断单连通 */
    if (TopoSort(G_))
        printf("true\n");
    else
        printf("false\n");
    return;
}

int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    struct Graph G;
    G.vexnum = n;
    G.edgenum = m;
    G.vhead = (struct VNode *)malloc(sizeof(struct VNode) * n);
    for (int i = 0; i < n; i++)
    {
        G.vhead[i].vex = i;
        G.vhead[i].next = NULL;
    }

    for (int i = 0; i < m; i++)
    {
        int u, v;
        scanf("%d%d", &u, &v);

        struct VNode *temp = (struct VNode *)malloc(sizeof(struct VNode));
        temp->next = G.vhead[u - 1].next;
        temp->vex = v - 1;
        G.vhead[u - 1].next = temp;
    }

    slove(G);
    return 0;
}
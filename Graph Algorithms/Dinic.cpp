/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "Dinic.cpp"
*************************************************************/
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <queue>
using namespace std;

struct VNode
{
    int vex;
    int cap;             //存储边的容量
    int flow;            //当前边的流量
    struct VNode *redge; //反向边指针
    struct VNode *next;
};

struct Graph
{
    int vexnum;
    int edgenum;
    struct VNode *vhead;
} G;

int depth[1000]; //保存节点在bfs的深度

//广度优先搜索，生成depth[]
bool bfs(struct Graph G, int s, int t)
{
    for (int i = 0; i < G.vexnum; i++)
        depth[i] = -1;
    depth[s] = 1;

    queue<int> q;
    q.push(s);

    while (!q.empty())
    {
        int i = q.front();
        q.pop();
        for (struct VNode *p = G.vhead[i].next; p != NULL; p = p->next)
        {
            int j = p->vex;
            if (p->cap - p->flow > 0 && depth[j] == -1)
            {
                depth[j] = depth[i] + 1;
                q.push(j);
            }
        }
    }
    return depth[t] != -1;
}

//找从s到t的增广路径，flow是当前的可用流量
int dfs(struct Graph G, int s, int t, int flow)
{
    if (s == t)
        return flow;

    int addf = 0; //当前增加的流量

    //搜索s的每个邻节点
    for (struct VNode *p = G.vhead[s].next; (p != NULL) && (addf < flow); p = p->next)
    {
        int i = p->vex;
        if (depth[i] == depth[s] + 1 && p->cap - p->flow > 0)
        {
            int delta = dfs(G, i, t, min(flow - addf, p->cap - p->flow));
            addf += delta;
            p->flow += delta;
            p->redge->flow -= delta;
        }
    }
    return addf;
}

//最大流算法
void Dinic(struct Graph G, int s, int t)
{
    int ans = 0;
    while (bfs(G, s, t))
    {
        ans += dfs(G, s, t, INT_MAX);
    }
    printf("%d\n", ans);
    return;
}

int main()
{
    int n, m, s, t;
    scanf("%d%d%d%d", &n, &m, &s, &t);
    G.vexnum = n;
    G.edgenum = m;
    G.vhead = (struct VNode *)malloc(sizeof(struct VNode) * n);
    for (int i = 0; i < n; i++)
    {
        G.vhead[i].vex = i;
        G.vhead[i].cap = 0;
        G.vhead[i].flow = 0;
        G.vhead[i].redge = NULL;
        G.vhead[i].next = NULL;
    }

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);

        struct VNode *temp1 = (struct VNode *)malloc(sizeof(struct VNode));
        struct VNode *temp2 = (struct VNode *)malloc(sizeof(struct VNode));

        //正向边
        temp1->cap = w;
        temp1->flow = 0;
        temp1->redge = temp2;
        temp1->next = G.vhead[u - 1].next;
        temp1->vex = v - 1;
        G.vhead[u - 1].next = temp1;

        //反向边
        temp2->cap = w;
        temp2->flow = w;
        temp2->redge = temp1;
        temp2->next = G.vhead[v - 1].next;
        temp2->vex = u - 1;
        G.vhead[v - 1].next = temp2;
    }

    Dinic(G, s - 1, t - 1);
    return 0;
}
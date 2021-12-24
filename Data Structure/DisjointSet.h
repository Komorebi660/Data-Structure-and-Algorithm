#include <iostream>

class DisjointSet
{
public:
    DisjointSet(int n);
    ~DisjointSet();
    int Find(int x);
    void Union(int x, int y);
    bool is_same(int x, int y);

private:
    int *node; //节点i所属的集合存储在node[i]中
    int *rank; //树高
};

DisjointSet::DisjointSet(int n)
{
    node = (int *)malloc(sizeof(int) * n);
    rank = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        node[i] = i;
        rank[i] = 0;
    }
}

DisjointSet::~DisjointSet()
{
    free(node);
    free(rank);
}

//查找当前元素所在树的根节点(代表元素)
int DisjointSet::Find(int x)
{
    if (x == node[x])
        return x;
    //在第一次查找时，将节点直连到根节点
    return node[x] = Find(node[x]);
}

//合并元素x，y所处的集合
void DisjointSet::Union(int x, int y)
{
    //查找到x，y的根节点
    x = Find(x);
    y = Find(y);
    //在同一个集合
    if (x == y)
        return;
    //判断两棵树的高度，将较矮的树合并到较大的树上
    if (rank[x] < rank[y])
        node[x] = y;
    else
    {
        node[y] = x;
        //树高加1
        if (rank[x] == rank[y])
            rank[x]++;
    }
}

//判断x，y是否属于同一个集合
bool DisjointSet::is_same(int x, int y)
{
    return Find(x) == Find(y);
}

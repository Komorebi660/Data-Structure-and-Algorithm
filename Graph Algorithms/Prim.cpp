#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
using namespace std;

class FibNode
{
public:
    int key;         // 关键字,在本题中是边的权值
    int vex;         // 点的标号
    int degree;      // 度数
    FibNode *left;   // 左兄弟
    FibNode *right;  // 右兄弟
    FibNode *child;  // 第一个孩子节点
    FibNode *parent; // 父节点
    bool marked;     // 是否被删除第一个孩子

    FibNode(int value, int vex_num)
    {
        key = value;
        vex = vex_num;
        degree = 0;
        marked = false;
        left = this; //指向自己
        right = this;
        parent = NULL;
        child = NULL;
    };
};

class FibHeap
{
private:
    int keyNum;     // 堆中节点的总数
    int maxDegree;  // 最大度
    FibNode *min;   // 最小节点(某个最小堆的根节点)
    FibNode **cons; // 为了删除最小节点后的consolidate操作预留的空间

public:
    FibHeap();

    // 新建key对应的节点，并将其插入到斐波那契堆中
    FibNode *insert(int key, int vex);
    // 移除斐波那契堆中的最小节点，返回最小节点
    FibNode *removeMin();
    // 将斐波那契堆中节点node的值减少为key
    void decrease(FibNode *node, int key);

private:
    // 将node从双链表移除
    void removeNode(FibNode *node);
    // 将node堆结点加入root结点之前(循环链表中)
    void addNode(FibNode *node, FibNode *root);
    // 将"堆的最小结点"从根链表中移除，
    FibNode *extractMin();
    // 将node链接到root根结点
    void link(FibNode *node, FibNode *root);
    // 合并斐波那契堆的根链表中左右相同度数的树
    void consolidate();
    // 将node从父节点parent的子链接中剥离出来，并使node成为"堆的根链表"中的一员。
    void cut(FibNode *node, FibNode *parent);
    // 对节点node进行"级联剪切"
    void cascadingCut(FibNode *node);
};

FibHeap::FibHeap()
{
    keyNum = 0;
    maxDegree = 0;
    min = NULL;
    cons = NULL;
}

/*
 * 将node堆结点加入root结点之前(循环链表中)
 *   a …… root
 *   a …… node …… root
*/
void FibHeap::addNode(FibNode *node, FibNode *root)
{
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
}

/*
 * 将node堆结点删除
 *   a …… node …… b
 *   a …… b
*/
void FibHeap::removeNode(FibNode *node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

/*
 * 将新建key，插入到斐波那契堆中
 */
FibNode *FibHeap::insert(int key, int vex)
{
    FibNode *node = new FibNode(key, vex);
    if (node == NULL)
        return NULL;
    if (keyNum == 0)
        min = node;
    else
    {
        addNode(node, min);
        if (node->key < min->key)
            min = node;
    }
    keyNum++;
    return node;
}

/*
 * 将最小节点所属的树从堆中移除
 */
FibNode *FibHeap::extractMin()
{
    FibNode *p = min;

    //堆顶只有一个节点
    if (p == p->right)
        min = NULL;

    else
    {
        removeNode(p);
        min = p->right;
    }
    //p单独成堆
    p->left = p->right = p;

    return p;
}

/*
 * 将node链接到root根结点
 */
void FibHeap::link(FibNode *node, FibNode *root)
{
    // 将node从双链表中移除
    removeNode(node);
    // 将node设为root的孩子
    if (root->child == NULL)
        root->child = node;
    else
        addNode(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = false;
}

/* 
 * 合并斐波那契堆的根链表中左右相同度数的树
 */
void FibHeap::consolidate()
{
    //开辟哈希所用空间
    int old = maxDegree;
    // 计算log2(keyNum)，"+1"意味着向上取整
    maxDegree = (int)(log(keyNum) / log(2.0)) + 1;
    if (old < maxDegree)
        cons = (FibNode **)realloc(cons, sizeof(FibHeap *) * (maxDegree + 2));
    //初始化
    int D = maxDegree + 1;
    for (int i = 0; i < D; i++)
        cons[i] = NULL;

    //合并相同度的根节点，使每个度数的树唯一
    while (min != NULL)
    {
        FibNode *x = extractMin(); // 取出堆中的最小树(最小节点所在的树)
        int d = x->degree;         // 获取最小树的度数
        // cons[d] != NULL，意味着有两棵树(x和y)的度数相同。
        while (cons[d] != NULL)
        {
            FibNode *y = cons[d]; // y是与x的度数相同的树
            // 比较两者的键值，保证x的键值比y小
            if (x->key > y->key)
                swap(x, y);
            link(y, x); //将y链接到x中
            cons[d] = NULL;
            d++;
        }
        cons[d] = x;
    }

    // 将cons中的结点重新加到根表中
    min = NULL;
    for (int i = 0; i < D; i++)
    {
        if (cons[i] != NULL)
        {
            if (min == NULL)
                min = cons[i];
            else
            {
                addNode(cons[i], min); //插入到根表
                //设置min
                if ((cons[i])->key < min->key)
                    min = cons[i];
            }
        }
    }
}

/*
 * 移除最小节点
 */
FibNode *FibHeap::removeMin()
{
    if (min == NULL)
        return NULL;
    FibNode *m = min;
    // 将min每一个儿子都添加到斐波那契堆的根链表中
    while (m->child != NULL)
    {
        FibNode *child = m->child;
        removeNode(child);         //从双向链表中移除
        if (child->right == child) //只剩最后一个节点
            m->child = NULL;
        else
            m->child = child->right;
        addNode(child, min); //加到斐波那契堆的根链表中
        child->parent = NULL;
    }

    // 将m从根链表中移除
    removeNode(m);
    // 若m是堆中唯一节点，则设置堆的最小节点为NULL；
    if (m->right == m)
        min = NULL;
    // 否则，设置堆的最小节点为一个非空节点(m->right)，然后再进行调节。
    else
    {
        min = m->right;
        consolidate();
    }
    keyNum--;
    return m;
}

/* 
 * 将node从父节点parent的子链接中剥离出来，
 * 并使node成为"堆的根链表"中的一员。
 */
void FibHeap::cut(FibNode *node, FibNode *parent)
{
    removeNode(node);
    parent->degree--;
    // node没有兄弟
    if (node == node->right)
        parent->child = NULL;
    else
        parent->child = node->right;

    node->parent = NULL;
    node->left = node->right = node;
    node->marked = false;
    // 将"node所在树"添加到"根链表"中
    addNode(node, min);
}

/* 
 * 对节点node进行"级联剪切"
 */
void FibHeap::cascadingCut(FibNode *node)
{
    FibNode *parent = node->parent;
    if (parent != NULL)
    {
        if (node->marked == false)
            node->marked = true;
        else
        {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

/* 
 * 将斐波那契堆中节点node的值减少为key
 */
void FibHeap::decrease(FibNode *node, int key)
{
    if (min == NULL || node == NULL)
        return;
    if (key >= node->key)
        return;

    node->key = key;
    FibNode *parent = node->parent;
    if (parent != NULL && node->key < parent->key)
    {
        // 将node从父节点parent中剥离出来，并将node添加到根链表中
        cut(node, parent);
        cascadingCut(parent);
    }
    // 更新最小节点
    if (node->key < min->key)
        min = node;
}

struct VNode
{
    int vex;
    int weight;
    struct VNode *next;
};

struct Graph
{
    int vexnum;
    int edgenum;
    struct VNode *vhead;
} G;

struct node
{
    FibNode *fnode;
    int weight;
    bool has_visited;
};

void Prim(struct Graph G)
{
    struct node *d = (struct node *)malloc(sizeof(struct node) * G.vexnum);
    FibHeap fheap;

    //从0开始搜索
    d[0].has_visited = true;
    d[0].weight = 0;
    d[0].fnode = fheap.insert(0, 0);
    //初始化
    for (int i = 1; i < G.vexnum; i++)
    {
        d[i].has_visited = false;
        d[i].weight = INT_MAX;
        d[i].fnode = fheap.insert(INT_MAX, i);
    }

    int result = 0;
    //循环n次
    for (int _ = 0; _ < G.vexnum; _++)
    {
        //获取最小值对应的点
        struct FibNode *min = fheap.removeMin();
        int min_i = min->vex;

        //将该点加入集合
        d[min_i].has_visited = true;
        result += min->key;

        //更新d[]
        for (struct VNode *p = G.vhead[min_i].next; p != NULL; p = p->next)
        {
            if (!d[p->vex].has_visited && p->weight < d[p->vex].weight)
            {
                d[p->vex].weight = p->weight;
                fheap.decrease(d[p->vex].fnode, p->weight);
            }
        }
    }

    printf("%d\n", result);
    return;
}

int main()
{
    int n, m;
    scanf("%d%d", &n, &m);
    G.vexnum = n;
    G.edgenum = m;
    G.vhead = (struct VNode *)malloc(sizeof(struct VNode) * n);
    for (int i = 0; i < n; i++)
    {
        G.vhead[i].vex = i;
        G.vhead[i].weight = 0;
        G.vhead[i].next = NULL;
    }

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);

        struct VNode *temp1 = (struct VNode *)malloc(sizeof(struct VNode));
        temp1->weight = w;
        temp1->next = G.vhead[u - 1].next;
        temp1->vex = v - 1;
        G.vhead[u - 1].next = temp1;

        struct VNode *temp2 = (struct VNode *)malloc(sizeof(struct VNode));
        temp2->weight = w;
        temp2->next = G.vhead[v - 1].next;
        temp2->vex = u - 1;
        G.vhead[v - 1].next = temp2;
    }

    Prim(G);
    return 0;
}
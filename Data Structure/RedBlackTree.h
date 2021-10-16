/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "RedBlackTree.h"
* Function:  Data Structure Red-Black Tree
*************************************************************/
#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
using namespace std;

#define RED 0   // 红色节点
#define BLACK 1 // 黑色节点

typedef int T;

// 红黑树的节点
struct RBTreeNode
{
    unsigned char color;       // 颜色(RED 或 BLACK)
    T *key;                    // 关键字(键值)
    struct RBTreeNode *left;   // 左孩子
    struct RBTreeNode *right;  // 右孩子
    struct RBTreeNode *parent; // 父结点
};

class RedBlackTree
{
private:
    struct RBTreeNode *root;
    int (*cmp)(T *t1, T *t2);
    void Destroy(struct RBTreeNode *root);
    void _PreorderTraverse(struct RBTreeNode *root, void (*func)(struct RBTreeNode *));
    void _InorderTraverse(struct RBTreeNode *root, void (*func)(struct RBTreeNode *));
    void _PostorderTraverse(struct RBTreeNode *root, void (*func)(struct RBTreeNode *));
    void left_rotate(struct RBTreeNode *x);
    void right_rotate(struct RBTreeNode *y);
    void Adjust_Insert(struct RBTreeNode *node);
    void Adjust_Delete(struct RBTreeNode *node, struct RBTreeNode *parent);

public:
    RedBlackTree(int (*cmp)(T *, T *));
    ~RedBlackTree();
    bool Insert(T *key);
    bool Delete(T *key);
    struct RBTreeNode *Search(T *key);
    void PreorderTraverse(void (*func)(struct RBTreeNode *));
    void InorderTraverse(void (*func)(struct RBTreeNode *));
    void PostorderTraverse(void (*func)(struct RBTreeNode *));
    struct RBTreeNode *Max();
    struct RBTreeNode *Min();
    struct RBTreeNode *FindForward(T *key);
    struct RBTreeNode *FindBackward(T *key);
};

//初始化
RedBlackTree::RedBlackTree(int (*cmp)(T *, T *))
{
    this->root = NULL;
    this->cmp = cmp;
    return;
}

//销毁
RedBlackTree::~RedBlackTree()
{
    Destroy(this->root);
}
void RedBlackTree::Destroy(struct RBTreeNode *root)
{
    //后序遍历销毁树
    if (root == NULL)
        return;
    Destroy(root->left);
    Destroy(root->right);
    free(root->key);
    free(root);
    return;
}

//前序遍历
void RedBlackTree::PreorderTraverse(void (*func)(struct RBTreeNode *))
{
    _PreorderTraverse(this->root, func);
    return;
}
void RedBlackTree::_PreorderTraverse(struct RBTreeNode *root, void (*func)(struct RBTreeNode *))
{
    if (root == NULL)
        return;
    func(root);
    _PreorderTraverse(root->left, func);
    _PreorderTraverse(root->right, func);
    return;
}

//中序遍历
void RedBlackTree::InorderTraverse(void (*func)(struct RBTreeNode *))
{
    _InorderTraverse(this->root, func);
    return;
}
void RedBlackTree::_InorderTraverse(struct RBTreeNode *root, void (*func)(struct RBTreeNode *))
{
    if (root == NULL)
        return;
    _InorderTraverse(root->left, func);
    func(root);
    _InorderTraverse(root->right, func);
    return;
}

//后序遍历
void RedBlackTree::PostorderTraverse(void (*func)(struct RBTreeNode *))
{
    _PostorderTraverse(this->root, func);
    return;
}
void RedBlackTree::_PostorderTraverse(struct RBTreeNode *root, void (*func)(struct RBTreeNode *))
{
    if (root == NULL)
        return;
    _PostorderTraverse(root->left, func);
    _PostorderTraverse(root->right, func);
    func(root);
    return;
}

//搜索
struct RBTreeNode *RedBlackTree::Search(T *t)
{
    struct RBTreeNode *p = this->root;
    while (p != NULL)
    {
        if (this->cmp(t, p->key) == 0)
            return p;

        //比当前节点小，则往左子树找
        if (this->cmp(t, p->key) < 0)
            p = p->left;
        //比当前节点大，则往右子树找
        else
            p = p->right;
    }
    return NULL;
}

//找最大值
struct RBTreeNode *RedBlackTree::Max()
{
    struct RBTreeNode *p = this->root;
    struct RBTreeNode *q = p;
    if (p == NULL)
        return NULL;

    //找右子树的最右节点
    while (p != NULL)
    {
        q = p;
        p = p->right;
    }
    return q;
}

//找最小值
struct RBTreeNode *RedBlackTree::Min()
{
    struct RBTreeNode *p = this->root;
    struct RBTreeNode *q = p;
    if (p == NULL)
        return NULL;

    //找左子树的最左节点
    while (p != NULL)
    {
        q = p;
        p = p->left;
    }
    return q;
}

//找节点t的前驱
struct RBTreeNode *RedBlackTree::FindForward(T *t)
{
    struct RBTreeNode *p = Search(t);
    if (p == NULL)
        return NULL;

    struct RBTreeNode *q;
    if (p->left == NULL)
    {
        //向上找父节点，使得q->rchild==p
        q = p->parent;
        while (q != NULL && q->right != p)
        {
            p = q;
            q = q->parent;
        }
        //第一个节点
        if (q == NULL)
            return NULL;
    }
    else
    {
        //找左子树的最大值
        q = p->left;
        p = q;
        while (p != NULL)
        {
            q = p;
            p = p->right;
        }
    }

    return q;
}

//找节点t的后继
struct RBTreeNode *RedBlackTree::FindBackward(T *t)
{
    struct RBTreeNode *p = Search(t);
    struct RBTreeNode *q;
    if (p->right == NULL)
    {
        //向上找父节点，使得q->lchild==p
        q = p->parent;
        while (q != NULL && q->left != p)
        {
            p = q;
            q = q->parent;
        }
        //最后一个节点
        if (q == NULL)
            return NULL;
    }
    else
    {
        //找右子树的最小值
        q = p->right;
        p = q;
        while (p != NULL)
        {
            q = p;
            p = p->left;
        }
    }

    return q;
}

//左旋
void RedBlackTree::left_rotate(struct RBTreeNode *x)
{
    /* 对红黑树的节点(x)进行左旋转:
     *      px                          px
     *     /                           /
     *    x                           y
     *   /  \      --(左旋)-->       / \                
     *  lx   y                      x  ry
     *     /   \                   /  \
     *    ly   ry                 lx  ly
    */
    struct RBTreeNode *y = x->right;

    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;
    // 如果 “x的父亲” 是空节点，则将y设为根节点
    if (x->parent == NULL)
        this->root = y;
    else
    {
        // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        if (x->parent->left == x)
            x->parent->left = y;
        // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
    return;
}

//右旋
void RedBlackTree::right_rotate(struct RBTreeNode *y)
{
    /* 对红黑树的节点(y)进行右旋转
     *         py                         py
     *        /                           /
     *       y                           x
     *      / \      --(右旋)-->        /  \                     
     *     x  ry                       lx  y
     *    / \                             / \                  
     *   lx  rx                          rx  ry
    */
    struct RBTreeNode *x = y->left;

    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;
    // 如果 “y的父亲” 是空节点，则将x设为根节点
    if (y->parent == NULL)
        this->root = x;
    else
    {
        // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        if (y == y->parent->right)
            y->parent->right = x;
        // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
        else
            y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
    return;
}

//插入
bool RedBlackTree::Insert(T *t)
{
    struct RBTreeNode *p = this->root;
    //构造新节点
    struct RBTreeNode *temp = (struct RBTreeNode *)malloc(sizeof(struct RBTreeNode));
    if (temp == NULL)
    {
        printf("Can not allocate memeory!\n");
        exit(-1);
    }
    //空树插入
    if (p == NULL)
    {
        temp->parent = NULL;
        temp->left = NULL;
        temp->right = NULL;
        temp->color = BLACK; //设为黑色
        temp->key = (T *)malloc(sizeof(T));
        if (temp->key == NULL)
        {
            printf("Can not allocate memeory!\n");
            exit(-1);
        }
        memcpy(temp->key, t, sizeof(T));
        this->root = temp;
        return true;
    }

    while (1)
    {
        //重复值不插入
        if (this->cmp(t, p->key) == 0)
            return false;

        //比当前节点小，则往左子树找
        if (this->cmp(t, p->key) < 0)
        {
            if (p->left == NULL)
            {
                temp->parent = p;
                temp->left = NULL;
                temp->right = NULL;
                temp->color = RED; //设为红色
                temp->key = (T *)malloc(sizeof(T));
                if (temp->key == NULL)
                {
                    printf("Can not allocate memeory!\n");
                    exit(-1);
                }
                memcpy(temp->key, t, sizeof(T));
                p->left = temp;
                break;
            }
            else
                p = p->left;
        }
        //比当前节点大，则往右子树找
        else
        {
            if (p->right == NULL)
            {
                temp->parent = p;
                temp->left = NULL;
                temp->right = NULL;
                temp->color = RED; //设为红色
                temp->key = (T *)malloc(sizeof(T));
                if (temp->key == NULL)
                {
                    printf("Can not allocate memeory!\n");
                    exit(-1);
                }
                memcpy(temp->key, t, sizeof(T));
                p->right = temp;
                break;
            }
            else
                p = p->right;
        }
    }

    Adjust_Insert(temp); //调整节点颜色
    return true;
}

//红黑树插入修正函数
//在向红黑树中插入节点之后(失去平衡)，再调用该函数；
//目的是将它重新塑造成一颗红黑树。
void RedBlackTree::Adjust_Insert(struct RBTreeNode *node)
{
    struct RBTreeNode *parent, *gparent;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = node->parent) && (parent->color == RED))
    {
        gparent = parent->parent;

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left)
        {
            // Case 1条件：叔叔节点是红色，将冲突上移
            struct RBTreeNode *uncle = gparent->right;
            if (uncle && uncle->color == RED)
            {
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue;
            }
            // Case 2条件：叔叔是黑色，且当前节点是右孩子，将问题转化为case 3
            if (parent->right == node)
            {
                struct RBTreeNode *temp;
                left_rotate(parent);
                temp = parent;
                parent = node;
                node = temp;
            }
            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            parent->color = BLACK;
            gparent->color = RED;
            right_rotate(gparent);
        }
        //若“父节点”是“祖父节点的右孩子”
        else
        {
            // Case 1条件：叔叔节点是红色，将冲突上移
            struct RBTreeNode *uncle = gparent->left;
            if (uncle && uncle->color == RED)
            {
                uncle->color = BLACK;
                parent->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue;
            }
            // Case 2条件：叔叔是黑色，且当前节点是左孩子，将问题转化为case 3
            if (parent->left == node)
            {
                struct RBTreeNode *temp;
                right_rotate(parent);
                temp = parent;
                parent = node;
                node = temp;
            }
            // Case 3条件：叔叔是黑色，且当前节点是右孩子。
            parent->color = BLACK;
            gparent->color = RED;
            left_rotate(gparent);
        }
    }
    // 将根节点设为黑色
    this->root->color = BLACK;
    return;
}

//删除
bool RedBlackTree::Delete(T *t)
{
    struct RBTreeNode *p = Search(t);
    //树中不存在该节点
    if (p == NULL)
        return false;
    //如果左右子树均为空
    if (p->left == NULL && p->right == NULL)
    {
        //删除根节点后要更新根节点
        if (p->parent == NULL)
            this->root = NULL;
        else if (p->parent->left == p)
            p->parent->left = NULL;
        else
            p->parent->right = NULL;

        if (p->color == BLACK)
            Adjust_Delete(NULL, p->parent);
        free(p->key);
        free(p);
    }
    //如果左子树为空
    else if (p->left == NULL)
    {
        if (p->parent == NULL)
        {
            //删除根节点后要更新根节点
            this->root = p->right;
            p->right->parent = NULL;
        }
        else if (p->parent->left == p)
        {
            p->parent->left = p->right;
            p->right->parent = p->parent;
        }
        else
        {
            p->parent->right = p->right;
            p->right->parent = p->parent;
        }
        if (p->color == BLACK)
            Adjust_Delete(p->right, p->parent);
        free(p->key);
        free(p);
    }
    //如果右子树为空
    else if (p->right == NULL)
    {
        if (p->parent == NULL)
        {
            //删除根节点后要更新根节点
            this->root = p->left;
            p->left->parent = NULL;
        }
        else if (p->parent->left == p)
        {
            p->parent->left = p->left;
            p->left->parent = p->parent;
        }
        else
        {
            p->parent->right = p->left;
            p->left->parent = p->parent;
        }
        if (p->color == BLACK)
            Adjust_Delete(p->left, p->parent);
        free(p->key);
        free(p);
    }
    //如果左右子树均不为空
    else
    {
        //找p的后继
        struct RBTreeNode *q = p->right;
        while (q->left != NULL)
            q = q->left;
        //交换后继与待删除元素
        memcpy(p->key, q->key, sizeof(T));
        //在这里的后继节点一定没有左孩子，且一定不是根节点
        if (q->parent->left == q)
        {
            q->parent->left = q->right;
            //如果它有右子树
            if (q->right != NULL)
                q->right->parent = q->parent;
        }
        else
        {
            q->parent->right = q->right;
            //如果它有右子树
            if (q->right != NULL)
                q->right->parent = q->parent;
        }
        if (q->color == BLACK)
            Adjust_Delete(q->right, q->parent);
        free(q->key);
        free(q);
    }
    return true;
}

//红黑树删除修正函数
//在从红黑树中删除节点之后(红黑树失去平衡)，再调用该函数；
//目的是将它重新塑造成一颗红黑树。
void RedBlackTree::Adjust_Delete(struct RBTreeNode *node, struct RBTreeNode *parent)
{
    struct RBTreeNode *sibling; //node的兄弟

    while ((node == NULL || node->color == BLACK) && node != this->root)
    {
        //如果父亲的左孩子为node
        if (parent->left == node)
        {
            sibling = parent->right;
            // Case 1: 兄弟是红色的，转化为后面几种情况
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                left_rotate(parent);
                sibling = parent->right;
            }
            // Case 2: 兄弟是黑色，且它的两个孩子也都是黑色的，将冲突上移。
            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK))
            {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            //兄弟是黑色的，但兄弟的孩子不全为黑色
            else
            {
                // Case 3: 兄弟的左孩子是红色，右孩子为黑色。将问题转化为case 4.
                if (!sibling->right || sibling->right->color == BLACK)
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    right_rotate(sibling);
                    sibling = parent->right;
                }
                // Case 4: 兄弟右孩子是红色的，左孩子任意颜色。调整后即可结束。
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                left_rotate(parent);
                node = this->root;
                break;
            }
        }
        //如果父亲的右孩子为node
        else
        {
            sibling = parent->left;
            // Case 1: 兄弟是红色的，转化为后面几种情况
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                right_rotate(parent);
                sibling = parent->left;
            }
            // Case 2: 兄弟是黑色，且它的两个孩子也都是黑色的，将冲突上移。
            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK))
            {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            //兄弟是黑色的，但兄弟的孩子不全为黑色
            else
            {
                // Case 3: 兄弟的右孩子是红色，左孩子为黑色。将问题转化为case 4.
                if (!sibling->left || sibling->left->color == BLACK)
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    left_rotate(sibling);
                    sibling = parent->left;
                }
                // Case 4: 兄弟的左孩子是红色的，右孩子任意颜色。调整后即可结束。
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                right_rotate(parent);
                node = this->root;
                break;
            }
        }
    }
    if (node)
        node->color = BLACK;
    return;
}

/*
/////////////////////////test////////////////////////
int mycmp(int *a, int *b)
{
    return *a - *b;
}

void func(struct RBTreeNode *node)
{
    printf("%d ", *(node->key));
}

int main()
{
    int a[100] = {100, 34, 34, 13, 12, 11, 99, 78, 20};
    RedBlackTree tree(mycmp);
    for (int i = 0; i < 9; i++)
        tree.Insert(&a[i]);
    tree.InorderTraverse(func);
    printf("\n");
    printf("Max=%d\n", *(tree.Max()->key));
    printf("Min=%d\n", *(tree.Min()->key));
    for (int i = 0; i < 8; i++)
        tree.Delete(&a[i]);
    tree.InorderTraverse(func);
    return 0;
}
/////////////////////////test////////////////////////
*/
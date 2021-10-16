/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "BinarySearchTree.h"
* Function:  Data Structure BST
*************************************************************/
#include <iostream>
#include <stdlib.h>
using namespace std;

template <typename T>
struct BNode
{
    struct BNode *parent; //指向父节点
    struct BNode *lchild; //指向左孩子
    struct BNode *rchild; //指向右孩子
    T *key;               //键值
};

template <typename T>
class BinarySearchTree
{
private:
    struct BNode<T> *root;
    int (*cmp)(T *, T *);
    void _Traverse(struct BNode<T> *root, void (*func)(T *));

public:
    BinarySearchTree(int (*cmp)(T *, T *));
    void Insert(T *t);
    void Delete(T *t);
    struct BNode<T> *Search(T *t);
    T *Max();
    T *Min();
    T *FindForward(T *t);
    T *FindBackward(T *t);
    void Traverse(void (*func)(T *));
};

//初始化
template <typename T>
BinarySearchTree<T>::BinarySearchTree(int (*cmp)(T *, T *))
{
    this->root = NULL;
    this->cmp = cmp;
    return;
}

//搜索
template <typename T>
struct BNode<T> *BinarySearchTree<T>::Search(T *t)
{
    struct BNode<T> *p = this->root;
    while (p != NULL)
    {
        if (this->cmp(t, p->key) == 0)
            return p;

        //比当前节点小，则往左子树找
        if (this->cmp(t, p->key) < 0)
            p = p->lchild;
        //比当前节点大，则往右子树找
        else
            p = p->rchild;
    }
    return NULL;
}

//遍历以root为根的树
template <typename T>
void BinarySearchTree<T>::_Traverse(struct BNode<T> *root, void (*func)(T *))
{
    struct BNode<T> *p = root;
    if (p != NULL)
    {
        _Traverse(p->lchild, func);
        func(p->key);
        _Traverse(p->rchild, func);
    }
    return;
}

//遍历整颗BST
template <typename T>
void BinarySearchTree<T>::Traverse(void (*func)(T *))
{
    _Traverse(this->root, func);
    return;
}

//找最大值
template <typename T>
T *BinarySearchTree<T>::Max()
{
    struct BNode<T> *p = this->root;
    struct BNode<T> *q = p;
    if (p == NULL)
        return NULL;

    //找右子树的最右节点
    while (p != NULL)
    {
        q = p;
        p = p->rchild;
    }
    return q->key;
}

//找最小值
template <typename T>
T *BinarySearchTree<T>::Min()
{
    struct BNode<T> *p = this->root;
    struct BNode<T> *q = p;
    if (p == NULL)
        return NULL;

    //找左子树的最左节点
    while (p != NULL)
    {
        q = p;
        p = p->lchild;
    }
    return q->key;
}

//找节点t的前驱
template <typename T>
T *BinarySearchTree<T>::FindForward(T *t)
{
    struct BNode<T> *p = Search(t);
    if (p == NULL)
        return NULL;

    struct BNode<T> *q;
    if (p->lchild == NULL)
    {
        //向上找父节点，使得q->rchild==p
        q = p->parent;
        while (q != NULL && q->rchild != p)
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
        q = p->lchild;
        p = q;
        while (p != NULL)
        {
            q = p;
            p = p->rchild;
        }
    }

    return q->key;
}

//找节点t的后继
template <typename T>
T *BinarySearchTree<T>::FindBackward(T *t)
{
    struct BNode<T> *p = Search(t);
    struct BNode<T> *q;
    if (p->rchild == NULL)
    {
        //向上找父节点，使得q->lchild==p
        q = p->parent;
        while (q != NULL && q->lchild != p)
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
        q = p->rchild;
        p = q;
        while (p != NULL)
        {
            q = p;
            p = p->lchild;
        }
    }

    return q->key;
}

//插入
template <typename T>
void BinarySearchTree<T>::Insert(T *t)
{
    struct BNode<T> *p = this->root;
    //空树插入
    if (p == NULL)
    {
        struct BNode<T> *temp = (struct BNode<T> *)malloc(sizeof(struct BNode<T>));
        if (temp == NULL)
        {
            printf("Can not allocate memeory!\n");
            exit(-1);
        }
        temp->parent = NULL;
        temp->lchild = NULL;
        temp->rchild = NULL;
        temp->key = (T *)malloc(sizeof(T));
        if (temp->key == NULL)
        {
            printf("Can not allocate memeory!\n");
            exit(-1);
        }
        memcpy(temp->key, t, sizeof(T));
        this->root = temp;
        return;
    }

    while (1)
    {
        if (this->cmp(t, p->key) == 0)
            return;

        //比当前节点小，则往左子树找
        if (this->cmp(t, p->key) < 0)
        {
            if (p->lchild == NULL)
            {
                struct BNode<T> *temp = (struct BNode<T> *)malloc(sizeof(struct BNode<T>));
                if (temp == NULL)
                {
                    printf("Can not allocate memeory!\n");
                    exit(-1);
                }
                temp->parent = p;
                temp->lchild = NULL;
                temp->rchild = NULL;
                temp->key = (T *)malloc(sizeof(T));
                if (temp->key == NULL)
                {
                    printf("Can not allocate memeory!\n");
                    exit(-1);
                }
                memcpy(temp->key, t, sizeof(T));
                p->lchild = temp;
                break;
            }
            else
                p = p->lchild;
        }
        //比当前节点大，则往右子树找
        else
        {
            if (p->rchild == NULL)
            {
                struct BNode<T> *temp = (struct BNode<T> *)malloc(sizeof(struct BNode<T>));
                if (temp == NULL)
                {
                    printf("Can not allocate memeory!\n");
                    exit(-1);
                }
                temp->parent = p;
                temp->lchild = NULL;
                temp->rchild = NULL;
                temp->key = (T *)malloc(sizeof(T));
                if (temp->key == NULL)
                {
                    printf("Can not allocate memeory!\n");
                    exit(-1);
                }
                memcpy(temp->key, t, sizeof(T));
                p->rchild = temp;
                break;
            }
            else
                p = p->rchild;
        }
    }
    return;
}

//删除
template <typename T>
void BinarySearchTree<T>::Delete(T *t)
{
    struct BNode<T> *p = Search(t);
    //树中不存在该节点
    if (p == NULL)
        return;
    //如果左右子树均为空
    if (p->lchild == NULL && p->rchild == NULL)
    {
        //删除根节点后要更新根节点
        if (p->parent == NULL)
            this->root = NULL;
        else if (p->parent->lchild == p)
            p->parent->lchild = NULL;
        else
            p->parent->rchild = NULL;

        free(p->key);
        free(p);
    }
    //如果左子树为空
    else if (p->lchild == NULL)
    {
        if (p->parent == NULL)
        {
            //删除根节点后要更新根节点
            this->root = p->rchild;
            p->rchild->parent = NULL;
        }
        else if (p->parent->lchild == p)
        {
            p->parent->lchild = p->rchild;
            p->rchild->parent = p->parent;
        }
        else
        {
            p->parent->rchild = p->rchild;
            p->rchild->parent = p->parent;
        }

        free(p->key);
        free(p);
    }
    //如果右子树为空
    else if (p->rchild == NULL)
    {
        if (p->parent == NULL)
        {
            //删除根节点后要更新根节点
            this->root = p->lchild;
            p->lchild->parent = NULL;
        }
        else if (p->parent->lchild == p)
        {
            p->parent->lchild = p->lchild;
            p->lchild->parent = p->parent;
        }
        else
        {
            p->parent->rchild = p->lchild;
            p->lchild->parent = p->parent;
        }

        free(p->key);
        free(p);
    }
    //如果左右子树均不为空
    else
    {
        //找p的后继
        struct BNode<T> *q = p->rchild;
        struct BNode<T> *r = q;
        while (q != NULL)
        {
            r = q;
            q = q->lchild;
        }
        //交换后继与待删除元素
        memcpy(p->key, r->key, sizeof(T));
        //删除节点
        if (r->parent->lchild == r)
        {
            r->parent->lchild = r->rchild;
            //如果它有右子树
            if (r->rchild != NULL)
            {
                r->rchild->parent = r->parent;
            }
        }
        else
        {
            r->parent->rchild = r->rchild;
            //如果它有右子树
            if (r->rchild != NULL)
            {
                r->rchild->parent = r->parent;
            }
        }
        free(r->key);
        free(r);
    }
    return;
}

/*
////////////////////////test//////////////////////
int mycmp(int *a, int *b)
{
    return (*a - *b);
}

void f(int *a)
{
    printf("%d ", *a);
    return;
}

int main()
{
    int a[10] = {6, 1, 7, 5, 9, 3, 2, 4, 8, 10};
    BinarySearchTree<int> test(mycmp);
    for (int i = 0; i < 10; i++)
    {
        test.Insert(&a[i]);
    }
    printf("Max=%d\n", *(test.Max()));
    printf("Min=%d\n", *(test.Min()));
    test.Traverse(f);
    printf("\n");
    int *temp = test.FindForward(&a[6]);
    printf("2's forward is %d\n", *temp);
    temp = test.FindBackward(&a[2]);
    printf("7's backward is %d\n", *temp);
    printf("\nDeleting...\n");
    for (int i = 0; i < 10; i++)
    {
        test.Delete(&a[i]);
        test.Traverse(f);
        printf("\n");
    }
    return 0;
}
////////////////////////test//////////////////////
*/
/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "HuffmanTree.h"
* Function:  Data Structure Huffman Tree
*************************************************************/
#include "PriorityQueue.h"
#include "Queue.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#define MAXCODELEN 1024

struct HuffmanTreeNode
{
    char *code; //Huffman编码
    unsigned int weight;
    unsigned int layer; //记录节点所在层次，便于格式化打印
    struct HuffmanTreeNode *parent;
    struct HuffmanTreeNode *lchild;
    struct HuffmanTreeNode *rchild;
};

int mycompare(HuffmanTreeNode *const &a, HuffmanTreeNode *const &b)
{
    if (a->weight > b->weight)
        return 1;
    else if (a->weight == b->weight)
        return 0;
    else
        return -1;
}

struct HuffmanTree
{
private:
    struct HuffmanTreeNode *root;  //huffman树的根
    struct HuffmanTreeNode *HNode; //叶子节点
    int node_num;                  //叶子节点数目

public:
    void CreatHuffmanTree(int *weight, int len);
    void GenHuffmanCode();
    void PrintTree();
    void PrintCode();
};

void HuffmanTree::CreatHuffmanTree(int *weight, int len)
{
    //根据优先队列q构造以root为根的huffman树
    // weight:  权重数组
    // len:     权重数组的长度

    //HNode存储所有huffman叶子节点
    this->HNode = (HuffmanTreeNode *)malloc(sizeof(HuffmanTreeNode) * (len));
    this->node_num = len;
    memset(this->HNode, 0, sizeof(HuffmanTreeNode) * len);
    //构造优先队列
    Priority_Queue<HuffmanTreeNode *> q;
    q.init(len + 1, mycompare);
    for (int i = 0; i < len; i++)
    {
        this->HNode[i].weight = weight[i];
        q.enqueue(&(this->HNode[i]));
    }

    //根据优先队列构造huffman树
    while (q.length() > 1)
    {
        HuffmanTreeNode *temp = (HuffmanTreeNode *)malloc(sizeof(HuffmanTreeNode));
        if (temp == NULL)
            exit(-1);

        HuffmanTreeNode *t1;
        q.dequeue(t1); //删除第一个节点，作为新bt的左子节点
        t1->parent = temp;
        HuffmanTreeNode *t2;
        q.dequeue(t2); //删除第二个节点，作为新bt的右子节点
        t2->parent = temp;

        temp->lchild = t1;
        temp->rchild = t2;
        temp->weight = t1->weight + t2->weight; //计算最后权值
        temp->parent = NULL;
        q.enqueue(temp); //将根插入优先队列
    }

    //取出根结点
    q.dequeue(this->root);

    return;
}

void HuffmanTree::GenHuffmanCode()
{
    //根据HNode所在Huffman树的位置构造Huffman编码，存储在Code中

    if (this->node_num == 1)
    {
        this->HNode[0].code = (char *)malloc(2 * sizeof(char));
        if (this->HNode[0].code == NULL)
            exit(-1);
        this->HNode[0].code[0] = '0';
        this->HNode[0].code[1] = 0;
        return;
    }
    HuffmanTreeNode *currNode;
    HuffmanTreeNode *nextNode;
    char temp[MAXCODELEN];
    int start = MAXCODELEN - 1;
    //从每个叶子节点开始往前遍历
    for (int i = 0; i < this->node_num; i++)
    {
        temp[MAXCODELEN - 1] = 0;
        start = MAXCODELEN - 1;
        for (currNode = &(this->HNode[i]), nextNode = this->HNode[i].parent; nextNode != NULL; currNode = nextNode, nextNode = nextNode->parent)
        {
            if (nextNode->rchild == currNode)
                temp[--start] = '1';
            else
                temp[--start] = '0';
        }
        this->HNode[i].code = (char *)malloc((MAXCODELEN - start) * sizeof(char));
        if (this->HNode[i].code == NULL)
            exit(-1);
        strcpy(this->HNode[i].code, &temp[start]);
    }
    return;
}

void HuffmanTree::PrintTree()
{
    //借助队列实现树的层次遍历
    if (this->root == NULL)
        return;
    HuffmanTreeNode *temp = this->root;
    temp->layer = 1;

    //构造辅助队列
    Queue<HuffmanTreeNode *> q;
    q.init(this->node_num + 1);
    q.enqueue(temp);

    int curr_layer = 0;
    printf("HuffmanTree: ");

    //层次化遍历
    while (q.length() > 0)
    {
        q.dequeue(temp);
        if (curr_layer < temp->layer)
        {
            printf("\n%d ", temp->weight);
            curr_layer++;
        }
        else
            printf("%d ", temp->weight);

        //左孩子入队
        if (temp->lchild != NULL)
        {
            q.enqueue(temp->lchild);
            temp->lchild->layer = temp->layer + 1;
        }

        //右孩子入队
        if (temp->rchild != NULL)
        {
            q.enqueue(temp->rchild);
            temp->rchild->layer = temp->layer + 1;
        }
    }
    printf("\n");
    return;
}

void HuffmanTree::PrintCode()
{
    if (this->HNode == NULL)
        return;
    for (int i = 0; i < this->node_num; i++)
        printf("weight: %d\t\t HuffmanCode: %s\n", this->HNode[i].weight, this->HNode[i].code);
    return;
}

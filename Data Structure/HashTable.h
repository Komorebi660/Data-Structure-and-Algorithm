/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "HashTable.h"
* Function:  Data Structure Hash Table
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//链表节点，数据域存字符串
typedef struct Node
{
    char name[10];
    int count; //对相同元素计数
    struct Node *next;
} Node;

//哈希表
typedef struct HashTable
{
    //哈希表指针数组
    Node **data;
    //哈希表大小
    int size;
} HashTable;

//初始化哈希表
HashTable *init_hashtable(int n)
{
    HashTable *h = (HashTable *)malloc(sizeof(HashTable));
    h->size = n << 1; //若要使哈希表存储效率变高，存储空间扩大一倍
    h->data = (Node **)malloc(sizeof(Node *) * (h->size));
    for (int i = 0; i < h->size; i++)
        h->data[i] = NULL;
    return h;
}

//清空哈希表
void clear_hashtable(HashTable *h)
{
    if (h == NULL)
        return;
    for (int i = 0; i < h->size; i++)
    {
        if (h->data[i] == NULL)
            continue;
        Node *p = h->data[i];
        Node *q = p;
        while (p)
        {
            q = p->next;
            free(p);
            p = q;
        }
    }
    free(h->data);
    free(h);
    return;
}

//字符串哈希处理
int BKDRHash(char *str)
{
    int seed = 31;
    int hash = 0;
    int n = strlen(str);
    for (int i = 0; i < n; i++)
        hash = hash * seed + str[i];
    //保证正数
    return hash & 0x7fffffff;
}

//插入哈希表
bool insert(HashTable *h, char *str)
{
    int hash = BKDRHash(str);
    int index = hash % h->size;

    Node *temp = h->data[index];
    //查找是否已经存在相同节点
    while (temp && strcmp(str, temp->name))
        temp = temp->next;
    //若存在，则count+1，返回false
    if (temp != NULL)
    {
        temp->count++;
        return false;
    }
    //若不存在，则构造新节点，采用头插法，返回true
    else
    {
        //构造新节点
        Node *p = (Node *)malloc(sizeof(Node));
        strcpy(p->name, str);
        p->count = 1;
        p->next = h->data[index];
        //更新头节点
        h->data[index] = p;
        return true;
    }
}

bool remove(HashTable *h, char *str)
{
    int hash = BKDRHash(str);
    int index = hash % h->size;

    Node *p = h->data[index];
    Node *q = p;
    //查找该节点
    while (p && strcmp(str, p->name))
    {
        q = p;
        p = p->next;
    }
    //如果节点不存在，返回false
    if (p == NULL)
    {
        printf("can not find record.\n");
        return false;
    }

    //若存在超过一个，则count--，返回false
    if (p->count > 1)
    {
        p->count--;
        return false;
    }
    //若仅存在一个，则删除节点，返回true
    else
    {
        //如果是头节点
        if (h->data[index] == p)
        {
            h->data[index] = p->next;
            free(p);
        }
        else
        {
            q->next = p->next;
            free(p);
        }
        return true;
    }
}

/*
/////////////////////////////test/////////////////////////////
int main()
{
    char s[100][6] = {"hello", "abc", "123", "00", "hello"};
    HashTable *h = init_hashtable(100);
    if (insert(h, s[0]))
        printf("Insert new string.\n");
    else
        printf("Count++\n");
    if (insert(h, s[1]))
        printf("Insert new string.\n");
    else
        printf("Count++\n");
    if (insert(h, s[4]))
        printf("Insert new string.\n");
    else
        printf("Count++\n");
    if (insert(h, s[2]))
        printf("Insert new string.\n");
    else
        printf("Count++\n");
    if (remove(h, s[1]))
        printf("delete string.\n");
    else
        printf("Count--\n");
    if (remove(h, s[0]))
        printf("delete string.\n");
    else
        printf("Count--\n");
    if (remove(h, s[4]))
        printf("delete string.\n");
    else
        printf("Count--\n");

    return 0;
}
/////////////////////////////test/////////////////////////////
*/
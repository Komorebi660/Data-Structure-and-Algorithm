/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "PriorityQueue.h"
* Function:  Data Structure Priority Queue
*************************************************************/
#include <stdlib.h>
#include <assert.h>

template <typename T>
struct Priority_Queue
{
private:
    T *data;
    size_t rear;
    size_t capacity;
    int (*cmp)(const T &, const T &);

public:
    void init(size_t capacity, int (*cmp)(const T &, const T &));
    void destroy();
    bool enqueue(const T &e);
    bool dequeue(T &e);
    bool top(T &e);
    size_t length();
    bool empty();
    bool full();
};

template <typename T>
void Priority_Queue<T>::init(size_t capacity, int (*cmp)(const T &a, const T &b))
{
    //初始化，第一个元素不使用
    this->capacity = capacity;
    this->data = (T *)malloc((capacity + 1) * sizeof(T));
    this->rear = 0;
    this->cmp = cmp;
    assert(this->data != NULL);
}

template <typename T>
void Priority_Queue<T>::destroy()
{
    //销毁
    free(this->data);
    this->capacity = 0;
    this->rear = 0;
}

template <typename T>
bool Priority_Queue<T>::enqueue(const T &e)
{
    //元素入队，需要使用大顶堆的思想来调整
    if (full())
        return false;

    size_t i = ++this->rear;
    for (; i > 1 && this->cmp(this->data[i / 2], e) > 0; i /= 2)
    {
        this->data[i] = this->data[i / 2];
    }
    this->data[i] = e;
    return true;
}

template <typename T>
bool Priority_Queue<T>::dequeue(T &e)
{
    //元素出队，需要使用大顶堆的思想来调整
    if (empty())
        return false;

    e = this->data[1];
    this->rear--;

    int i = 2;
    for (; i <= this->rear; i *= 2)
    {
        if (i<this->rear &&this->cmp(this->data[i], this->data[i + 1])> 0)
            i++;

        if (i <= this->rear && this->cmp(this->data[this->rear + 1], this->data[i]) > 0)
            this->data[i / 2] = this->data[i];
        else
            break;
    }
    this->data[i / 2] = this->data[this->rear + 1];
    return true;
}

template <typename T>
bool Priority_Queue<T>::top(T &e)
{
    //查看堆顶元素
    if (empty())
        return false;

    e = this->data[1];
    return true;
}

template <typename T>
size_t Priority_Queue<T>::length()
{
    //返回队列长度
    return this->rear;
}

template <typename T>
bool Priority_Queue<T>::empty()
{
    //队列是否为空
    return length() == 0;
}

template <typename T>
bool Priority_Queue<T>::full()
{
    //队列是否已满
    return length() == this->capacity;
}

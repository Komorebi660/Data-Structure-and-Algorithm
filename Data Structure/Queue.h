/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "Queue.h"
* Function:  Data Structure Queue
*************************************************************/
#include <stdlib.h>

template <typename T>
struct Queue
{
private:
    T *data;
    size_t front;
    size_t rear;
    size_t capacity;

public:
    void init(size_t capacity);
    void destroy();
    bool enqueue(const T &e);
    bool dequeue(T &e);
    bool top(T &e);
    size_t length();
    bool empty();
    bool full();
};

template <typename T>
void Queue<T>::init(size_t capacity)
{
    //构造一个空队列
    this->data = new T[capacity + 1];
    if (!this->data)
        exit(-1);
    this->front = 0;
    this->rear = 0;
    this->capacity = capacity;
    return;
}

template <typename T>
void Queue<T>::destroy()
{
    //删除队列中分配的堆空间
    delete[](this->data);
    this->capacity = 0;
    this->front = 0;
    this->rear = 0;
    return;
}

template <typename T>
bool Queue<T>::enqueue(const T &e)
{
    //元素入队
    if ((this->rear + 1) % (this->capacity + 1) == this->front)
        return false;
    this->data[this->rear] = e;
    this->rear = (this->rear + 1) % (this->capacity + 1);
    return true;
}

template <typename T>
bool Queue<T>::dequeue(T &e)
{
    //元素出队
    if (this->front == this->rear)
        return false;
    e = this->data[this->front];
    this->front = (this->front + 1) % (this->capacity + 1);
    return true;
}

template <typename T>
bool Queue<T>::top(T &e)
{
    //获取队头元素
    if (this->front == this->rear)
        return false;
    else
    {
        e = this->data[this->front];
        return true;
    }
}

template <typename T>
size_t Queue<T>::length()
{
    //返回队列中元素个数
    return (this->rear - this->front + this->capacity + 1) % (this->capacity + 1);
}

template <typename T>
bool Queue<T>::empty()
{
    //判断队列是否为空
    if (this->front == this->rear)
        return true;
    else
        return false;
}

template <typename T>
bool Queue<T>::full()
{
    //判断队列是否已满
    if ((this->rear + 1) % (this->capacity + 1) == this->front)
        return true;
    else
        return false;
}

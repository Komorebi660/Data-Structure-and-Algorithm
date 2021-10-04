/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "Stack.h"
* Function:  Data Structure Stack
*************************************************************/
#include <stdlib.h>

template <typename T>
struct Stack
{
private:
    T *data;
    size_t top; //指向下一个空闲位
    size_t capacity;

public:
    void init(size_t capacity);
    void destroy();
    bool push(const T &e);
    bool pop(T &e);
    bool getop(T &e);
    size_t length();
    bool empty();
    bool full();
};

template <typename T>
void Stack<T>::init(size_t capacity)
{
    //构造一个空队列
    this->data = new T[capacity + 1];
    if (!this->data)
        exit(-1);
    this->top = 0;
    this->capacity = capacity;
    return;
}

template <typename T>
void Stack<T>::destroy()
{
    //删除栈中分配的堆空间
    delete[](this->data);
    this->capacity = 0;
    this->top = 0;
    return;
}

template <typename T>
bool Stack<T>::push(const T &e)
{
    //元素入栈
    if (full())
        return false;
    this->data[this->top] = e;
    this->top++;
    return true;
}

template <typename T>
bool Stack<T>::pop(T &e)
{
    //元素出栈
    if (empty())
        return false;
    e = this->data[--this->top];
    return true;
}

template <typename T>
bool Stack<T>::getop(T &e)
{
    //获取栈顶元素
    if (empty())
        return false;
    else
        e = this->data[this->top];
    return true;
}

template <typename T>
size_t Stack<T>::length()
{
    //返回栈中元素个数
    return (this->top);
}

template <typename T>
bool Stack<T>::empty()
{
    //判断栈是否为空
    if (this->top == 0)
        return true;
    else
        return false;
}

template <typename T>
bool Stack<T>::full()
{
    //判断栈是否已满
    if (this->top == this->capacity)
        return true;
    else
        return false;
}
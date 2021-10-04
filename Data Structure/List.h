/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "List.h"
* Function:  Data Structure List
*************************************************************/
#include <stdio.h>
#include <stdlib.h>

template <typename T>
struct List
{
private:
	struct ListNode
	{
		T data;
		struct ListNode *next;
	};
	struct ListNode *head; //链表头
	size_t curr_len;

public:
	void init();
	void destroy();
	bool insert(const T flag, T e, int (*cmp)(const T, const T));
	bool remove(int (*cmp)(const T));
	size_t length();
	bool get_first(T &e);
	bool get_next(const T flag, T &next, int (*cmp)(const T, const T));
	bool find(T &e, int (*cmp)(const T));
	void traverse(void (*func)(const T));
};

template <typename T>
void List<T>::init()
{
	//初始化一个线性链表
	this->head = NULL;
	this->curr_len = 0;
	return;
}

template <typename T>
void List<T>::destroy()
{
	//销毁线性链表
	struct ListNode *p = this->head;
	struct ListNode *q = p;
	while (q != NULL)
	{
		p = q;
		q = q->next;
		free(p);
	}
	this->curr_lens = 0;
	return;
}

template <typename T>
bool List<T>::insert(const T flag, T e, int (*cmp)(const T, const T))
{
	//依据cmp规则在flag之前插入e
	struct ListNode *p = this->head;
	struct ListNode *q = p;
	//初始情况,不管cmp结果均插入
	if (p == NULL)
	{
		struct ListNode *temp = (struct ListNode *)malloc(sizeof(ListNode));
		if (!temp)
			exit(0);
		temp->data = e;
		temp->next = NULL;
		this->head = temp;
		this->curr_len++;
		return true;
	}
	while (p != NULL)
	{
		//符合条件,在p前方插入
		if (cmp(flag, p->data))
		{
			if (p == this->head)
			{
				//头插
				struct ListNode *temp = (struct ListNode *)malloc(sizeof(ListNode));
				if (!temp)
					exit(0);
				temp->data = e;
				temp->next = this->head;
				this->head = temp;
				this->curr_len++;
				return true;
			}
			else
			{
				struct ListNode *temp = (struct ListNode *)malloc(sizeof(ListNode));
				if (!temp)
					exit(0);
				temp->data = e;
				temp->next = q->next;
				q->next = temp;
				this->curr_len++;
				return true;
			}
		}
		else
		{
			q = p;
			p = p->next;
		}
	}
	return false;
}

template <typename T>
bool List<T>::remove(int (*cmp)(const T))
{
	//依据cmp规则删除元素e
	struct ListNode *p = this->head;
	struct ListNode *q = p;
	while (p != NULL)
	{
		if (cmp(p->data))
		{
			//链表头部特殊处理
			if (p == this->head)
			{
				this->head = p->next;
				free(p);
				this->curr_len--;
				return true;
			}
			else
			{
				q->next = p->next;
				free(p);
				this->curr_len--;
				return true;
			}
		}
		else
		{
			q = p;
			p = p->next;
		}
	}
	return false;
}

template <typename T>
size_t List<T>::length()
{
	//返回队列中元素个数
	return (this->curr_len);
}

template <typename T>
bool List<T>::get_first(T &e)
{
	//获取链表首元素
	if (this->head == NULL)
		return false;
	else
		e = this->head->data;
	return true;
}

template <typename T>
bool List<T>::get_next(const T flag, T &next, int (*cmp)(const T, const T))
{
	//依据cmp规则获取flag的下一个元素
	struct ListNode *p = this->head;
	while (p != NULL)
	{
		if (cmp(flag, p->data))
		{
			p = p->next;
			if (p == NULL)
				next = 0;
			else
				next = p->data;
			return true;
		}
		else
			p = p->next;
	}
	return false;
}

template <typename T>
bool List<T>::find(T &e, int (*cmp)(const T))
{
	//依据func查询满足条件的元素，返回给e
	struct ListNode *p = this->head;
	while (p != NULL)
	{
		if (cmp(p->data))
		{
			e = p->data;
			return true;
		}
		else
			p = p->next;
	}
	return false;
}

template <typename T>
void List<T>::traverse(void (*func)(const T))
{
	//遍历链表
	struct ListNode *p = this->head;
	while (p != NULL)
	{
		func(p->data);
		p = p->next;
	}
	return;
}
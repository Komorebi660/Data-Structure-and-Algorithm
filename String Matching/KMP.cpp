/*************************************************************
 * Copyright © 2022 Komorebi660 All rights reserved.
 * File Name: "KMP.h"
 * Function: KMP String Mtching Algorithm
 *************************************************************/
#include <stdio.h>
#include <string.h>

/*Knuth-Pratt-Morris 字符串模式匹配算法实现*/
char s[1000005];
char pattern[10005];
int next[10005];

//获得next[]数组
void get_next()
{
    //0号元素没有使用
    int n = strlen(pattern + 1);
    memset(next, 0, sizeof(next));
    next[1] = 0;
    int j = 0;
    for (int i = 2; i <= n; i++)
    {
        //向前迭代，直到找到匹配位置
        while (j > 0 && pattern[j + 1] != pattern[i])
            j = next[j];
        if (pattern[j + 1] == pattern[i])
            j++;
        next[i] = j;
    }
    return;
}

//模式串pattern在主串s中匹配的位置/次数
void string_matching()
{
    //0号元素没有使用
    int n = strlen(s + 1);
    int m = strlen(pattern + 1);
    get_next();
    int pos = 0;
    int result = 0;

    for (int i = 1; i <= n; i++)
    {
        //find next position
        while (pos > 0 && pattern[pos + 1] != s[i])
            pos = next[pos];
        //match
        if (pattern[pos + 1] == s[i])
            pos++;

        //find a substring matches the pattern
        if (pos == m)
        {
            result++;
            pos = next[pos];
        }
    }
    printf("%d\n", result);
    return;
}

int main()
{
    int T = 0;
    scanf("%d", &T);
    getchar();
    for (int i = 0; i < T; i++)
    {
        memset(pattern, 0, sizeof(pattern));
        memset(s, 0, sizeof(s));

        gets(pattern + 1);
        gets(s + 1);

        string_matching();
    }
    return 0;
}

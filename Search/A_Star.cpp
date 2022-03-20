#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
using namespace std;

class Node
{
public:
    int g;       // 过往的代价
    int h;       // 估计将来的代价(启发式函数)
    int f;       // g+h
    int a[3][3]; //该节点的状态，依据问题不同而不同
    //初始化节点
    Node(int _g, int _a[3][3])
    {
        memcpy(a, _a, sizeof(int) * 3 * 3);
        this->g = _g;
        this->h = _h(this);
        this->f = this->g + this->h;
    }
    //为优先队列排序重载运算符
    bool operator<(const Node &n) const
    {
        return this->f > n.f;
    }

private:
    //启发式函数
    int _h(Node *n)
    {
        /*
         *   goal_state:
         *   1   2   3
         *   8       4
         *   7   6   5
         */
        int tmp = 0;
        // h1(错位的数字数目)
        /*
        if (a[0][0] != 1)
            tmp += 1;
        if (a[0][1] != 2)
            tmp += 1;
        if (a[0][2] != 3)
            tmp += 1;
        if (a[1][2] != 4)
            tmp += 1;
        if (a[2][2] != 5)
            tmp += 1;
        if (a[2][1] != 6)
            tmp += 1;
        if (a[2][0] != 7)
            tmp += 1;
        if (a[1][0] != 8)
            tmp += 1;
        */
        // h2(曼哈顿距离)
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (a[i][j] == 1)
                    tmp += (fabs(i - 0) + fabs(j - 0));
                if (a[i][j] == 2)
                    tmp += (fabs(i - 0) + fabs(j - 1));
                if (a[i][j] == 3)
                    tmp += (fabs(i - 0) + fabs(j - 2));
                if (a[i][j] == 4)
                    tmp += (fabs(i - 1) + fabs(j - 2));
                if (a[i][j] == 5)
                    tmp += (fabs(i - 2) + fabs(j - 2));
                if (a[i][j] == 6)
                    tmp += (fabs(i - 2) + fabs(j - 1));
                if (a[i][j] == 7)
                    tmp += (fabs(i - 2) + fabs(j - 0));
                if (a[i][j] == 8)
                    tmp += (fabs(i - 1) + fabs(j - 0));
            }
        }
        return tmp;
    }
};

void A_star_Search(int initial_state[3][3], int goal_state[3][3])
{
    Node init(0, initial_state);
    priority_queue<Node> open_list;
    open_list.push(init);

    while (!open_list.empty())
    {
        //从优先队列中弹出f最小的状态
        Node current_state = open_list.top();
        open_list.pop();

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf("%d\t", current_state.a[i][j]);
            }
            printf("\n");
        }
        printf("f=%d+%d=%d\n\n", current_state.g, current_state.h, current_state.f);

        //检查该状态是否符合要求
        bool same = true;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (current_state.a[i][j] != goal_state[i][j])
                {
                    same = false;
                    break;
                }
            }
            if (!same)
                break;
        }
        //找到结果
        if (same)
            return;

        //扩展结点
        bool movable = false;
        int i = 0, j = 0;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                //寻找没有存放数字的位置
                if (current_state.a[i][j] == 0)
                {
                    movable = true;
                    break;
                }
            }
            if (movable)
                break;
        }
        //(i-1,j)--(i,j)
        if (i - 1 >= 0)
        {
            int tmp[3][3];
            memcpy(tmp, current_state.a, sizeof(int) * 3 * 3);
            tmp[i][j] = tmp[i - 1][j];
            tmp[i - 1][j] = 0;
            Node tmp_node(current_state.g + 1, tmp);
            open_list.push(tmp_node);
        }
        //(i+1,j)--(i,j)
        if (i + 1 < 3)
        {
            int tmp[3][3];
            memcpy(tmp, current_state.a, sizeof(int) * 3 * 3);
            tmp[i][j] = tmp[i + 1][j];
            tmp[i + 1][j] = 0;
            Node tmp_node(current_state.g + 1, tmp);
            open_list.push(tmp_node);
        }
        //(i,j-1)--(i,j)
        if (j - 1 >= 0)
        {
            int tmp[3][3];
            memcpy(tmp, current_state.a, sizeof(int) * 3 * 3);
            tmp[i][j] = tmp[i][j - 1];
            tmp[i][j - 1] = 0;
            Node tmp_node(current_state.g + 1, tmp);
            open_list.push(tmp_node);
        }
        //(i,j+1)--(i,j)
        if (j + 1 < 3)
        {
            int tmp[3][3];
            memcpy(tmp, current_state.a, sizeof(int) * 3 * 3);
            tmp[i][j] = tmp[i][j + 1];
            tmp[i][j + 1] = 0;
            Node tmp_node(current_state.g + 1, tmp);
            open_list.push(tmp_node);
        }
    }
}

int main()
{
    int init[3][3] = {{2, 8, 3}, {1, 6, 4}, {7, 5, 0}};
    int goal[3][3] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};
    A_star_Search(init, goal);
    return 0;
}
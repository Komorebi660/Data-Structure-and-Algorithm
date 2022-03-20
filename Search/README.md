## Informated-Search算法

对于较为复杂的问题，搜索空间过于庞大，传统的无信息搜索`BFS`、`DFS`已经无法满足工程上的需要，这时就需要引入问题的**先验知识**来帮助我们减少无用搜索。其中，最为重要的`Informated-Search`算法当属`A*`算法，其基本思想结合了一致代价搜索和贪心搜索，借助**已知路径的代价(g)** 和 **启发式函数估计的代价(h)** 对待访问节点排序，选择`cost`最小的节点访问。

```
A* Search(init, goal)
{
    open_list={init};       //初始化优先队列

    while(open_list!=empty)
    {
        //弹出代价最小的节点
        current=open_list.pop();

        //到达目标，结束
        if(current==goal)
            return current;
        
        //扩展结点
        for each successor node of current
        {
            Calcualte cost with g and h
            open_list.push(node)
        }
    }
}
```

`open_list`的具体实现可以采用**优先队列**；为了避免重复节点访问，可以对已访问节点做标记(此时对启发式函数要求较高)。

[A_Star.cpp](./A_Star.cpp)针对**8数码问题**构造启发式函数并采用`A*`算法求解。
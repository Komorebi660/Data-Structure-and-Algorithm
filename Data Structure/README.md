## 常用数据结构合集

- [B+树](./BPlusTree.h)

`B+`树是一种树数据结构，通常用于数据库和操作系统的**文件系统**中。`B+`树的特点是能够保持数据**稳定有序**，其插入与修改拥有较稳定的**对数时间复杂度**。

- [Huffman树](./HuffmanTree.h)

给定`N`个权值作为`N`个叶子结点，构造一棵二叉树，若该树的带权路径长度达到最小，称这样的二叉树为**最优二叉树**，也称为哈夫曼树(`Huffman Tree`)。哈夫曼树是**带权路径长度最短的树**，可用于**计算机编码**。

- [二叉搜索树](./BinarySearchTree.h)

二叉搜索树(`Binary Search Tree`)，它或者是一棵空树，或者是具有下列性质的二叉树：若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值；若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值；它的左、右子树也分别为二叉搜索树。二叉搜索树作为一种经典的数据结构，它既有链表的快速插入与删除操作的特点，又有数组快速查找的优势；所以应用十分广泛，例如在文件系统和数据库系统一般会采用这种数据结构进行**高效率的排序与检索操作**。

- [红黑树](./RedBlackTree.h)

红黑树(`Red–black tree`)是一种**自平衡二叉查找树**，是在计算机科学中用到的一种数据结构，典型用途是实现关联数组。它在`1972`年由鲁道夫·贝尔发明，被称为"对称二叉B树"，它现代的名字源于`Leo J. Guibas`和罗伯特·塞奇威克于`1978`年写的一篇论文。红黑树的结构复杂，但它的操作有着良好的最坏情况运行时间，并且在实践中高效：它可以在**任何时候**以`O(nlogn)`时间内完成查找、插入和删除。

- [线性链表](./List.h)

具有链接存储结构的线性表，它用一组地址任意的存储单元存放线性表中的数据元素，逻辑上相邻的元素在物理上**不要求也相邻**，**不能随机存取**。

- [栈](./Stack.h)

栈(`stack`)又名堆栈，它是一种运算受限的线性表。限定**仅在表尾进行插入和删除操作**的线性表。这一端被称为栈顶，相对地，把另一端称为栈底。向一个栈插入新元素又称作进栈、入栈或压栈，它是把新元素放到栈顶元素的上面，使之成为新的栈顶元素；从一个栈删除元素又称作出栈或退栈，它是把栈顶元素删除掉，使其相邻的元素成为新的栈顶元素。

- [队列](./Queue.h)

队列是一种特殊的线性表，特殊之处在于它只允许在表的**前端(`front`)进行删除**操作，而在表的**后端(`rear`)进行插入**操作。

- [优先队列](./PriorityQueue.h)

优先队列本身也是一个队列，但是和普通队列又有不同之处。在优先队列中，元素被赋予优先级。当访问元素时，具有**最高优先级的元素最先删除**，所以优先队列具有最高级先出(`first in, largest out`)的行为特征，通常采用堆数据结构来实现。

- [哈希表](./HashTable.h)

散列表(`Hash table`，也叫哈希表)，是根据关键码值(`Key value`)而**直接**进行访问的数据结构。也就是说，它通过把关键码值映射到表中一个位置来访问记录，以加快查找的速度。这个映射函数叫做散列函数，存放记录的数组叫做散列表。

- [并查集](./DisjointSet.h)

并查集(`DisjointSet`)是一种用于处理一些不交集(`Disjoint sets`，一系列没有重复元素的集合)的合并及查询问题。并查集支持查询、合并等操作，是效率最高的常见数据结构之一，在`Kruskal`算法中具有重要的应用价值。
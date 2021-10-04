/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "BPlusTree_Test.cpp"
* Function:  Test the correction of "BPlusTree.h"
*************************************************************/
#include <iostream>
#include "BPlusTree.h"

struct Test
{
    char s[100];
    int age;
};

void func(struct Test *t)
{
    printf("name: %s\t\tage: %d\n", t->s, t->age);
    return;
}

int main()
{
    struct Test t[10] = {{"Alice", 10}, {"Jay", 20}, {"Enn", 12}, {"Bob", 15}, {"Tom", 18}, {"Candy", 30}, {"John", 25}, {"Mike", 12}, {"Toms", 18}};
    struct Test *temp;
    struct BPlusTree<Test> Tree;
    Tree.init();
    Tree.insert(t[0].s, &t[0]);
    Tree.insert(t[1].s, &t[1]);
    Tree.remove(t[0].s);
    Tree.remove(t[1].s);
    if (Tree.is_empty())
        printf("The tree is empty now!\n");
    else
        Tree.traverse(func);
    Tree.insert(t[1].s, &t[1]);
    Tree.insert(t[2].s, &t[2]);
    Tree.insert(t[3].s, &t[3]);
    Tree.insert(t[4].s, &t[4]);
    temp = Tree.find("Jay");
    if (temp == NULL)
        printf("can not find \"Jay\"!\n");
    else
        printf("Jay's age is %d\n", temp->age);
    temp = Tree.find("Alice");
    if (temp == NULL)
        printf("can not find \"Alice\"!\n");
    else
        printf("Alice's age is %d\n", temp->age);
    printf("Before delete:\n");
    Tree.insert(t[0].s, &t[0]);
    Tree.insert(t[1].s, &t[1]);
    Tree.insert(t[5].s, &t[5]);
    Tree.insert(t[6].s, &t[6]);
    Tree.insert(t[7].s, &t[7]);
    Tree.insert(t[8].s, &t[8]);
    Tree.traverse(func);
    printf("After delete:\n");
    Tree.remove(t[6].s);
    Tree.remove(t[5].s);
    Tree.remove(t[0].s);
    Tree.traverse(func);
    temp = Tree.get_first();
    if (temp == NULL)
        printf("The tree is empty!\n");
    else
        printf("The first people is \"%s\" and his/her age is %d\n", temp->s, temp->age);

    temp = Tree.get_next(t[1].s);
    if (temp == NULL)
        printf("There is no people after \"Jay\"!\n");
    else
        printf("The next people of \"Jay\" is \"%s\" and his/her age is %d\n", temp->s, temp->age);

    Tree.destroy();

    if (Tree.is_empty())
        printf("The tree has been destroyed successfully!\n");
    else
        Tree.traverse(func);
    return 0;
}
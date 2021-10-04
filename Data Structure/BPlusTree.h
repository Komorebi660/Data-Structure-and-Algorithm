/*************************************************************
* Copyright © 2021 Komorebi660 All rights reserved.
* File Name: "BPlusTree.h"
* Function:  Data Structure B+ Tree
*************************************************************/
#include <stdlib.h>
#include <string.h>
#define MAX_KEY_LEN 256 //max len of the keys string
#define MAX_KEY_NUM 2   //max number of keys of a node

//every node structure
typedef struct node
{
    void **pointers;     //the pointers to next layer, used as *next
    char **keys;         //each key is a string pointer
    struct node *parent; //point to its parent
    int num_keys;        //the number of keys, number of pointers = num_keys + 1
                         //for leaf node, number of pointers = num_keys
    bool is_leaf;        //whether is a leaf
} node;

//B+ Tree Class
template <typename T>
struct BPlusTree
{
private:
    //leaf node's record
    typedef T record;
    //the root of BPlusTree
    node *root;

    record *new_record(T *data);
    node *insert_into_leaf_after_splitting(node *root, node *leaf, int index, const char *key, record *rec);
    node *insert_into_parent(node *root, node *left, node *right, const char *key);
    node *insert_into_node_after_splitting(node *root, node *nd, node *right, int index, const char *key);
    node *delete_entry(node *root, node *nd, int index);
    node *adjust_root(node *root);
    void remove_entry(node *nd, int index);
    void distribute_nodes(node *nd, node *neighbor, int nd_index);
    node *coalesce_nodes(node *root, node *nd, node *neighbor, int nd_index);
    node *make_new_node(void);
    node *make_new_leaf(void);
    node *find_leaf(node *root, const char *key);
    int get_node_index(node *nd);

public:
    void init();
    void destroy();
    bool is_empty();
    record *get_first();
    record *get_next(const char *key);
    record *find(const char *key);
    void insert(const char *key, T *data);
    void remove(const char *key);
    void traverse(void (*fn)(T *data));
};

//init a new B+ tree
template <typename T>
void BPlusTree<T>::init()
{
    this->root = make_new_leaf();
    return;
}

template <typename T>
node *BPlusTree<T>::make_new_node(void)
{
    node *nd;
    nd = (node *)malloc(sizeof(node));
    nd->pointers = (void **)malloc((MAX_KEY_NUM + 1) * sizeof(void *));
    memset(nd->pointers, 0, (MAX_KEY_NUM + 1) * sizeof(void *));
    nd->keys = (char **)malloc(MAX_KEY_NUM * sizeof(char *));
    memset(nd->keys, 0, MAX_KEY_NUM * sizeof(char *));

    nd->parent = NULL;
    nd->num_keys = 0;
    nd->is_leaf = false;
    return nd;
}

template <typename T>
node *BPlusTree<T>::make_new_leaf(void)
{
    node *leaf;
    leaf = make_new_node();
    leaf->is_leaf = true;
    return leaf;
}

//destroy the B+ tree
template <typename T>
void BPlusTree<T>::destroy()
{
    node *p = this->root;
    while (p != NULL)
    {
        //find leaf
        while (!p->is_leaf)
            p = (node *)p->pointers[0];
        //the tree is empty
        if (p->num_keys == 0)
            break;
        BPlusTree<T>::remove(p->keys[0]);
        p = this->root;
    }
    return;
}

//find record that matches key
template <typename T>
typename BPlusTree<T>::record *BPlusTree<T>::find(const char *key)
{
    node *root = this->root;
    //firstly find the leaf node that contains the record matches the key
    node *leaf = find_leaf(root, key);
    //no leaf node
    if (leaf == NULL)
        return NULL;
    int i;
    //find the pointer of record
    for (i = 0; i < leaf->num_keys && strcmp(leaf->keys[i], key) != 0; i++)
        ;
    //no matched record
    if (i == leaf->num_keys)
        return NULL;

    return (record *)leaf->pointers[i];
}

template <typename T>
node *BPlusTree<T>::find_leaf(node *root, const char *key)
{
    node *temp = root;
    int i = 0;
    if (root != NULL)
    {
        while (!temp->is_leaf)
        {
            //find the next pointer
            for (i = 0; i < temp->num_keys && strcmp(temp->keys[i], key) <= 0; i++)
                ;
            temp = (node *)temp->pointers[i];
        }
    }
    return temp;
}

//get the first record of the leaves list
template <typename T>
typename BPlusTree<T>::record *BPlusTree<T>::get_first()
{
    node *p = this->root;
    if (p == NULL)
        return NULL;
    while (!p->is_leaf)
        p = (node *)p->pointers[0];
    return (record *)p->pointers[0];
}

//get the next record after "data"
template <typename T>
typename BPlusTree<T>::record *BPlusTree<T>::get_next(const char *key)
{
    node *p = this->root;
    int find = 0; //flag
    if (p == NULL)
        return NULL;
    while (!p->is_leaf)
        p = (node *)p->pointers[0];
    while (p)
    {
        for (int i = 0; i < p->num_keys; i++)
        {
            if (find == 1)
                return (record *)p->pointers[i];
            if (strcmp(p->keys[i], key) == 0)
                find = 1;
        }
        p = (node *)p->pointers[MAX_KEY_NUM];
    }
    return NULL;
}

//whether the tree is empty
template <typename T>
bool BPlusTree<T>::is_empty()
{
    node *root = this->root;
    if (root == NULL)
        return true;
    else
        return false;
}

//insert a record
template <typename T>
void BPlusTree<T>::insert(const char *key, T *data)
{
    node *root = this->root;
    record *rec = new_record(data);
    node *leaf = find_leaf(root, key);
    int index, cond;

    if (!leaf) // cannot find the leaf, the tree is empty
    {

        node *temp = make_new_leaf();
        temp->pointers[0] = rec;
        temp->keys[0] = (char *)malloc(MAX_KEY_LEN);
        strcpy(temp->keys[0], key);
        temp->num_keys++;
        this->root = temp;
        return;
    }

    //find a place to insert
    for (index = 0; index < leaf->num_keys && (cond = strcmp(leaf->keys[index], key)) < 0; index++)
        ;

    if (cond == 0) // already have a record that has the same key
        return;

    if (leaf->num_keys < MAX_KEY_NUM)
    {
        //insert into leaf node
        for (int i = leaf->num_keys; i > index; i--)
        {
            leaf->keys[i] = leaf->keys[i - 1];
            leaf->pointers[i] = leaf->pointers[i - 1];
        }
        leaf->keys[index] = (char *)malloc(MAX_KEY_LEN);
        strcpy(leaf->keys[index], key);
        leaf->pointers[index] = rec;
        leaf->num_keys++;

        return; // the root remains unchanged
    }
    this->root = insert_into_leaf_after_splitting(root, leaf, index, key, rec);
    return;
}

template <typename T>
typename BPlusTree<T>::record *BPlusTree<T>::new_record(T *data)
{
    record *rec;
    rec = (record *)malloc(sizeof(record));
    memcpy(rec, data, sizeof(T));
    return rec;
}

template <typename T>
node *BPlusTree<T>::insert_into_leaf_after_splitting(node *root, node *leaf, int index, const char *key, record *rec)
{
    node *new_leaf;
    record **temp_ps;
    char **temp_ks, *new_key;
    int i, split;

    temp_ps = (record **)malloc((MAX_KEY_NUM + 1) * sizeof(record *));
    temp_ks = (char **)malloc((MAX_KEY_NUM + 1) * sizeof(char *));
    for (i = 0; i < (MAX_KEY_NUM + 1); i++)
    {
        if (i == index)
        {
            temp_ps[i] = rec;
            temp_ks[i] = (char *)malloc(MAX_KEY_LEN);
            strcpy(temp_ks[i], key);
        }
        else if (i < index)
        {
            temp_ps[i] = (record *)leaf->pointers[i];
            temp_ks[i] = leaf->keys[i];
        }
        else
        {
            temp_ps[i] = (record *)leaf->pointers[i - 1];
            temp_ks[i] = leaf->keys[i - 1];
        }
    }

    split = (MAX_KEY_NUM + 1) / 2;
    leaf->num_keys = split;
    for (i = 0; i < split; i++)
    {
        leaf->pointers[i] = temp_ps[i];
        leaf->keys[i] = temp_ks[i];
    }

    new_leaf = make_new_leaf();
    new_leaf->num_keys = MAX_KEY_NUM + 1 - split;
    for (; i < MAX_KEY_NUM + 1; i++)
    {
        new_leaf->pointers[i - split] = temp_ps[i];
        new_leaf->keys[i - split] = temp_ks[i];
    }

    new_leaf->parent = leaf->parent;
    //next pointer of leaf node
    new_leaf->pointers[MAX_KEY_NUM] = leaf->pointers[MAX_KEY_NUM];
    leaf->pointers[MAX_KEY_NUM] = new_leaf;
    new_key = new_leaf->keys[0];
    free(temp_ps);
    free(temp_ks);

    return insert_into_parent(root, leaf, new_leaf, new_key);
}

template <typename T>
node *BPlusTree<T>::insert_into_parent(node *root, node *left, node *right, const char *key)
{
    node *parent = left->parent;
    int index;

    if (parent == NULL)
    {
        node *temp = make_new_node();
        temp->pointers[0] = left;
        temp->pointers[1] = right;
        temp->keys[0] = (char *)malloc(MAX_KEY_LEN);
        strcpy(temp->keys[0], key);
        temp->num_keys++;
        left->parent = temp;
        right->parent = temp;
        return temp;
    }

    for (index = 0; index < parent->num_keys && parent->pointers[index] != left; index++)
        ;

    if (parent->num_keys < MAX_KEY_NUM)
    {
        int i;
        for (i = parent->num_keys; i > index; i--)
        {
            parent->keys[i] = parent->keys[i - 1];
            parent->pointers[i + 1] = parent->pointers[i];
        }

        parent->keys[index] = (char *)malloc(MAX_KEY_LEN);
        strcpy(parent->keys[index], key);
        parent->pointers[index + 1] = right;
        parent->num_keys++;

        return root; // the root remains unchanged
    }
    return insert_into_node_after_splitting(root, parent, right, index, key);
}

template <typename T>
node *BPlusTree<T>::insert_into_node_after_splitting(node *root, node *nd, node *right, int index, const char *key)
{
    int i, split;
    node **temp_ps, *new_nd, *child;
    char **temp_ks, *new_key;
    temp_ps = (node **)malloc((MAX_KEY_NUM + 2) * sizeof(node *));
    temp_ks = (char **)malloc((MAX_KEY_NUM + 1) * sizeof(char *));

    for (i = 0; i < MAX_KEY_NUM + 2; i++)
    {
        if (i == index + 1)
            temp_ps[i] = right;
        else if (i < index + 1)
            temp_ps[i] = (node *)nd->pointers[i];
        else
            temp_ps[i] = (node *)nd->pointers[i - 1];
    }
    for (i = 0; i < MAX_KEY_NUM + 1; i++)
    {
        if (i == index)
        {
            temp_ks[i] = (char *)malloc(MAX_KEY_LEN);
            strcpy(temp_ks[i], key);
        }
        else if (i < index)
            temp_ks[i] = nd->keys[i];
        else
            temp_ks[i] = nd->keys[i - 1];
    }

    split = (MAX_KEY_NUM + 1) % 2 ? (MAX_KEY_NUM + 1) / 2 + 1 : (MAX_KEY_NUM + 1) / 2; // split is #pointers
    nd->num_keys = split - 1;
    for (i = 0; i < split - 1; i++)
    {
        nd->pointers[i] = temp_ps[i];
        nd->keys[i] = temp_ks[i];
    }
    nd->pointers[i] = temp_ps[i]; // i == split - 1
    new_key = temp_ks[split - 1];

    new_nd = make_new_node();
    new_nd->num_keys = MAX_KEY_NUM + 1 - split;
    for (++i; i < MAX_KEY_NUM + 1; i++)
    {
        new_nd->pointers[i - split] = temp_ps[i];
        new_nd->keys[i - split] = temp_ks[i];
    }
    new_nd->pointers[i - split] = temp_ps[i];
    new_nd->parent = nd->parent;
    for (i = 0; i <= new_nd->num_keys; i++)
    { //  #pointers == num_keys + 1
        child = (node *)(new_nd->pointers[i]);
        child->parent = new_nd;
    }

    free(temp_ps);
    free(temp_ks);
    return insert_into_parent(root, nd, new_nd, new_key);
}

//remove a record matches the key
template <typename T>
void BPlusTree<T>::remove(const char *key)
{
    node *root = this->root;
    node *leaf;
    int i;
    leaf = find_leaf(root, key);

    if (leaf == NULL)
        return;

    for (i = 0; i < leaf->num_keys && strcmp(leaf->keys[i], key) != 0; i++)
        ;

    if (i == leaf->num_keys) // no such key
        return;

    this->root = delete_entry(root, leaf, i);
    return;
}

template <typename T>
int BPlusTree<T>::get_node_index(node *nd)
{
    node *parent;
    int i;
    parent = nd->parent;
    for (i = 0; i < parent->num_keys && parent->pointers[i] != nd; i++)
        ;
    return i;
}

template <typename T>
node *BPlusTree<T>::delete_entry(node *root, node *nd, int index)
{
    int min_keys, cap, nd_index;
    node *neighbor;

    remove_entry(nd, index);
    if (nd == root)
        return adjust_root(nd);
    min_keys = nd->is_leaf ? (MAX_KEY_NUM + 1) / 2 : MAX_KEY_NUM / 2;
    if (nd->num_keys >= min_keys)
    {
        return root;
    }

    nd_index = get_node_index(nd);
    if (nd_index == 0)
        neighbor = (node *)nd->parent->pointers[1]; // right neighbor
    else
        neighbor = (node *)nd->parent->pointers[nd_index - 1]; // left neighbor

    cap = nd->is_leaf ? MAX_KEY_NUM : MAX_KEY_NUM - 1;
    if (neighbor->num_keys + nd->num_keys <= cap)
        return coalesce_nodes(root, nd, neighbor, nd_index);

    distribute_nodes(nd, neighbor, nd_index);
    return root;
}

template <typename T>
node *BPlusTree<T>::adjust_root(node *root)
{
    node *new_root;
    if (root->num_keys > 0) // at least two childs
        return root;
    if (!root->is_leaf)
    {
        // root has only one child
        new_root = (node *)root->pointers[0];
        new_root->parent = NULL;
    }
    else
        new_root = NULL;

    //destroy old node
    free(root->keys);
    free(root->pointers);
    free(root);

    return new_root;
}

template <typename T>
void BPlusTree<T>::remove_entry(node *nd, int index)
{
    int i, index_k;

    if (nd->is_leaf)
    {
        free(nd->keys[index]);
        free(nd->pointers[index]); // destroy the record
        for (i = index; i < nd->num_keys - 1; i++)
        {
            nd->keys[i] = nd->keys[i + 1];
            nd->pointers[i] = nd->pointers[i + 1];
        }
        nd->keys[i] = NULL;
        nd->pointers[i] = NULL;
    }
    else
    {
        index_k = index - 1; // index_p == index
        free(nd->keys[index_k]);
        for (i = index_k; i < nd->num_keys - 1; i++)
        {
            nd->keys[i] = nd->keys[i + 1];
            nd->pointers[i + 1] = nd->pointers[i + 2];
        }
        nd->keys[i] = NULL;
        nd->pointers[i + 1] = NULL;
    }
    nd->num_keys--;
}

template <typename T>
void BPlusTree<T>::distribute_nodes(node *nd, node *neighbor, int nd_index)
{
    int i;
    node *tmp;
    if (nd_index != 0)
    {
        if (!nd->is_leaf)
            nd->pointers[nd->num_keys + 1] = nd->pointers[nd->num_keys];
        for (i = nd->num_keys; i > 0; i--)
        { // shift to right by 1
            nd->keys[i] = nd->keys[i - 1];
            nd->pointers[i] = nd->pointers[i - 1];
        }
        if (!nd->is_leaf)
        {
            nd->keys[0] = nd->parent->keys[nd_index - 1];

            nd->pointers[0] = neighbor->pointers[neighbor->num_keys];
            tmp = (node *)nd->pointers[0];
            tmp->parent = nd;
            neighbor->pointers[neighbor->num_keys] = NULL;

            nd->parent->keys[nd_index - 1] = neighbor->keys[neighbor->num_keys - 1];
            neighbor->keys[neighbor->num_keys - 1] = NULL;
        }
        else
        {
            nd->keys[0] = neighbor->keys[neighbor->num_keys - 1];
            neighbor->keys[neighbor->num_keys - 1] = NULL;

            nd->pointers[0] = neighbor->pointers[neighbor->num_keys - 1];
            neighbor->pointers[neighbor->num_keys - 1] = NULL;

            // nd->parent->keys[nd_index - 1] = nd->keys[0];  //  share the same key with child !!
            strcpy(nd->parent->keys[nd_index - 1], nd->keys[0]);
        }
    }
    else
    {
        if (!nd->is_leaf)
        {
            nd->keys[nd->num_keys] = nd->parent->keys[0]; // link to father's key
            nd->pointers[nd->num_keys + 1] = neighbor->pointers[0];
            tmp = (node *)nd->pointers[nd->num_keys + 1];
            tmp->parent = nd;
            nd->parent->keys[0] = neighbor->keys[0]; //
        }
        else
        {
            nd->keys[nd->num_keys] = neighbor->keys[0];
            nd->pointers[nd->num_keys] = neighbor->pointers[0];
            // nd->parent->keys[0] = neighbor->keys[1];  // share the same key with chid !!
            strcpy(nd->parent->keys[0], neighbor->keys[1]);
        }
        for (i = 0; i < neighbor->num_keys - 1; i++)
        {
            neighbor->keys[i] = neighbor->keys[i + 1];
            neighbor->pointers[i] = neighbor->pointers[i + 1];
        }
        neighbor->keys[i] = NULL;
        if (!nd->is_leaf)
            neighbor->pointers[i] = neighbor->pointers[i + 1];
        else
            neighbor->pointers[i] = NULL;
    }

    neighbor->num_keys--;
    nd->num_keys++;
}

template <typename T>
node *BPlusTree<T>::coalesce_nodes(node *root, node *nd, node *neighbor, int nd_index)
{
    int i, j, start;
    node *tmp, *parent;

    if (nd_index == 0)
    { // make sure neighbor is on the left
        tmp = nd;
        nd = neighbor;
        neighbor = tmp;
        nd_index = 1;
    }
    parent = nd->parent;

    start = neighbor->num_keys;
    if (nd->is_leaf)
    {
        for (i = start, j = 0; j < nd->num_keys; i++, j++)
        {
            neighbor->keys[i] = nd->keys[j];
            neighbor->pointers[i] = nd->pointers[j];
            nd->keys[j] = NULL;
            nd->pointers[j] = NULL;
        }
        neighbor->num_keys += nd->num_keys;
        neighbor->pointers[MAX_KEY_NUM] = nd->pointers[MAX_KEY_NUM];
    }
    else
    {
        neighbor->keys[start] = (char *)malloc(MAX_KEY_LEN);
        strcpy(neighbor->keys[start], parent->keys[nd_index - 1]);
        // neighbor->keys[start] = parent->keys[nd_index - 1];
        for (i = start + 1, j = 0; j < nd->num_keys; i++, j++)
        {
            neighbor->keys[i] = nd->keys[j];
            neighbor->pointers[i] = nd->pointers[j];
        }
        neighbor->pointers[i] = nd->pointers[j];
        neighbor->num_keys += nd->num_keys + 1;

        for (i = 0; i <= neighbor->num_keys; i++)
        {
            tmp = (node *)neighbor->pointers[i];
            tmp->parent = neighbor;
        }
    }
    //destroy node;
    free(nd->keys);
    free(nd->pointers);
    free(nd);
    return delete_entry(root, parent, nd_index);
}

//traverse all of the leaves in the B+ Tree
template <typename T>
void BPlusTree<T>::traverse(void (*fn)(T *data))
{
    node *p = this->root;
    if (p == NULL)
        return;
    while (!p->is_leaf)
        p = (node *)p->pointers[0];
    while (p)
    {
        for (int i = 0; i < p->num_keys; i++)
            fn((record *)p->pointers[i]);
        p = (node *)p->pointers[MAX_KEY_NUM];
    }
    return;
}

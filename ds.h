﻿#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H

#define ERR_RESULT 0x80000000 
#define DEFAULT_CAPACITY 1024

//Stack ,Queue,List, HashTable should be used by pointer (newXXX).
//other wise it will cause free-memory problem.
//freeXXX should not be used as this: freeXXX(&localVarible,0/1);

typedef void * PData;

typedef int (*CompareFunction)(void *pA,void *pB);

typedef struct {
	int v1;
	float v2;
	char memo[11];
} Item_ex;


//stack and queue ************************************
typedef struct {
	PData *a;
	int len;
	int top;
} Stack;

typedef struct {
	PData *a;
	int len;
	int head;
	int tail;
} Queue;

Stack * newStack(int len);
void freeStack(Stack *p,int bFreeTarget);

Queue * newQueue(int len);
void freeQueue(Queue *p,int bFreeTarget);


int stack_is_empty(Stack *sp);
PData pop(Stack *sp);
void push(Stack *sp,PData x);

int queue_is_empty(Queue *qp);
void enqueue(Queue *qp,PData x);
PData dequeue(Queue *qp);

//linked list *******************************************
struct list_node{
	void *data;
	struct list_node * prev;
	struct list_node * next;
};

typedef struct list_node ListNode;

typedef struct {
	ListNode *head;
	//CompareFunction fCompare;
} List;

List *newList();
void freeList(List *l,int bFreeTarget);

ListNode * list_search(List *l,PData matchKey,CompareFunction fCompare);
void list_insert(List *l,PData aItem);//aItem must be malloced before call. can't be local stack address
int list_delete(List *l,PData matchKey,CompareFunction fCompare);

//binary tree******************************************
typedef struct binaryTreeNode{
	PData key;
	struct binaryTreeNode *left;
	struct binaryTreeNode *right;
} BinaryTreeNode;
 //struct binaryTreeNode BinaryTreeNode;

//tree******************************************
typedef struct treeNode{
	PData key;
	struct TreeNode *leftChild;
	struct TreeNode *rightSibling;
} TreeNode;

//Hash table *********************************************
typedef int (*HashFunction)(PData key);

typedef struct {
	int len;
	List **slots;
	HashFunction fHash;
	CompareFunction fCompare;
} HashTable;

HashTable * newHashTable(int len,HashFunction fHash,CompareFunction fCompare);
void freeHashTable(HashTable *p,int bFreeTarget);

void hash_insert(HashTable *pH,PData aItem);
PData hash_retrieve(HashTable *pH,PData matchKey);
void hash_delete(HashTable *pH,PData matchKey);

int hashFun_div(int k,int m);//m should be primary number far away from 2^n

int hashFun_mul(int k,int m);//m should be number of 2^n

//************* Disjoint set *************
struct Disjoint_set{
	PData data;
	int rank;
	struct Disjoint_set *parent;
}; 

typedef struct Disjoint_set Dset;

void set_make(Dset *x);
void set_union(Dset *x,Dset *y);
void set_link(Dset *x,Dset *y);
Dset * set_find(Dset *x);





#endif
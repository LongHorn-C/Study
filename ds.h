#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H

#define ERR_RESULT 0x80000000 
#define DEFAULT_CAPACITY 1024

typedef void * PData;

typedef int (*CompareFunction)(void *pA,void *pB);
//linked list *******************************************
struct list_node{
	void *data;
	struct list_node * prev;
	struct list_node * next;
};

typedef struct list_node ListNode;

typedef struct {
	int n;
	ListNode *head;
	ListNode *tail;
	//CompareFunction fCompare;
	ListNode *cursor;
	PData data;
} List;

typedef int (*FListNode)(ListNode *n,PData ctx);

List *newList();
void freeList(List *l,int bFreeTarget);

ListNode * list_search(List *l,PData matchKey,CompareFunction fCompare);
void list_prepend(List *l,PData aItem);//new item became new head;
void list_append(List *l,PData aItem);//new item became new tail;
int list_delete(List *l,ListNode *x);//won't free the node deleted 
int list_delete_by_data(List *l,PData matchKey,CompareFunction fCompare);//won't free the node delete.
void list_clear(List *l,int bFreeTarget);
int list_is_empty(List *l);
void list_foreach(List *l,FListNode f,PData ctx);


//Stack ,Queue,List, HashTable should be used by pointer (newXXX).
//other wise it will cause free-memory problem.
//freeXXX should not be used as this: freeXXX(&localVarible,0/1);



//stack and queue ************************************
/*
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
Queue * newQueue(int len);
*/

typedef List Queue;
typedef List Stack;


Stack * newStack();
Queue * newQueue();
void freeStack(Stack *p,int bFreeTarget);
void freeQueue(Queue *p,int bFreeTarget);


int stack_is_empty(Stack *sp);
PData pop(Stack *sp);
void push(Stack *sp,PData x);

int queue_is_empty(Queue *qp);
void enqueue(Queue *qp,PData x);
PData dequeue(Queue *qp);

//binary tree******************************************
typedef struct binaryTreeNode{
	PData key;
	struct binaryTreeNode *left;
	struct binaryTreeNode *right;
} BinaryTreeNode;
 //struct binaryTreeNode BinaryTreeNode;

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

/* Dset example code
	Dset s[10];
	int i;
	for (i=0;i<10;i++){
		s[i].data = a[i];
		set_make(&s[i]);
	}
	
	i = set_find(&(s[1])) != set_find(&(s[5]));
	tcut_assert("1 !=5 not OK\n", i);

	
	set_union(&(s[1]),&(s[5]));
*/



#endif
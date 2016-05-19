#include <stdlib.h>
#include <stdio.h>
#include "ds.h"

Stack * newStack(int len){
	Stack * p=malloc(sizeof(Stack));
	p->top=-1;
	p->len=len;
	p->a=malloc(sizeof(void *)*len);
	return p;
}

void freeStack(Stack *p,int bFreeTarget){
	if (!p)
		return;
	PData x;
	if (bFreeTarget)
		for (int i=0;i<= p->top;i++){
			//printf("free Pdataitem\n");
			x = p->a[i];
			//printf("x=%x,*x=%d",x,*(int *)x);
			free(x);
		}
	free(p->a);
	free(p);
}



Queue * newQueue(int len){
	Queue * p=malloc(sizeof(Queue));
	p->head=0;
	p->tail=0;
	p->len=len;
	p->a=malloc(sizeof(int)*len);
	return p;
}

void freeQueue(Queue *p,int bFreeTarget){
	if (!p)
		return;
	void * x;
	if (bFreeTarget){
		int i =p->head;
		while (i != p->tail){
			x = p->a[i];
			free(x);
			i++;
			if(i == p->len)
				i=0;
		}

	}

	free(p->a);
	free(p);
}


int stack_is_empty(Stack *sp){
	return sp->top == -1;
}

void push(Stack *sp,PData x){
	(sp->top)++;
	sp->a[sp->top] = x;
}

PData pop(Stack *sp){
	if (stack_is_empty(sp)){
		printf("stack is empty ,error.\n");
		return NULL;
	}
	sp->top	--;
	return sp->a[sp->top + 1];
}

int queue_is_empty(Queue *qp){
	return (qp->head == qp->tail);
}

void enqueue(Queue *qp,PData x){
	qp->a[qp->tail] = x;
	if (qp->tail == qp->len)
		qp->tail = 0;
	else
		qp->tail ++;
}

PData dequeue(Queue *qp){
	PData x =qp->a[qp->head];
	if (qp->head == qp->len)
		qp->head = 0;
	else
		qp->head ++;
	return x;
}

ListNode * list_search(List *l,PData matchKey,CompareFunction fCompare){
	if (!l)
		return NULL;
	ListNode *x=l->head;
	while(x != NULL && fCompare(matchKey,x->data) != 0)
		x=x->next;
	if (x)
		return x->data;
	else
		return NULL;
}
ListNode * list_head(List *l){
	return l->head;
}

List *newList(){
	List * x=(List *)malloc(sizeof(List));
	x->head =NULL;
	return x;
}


void freeList(List *l,int bFreeTarget){
	if (!l)
		return;
	ListNode *t, *x;
	x = l->head;
	while (x != NULL){
		t=x->next;
		if (bFreeTarget)
			free(x->data);
		free(x);
		x = t;
	}
	free(l);
}

static void _list_insert(List *l,ListNode *x){
	x->next = l->head;
	if (l->head != NULL)
		l->head->prev = x;
	l->head = x;
	x->prev =NULL;
}


static int _list_delete(List *l,ListNode *x){
	if (!x)
		return 0;
	if (x->prev != NULL)
		x->prev->next = x->next;
	else//no prev means that x  is the head;
		l->head = x->next;
	if (x->next !=NULL)
		x->next->prev = x->prev;
	return 1;
}

void list_insert(List *l,PData aItem){
	ListNode * x=(ListNode *)malloc(sizeof(ListNode));
	x->data = aItem;
	_list_insert(l,x);
}

int list_delete(List *l,PData matchKey,CompareFunction fCompare){
	ListNode *x = list_search(l,matchKey,fCompare);
	return _list_delete(l,x);
}

HashTable * newHashTable(int len,HashFunction fHash,CompareFunction fCompare){
	HashTable *result =(HashTable *)malloc(sizeof(HashTable));
	//printf("resultOK\n");
	result->len = len;
	result->slots = (List **)malloc(sizeof(List *)*len);
	result->fHash = fHash;
	result->fCompare = fCompare;
	for(int i=0;i <len;i++)
		result->slots[i] =NULL;

	return result;
}

void freeHashTable(HashTable *p,int bFreeTarget){
	if (!p)
		return;
	for (int i=0;i< p->len;i++)
		if (p->slots[i])
			freeList(p->slots[i],bFreeTarget);
	//printf("before free p.slots \n");
	free(p->slots);
	//printf("before free p \n");
	free(p);
}


void hash_insert(HashTable *pH,PData aItem){
	int i = pH->fHash(aItem);
	//printf("i=%d\n",i);
	if (!pH->slots[i])
		pH->slots[i] = newList();
	list_insert(pH->slots[i],aItem);
}

PData hash_retrieve(HashTable *pH,PData matchKey){
	int i = pH->fHash(matchKey);
	return list_search(pH->slots[i],matchKey,pH->fCompare);
}

void hash_delete(HashTable *pH,PData matchKey){
	int i = pH->fHash(matchKey);
	if (pH->slots[i])
		list_delete(pH->slots[i],matchKey,pH->fCompare);
}

int hashFun_div(int k,int m){
	return k % m;
}

int hashFun_mul(int k,int m){
	float f = k * 0.6180339887;
	float f1 = f - (int)f;
	float f2 = m * f1;
	return (int)f2;
}

void set_make(Dset *x){
	x->parent = x;
	x->rank =0;
}
void set_union(Dset *x,Dset *y){
	set_link(set_find(x),set_find(y));
}
void set_link(Dset *x,Dset *y){
	if (x->rank > y->rank)
		y->parent = x;
	else{
		x->parent =y;
		if (x->rank == y->rank)
			y->rank = y->rank +1;
	}

}

Dset * set_find(Dset *x){
	if (x != x->parent)
		x->parent = set_find(x->parent);
	return x->parent;
}


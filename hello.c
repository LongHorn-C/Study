#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 
#include "ds.h"
#include "sort.h"
#include "heap.h"
#include "tcut.h"

int nr_tests = 0;


int randomAB(int a,int b);
void produceA(int *a, int n);
void printA(int *a, int n);

void producePa(PData *a,int n);
void printPa(PData *a,int n);
void printHash(HashTable *h,int n);


void produceDa(PData *a,int n);
void printDa(PData *a,int n);
void printItem_ex(PData i);

void printList(List *l);

int fCompare(PData a,PData b);
int iCompare(PData a,PData b);
int xCompare(PData a,PData b);

int xHash(PData x);


//void testHeap();
//void testHeapSort();
void testList();
void testStackQueue();
void testHash();
static char * testDset();
//void printH(Heap *h);

static char * all_tests() 
{
    tcut_run_test(testDset);
    //tcut_run_test(test_bar);
    return 0;
}

int main(){
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", nr_tests);

    return result != 0;
	//testDset();	

/*
	printf("div hash k = %d,m=%d hash =%d\n",100,23,hashFun_div(100,23));
	printf("div hash k = %d,m=%d hash =%d\n",101,23,hashFun_div(101,23));
	printf("mul hash k = %d,m=%d hash =%d\n",101,16,hashFun_mul(101,16));
	printf("mul hash k = %d,m=%d hash =%d\n",231,256,hashFun_mul(231,256));
	*///long c=clock();
	//testHeapSort();
	//testHash();
	//sleep(1);
	//testList();
	//testStackQueue();
	//PData pd[10];
/*	Item_ex * aa,*bb;
	aa=(Item_ex *) malloc(sizeof(Item_ex));
	aa->v1=323;
	aa->v2 =2;
	aa->memo ="this is a test string";
	bb=(Item_ex *) malloc(sizeof(Item_ex));
	bb->v1=23;
	bb->v2 =1;
	bb->memo ="this is another test string";
	printf("compare %d",oCompare(aa,bb));
	*/
	//printf("%d,%d,%d\n",sizeof(short),sizeof(int),sizeof(long));
	//produceA(a,10);
	//printA(a,10);
	/*int *k;
*/

	//printA(a,10);
	//long l= GetTickCount();
	//heap_sort(a,10);
	//long l2=GetTickCount()-l;
	//printf("elasp time:%d\n",l2);
	//Heap *hp;
	//hp=newHeap(20);
	//for(i=0;i<10;i++)
	//	max_heap_insert(hp,a[i]);
		//max_heap_insert(hp,200);
		//max_heap_insert(hp,50);
	//counting_sort(a,10,100);
	//randomized_quick_sort(a,10);
	//Stack *sp = newStack(20);
	//Queue *qp = newQueue(20);
	/*List *lp=newList();
	for (i=0;i<4;i++)
		list_insert(lp,a[i]);
	printL(lp);
	for (ListNode *t = lp->head; t != NULL;t = t->next){
		//printf("key:%d\n",t->key);
		list_delete(lp,t->key);
	}
	printL(lp);
	freeList(lp);
	*/
	//printf("qp.a=%x\n",qp->a);
	//printf("qp=%x\n",qp);
	//freeStack(sp);
	//freeQueue(qp);
	//printA(a,10);
	//printA(hp->a,11);
	/*for(i=0;i<10;i++){
		printf("%d\n",heap_extract_max(hp));
		//printH(hp);
	}

	freeHeap(hp);
	*/
	
	//long c2= clock();
	//printf("\nclock counter:%d,%d\n",c,c2);
}

void printA(int *a, int n){
	for (int i=0;i<n;i++)
		printf("%d,",a[i]);
	printf("\n");
}


void produceA(int *a, int n)
{
	for (int i=0;i<n;i++)
		a[i] = 1 + rand() % 99;

}
void printList(List *lp){
	if (!lp)
		return;
	
	for (ListNode *t = lp->head; t != NULL;t = t->next)
		printItem_ex(t->data);
	printf("\n");
}

int fCompare(PData a,PData b){
	float r = *((float *)a) - *((float *)b);
	if (r < 1.0e-5  && r > -1.0e-5)
		return 0;
	else if (r <0)
		return -1;
	else
		return 1;
}

int iCompare(PData a,PData b){
	return *((int *)a) - *((int *)b);
}

int xCompare(PData a,PData b){
	Item_ex *e;
	e = (Item_ex *)a;
	int k1 =e->v1;//e->v1 *100 + e->v2*10;
	e = (Item_ex *)b;
	int k2 =e->v1;// *100 + e->v2*10;

	return k1 - k2;
}

/*
void testHeap(){
	int i,a[10];
	produceA(a,10);
	printA(a,10);

	Heap *h =newHeap(20,iCompare);

	max_heap_insert(h,&(a[0]));
	max_heap_insert(h,&(a[1]));
	max_heap_insert(h,&(a[2]));
	printf("max =%d\n",*(int *)heap_maximum(h));
	printf("extract_max =%d\n",*(int *)heap_extract_max(h));
	printf("extract_max =%d\n",*(int *)heap_extract_max(h));
	printf("extract_max =%d\n",*(int *)heap_extract_max(h));
	freeHeap(h,0);
}

void testHeapSort(){
	int a[10];
	produceA(a,10);
	printA(a,10);

	int i,*k;
	PData pd[10];
	for (i =0;i<10;i++){
		pd[i] = malloc(sizeof(int));
		k = (int *)pd[i];
		*k = a[i];
	}
	heap_sort(&pd[0],10,iCompare);

	printf("list: ");
	for (i =0;i<10;i++){
		k = (int *)pd[i];
		printf("%d,",*k);
	}
		printf("\n");

	for (i =0;i<10;i++)
		free(pd[i]);

}
*/


void testList(){
	PData a[10];
	produceDa(a,10);
	printDa(a,10);

	List *l =newList(iCompare);
	for (int i=0;i<10;i++)
		list_insert(l,a[i]);

	printf("List :");

	ListNode *n;
	n = l->head;
	printf("begin List:\n");
	while (n){
		printItem_ex(n->data);
		n = n->next;
	}
	printf("end List:\n");
	freeList(l,1);

}

void testStackQueue(){
	//int a[10];
	//produceA(a,10);
	//printA(a,10);
	PData a[10];
	producePa(a,10);
	printPa(a,10);

	int i;

	Stack *s =newStack(20);
	for (i=0;i<10;i++)
		push(s,(a[i]));

	PData d;
	/*
	while ( !stack_is_empty(s) ){
		d = pop(s);
		if (d)
			printf("%d,",*(int *)d);
		//n = n->next;
	};
	printf("end pop\n");*/
	freeStack(s,1);

	//for (i=0;i<10;i++)
	//	free(a[i]);

	Queue *q =newQueue(20);
	for (i=0;i<10;i++)
		enqueue(q,a[i]);

	printf("queue :");
	//PData d;
	while ( !queue_is_empty(q) ){
		d = dequeue(q);
		if (d)
			printf("%d,",*(int *)d);

	};
	printf("  .end queue\n");
	freeQueue(q,1);

}

void producePa(PData *a,int n){
	int *k;
	for (int i =0;i<n;i++){
		a[i] = malloc(sizeof(int));
		k=(int *)(a[i]);
		*k = randomAB(1,99);
	}
}

void printPa(PData *a,int n){
	int *k;
	printf("PData array:");
	for (int i =0;i<n;i++){
		k=(int *)(a[i]);
		printf("%d,",*k);
	}
	printf("\n");

}


void produceDa(PData *a,int n){
	Item_ex *k;
	for (int i =0;i<n;i++){
		a[i] = malloc(sizeof(Item_ex));
		k=(Item_ex *)(a[i]);
		k->v1 = randomAB(1,99);
		k->v2 = randomAB(1,99)/100.01;
		strcpy(k->memo,"work?");
	}
}

void printDa(PData *a,int n){
//	Item_ex *k;
	printf("begin Data array:\n");
	for (int i =0;i<n;i++){
	//	k=(Item_ex *)(a[i]);
		printItem_ex(a[i]);
	}
	printf("end of DataArray\n");
}

void printHash(HashTable *h,int n){
	int i;
	printf("Hash:\n");
	for(i=0;i<n;i++){
		printf("[%i]:\n",i);
		printList(h->slots[i]);
	}
}



void printItem_ex(PData i){
	Item_ex *k= (Item_ex *)i;
	printf("v1=%d,v2=%f,memo=%s\n",k->v1,k->v2,k->memo);
}

void testHash(){
	PData a[10];//Item_ex array;
	
	
	produceDa(a,10);
	
	
	printf("array :");
	printDa(a,10);

	HashTable *h =newHashTable(20,xHash,xCompare);

	for (int i=0;i<10;i++)
		hash_insert(h,a[i]);

	printHash(h,20);
	
	Item_ex e;
	e.v1 = 95;

	PData p= hash_retrieve(h,&e);
	printf("p=%x\n",p);
	Item_ex *pe = (Item_ex *)p;


	printItem_ex(p);
	
	
	freeHashTable(h,1);

	
}

static char * testDset(){
	PData a[10];//Item_ex array;
	produceDa(a,10);
	printf("array :\n");
	printDa(a,10);
	
	Dset s[10];
	int i;
	for (i=0;i<10;i++){
		s[i].data = a[i];
		set_make(&s[i]);
	}
	
	i = set_find(&(s[1])) != set_find(&(s[5]));
	tcut_assert("1 !=5 not OK\n", i);

	
	set_union(&(s[1]),&(s[5]));
	
	
	i = set_find(&(s[1]))==set_find(&(s[5]));
	tcut_assert("1 ==5 not OK\n", i);
	//	printf("1==5 not OK\n");
	
	i= set_find(&s[1]) != set_find(&s[7]);
	tcut_assert("1 !=7 not OK\n", i);
		//printf("1 != 7 OK\n");
	
	
	set_union(&s[1],&s[7]);

	tcut_assert("1 ==5 not OK\n", set_find(&s[1])==set_find(&s[7]));	
	//printf("1==7 OK\n");
	tcut_assert("anything? not OK\n", 0);		
	return 0;
}

int xHash(PData x){
	Item_ex *k =(Item_ex *)x;
	return hashFun_mul(k->v1,16);
}

/*int randomAB(int a,int b){
	return a + rand() % (b-a);
}
*/
/*
void printH(Heap *hp){
	for (int i=0;i<hp->size;i++)
		printf("%d,",hp->a[i]);
	printf("\n");
}
*/

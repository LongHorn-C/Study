//this file is for back_track and branch-and-bound method.
//for queen problem.  
//answer_list.node.data is an integer which means col.
//ctx is an integer, which means the size of the problem, i.e. size of the chessboard.
#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "common.h"

static int _pass_1(int r1,int c1,int r2,int c2){
	if (r1==r2 || c1 ==c2)
		return 0;
	else if (r2-r1 ==c2 -c1)
		return 0;
	else if (r1 - r2 ==c2 -c1)
		return 0;
    else
		return 1;
}

static int _pass(List *a_p_p,PData a_i,PData ctx){
	int r1=0,r2,c1,c2;
	r2 = a_p_p->n;
	c2=vi->c;
	ListNode *n= a_p_p->head;
	while(n){
		An *vn=(An *) n->data;
		r1++;
		c1 = n->data;
		if( !pass_1(r1,c1,r2,c2 ))
			return 0
		n=n->next;
	}
	
	int size = (int)ctx;
	if (r1 == size-1)
		return 2;

	return 1;
}

/*
static int _is_answer(List *a_p_p,PData ctx){
	int n = (int)ctx;
	return n == a_p_p->n;
}
*/

static int _first_child(List *a_p_p,PData *a_i,PData ctx){
	*a_i = 0;
	return *a_i  <  (int)ctx;
}

static int _next_sibling(List *a_p_p,PData *a_i,PData ctx){
	(*a_i)++;
	return *a_i < (int)ctx ;
}	

static void _print_result(List *a,PData ctx){
	ListNode *n= a_p_p->head;
	while (n){
		int col = (int) n->data;
		printf("%d, "col);
		n = n->next;
	}
	printf("\n");
}

int queen(int n){
	Bt_callbacks cb;
	cb.pass =_pass;
	//cb.is_answer =_is_answer;
	cb.first_child =_first_child;
	cb.next_sibling =_next_sibling;
	
	List *al =newList();
	An *a=(An *)malloc(sizeof An);
	a = 0;
	if (back_track(al,a,&cb,n))
		_print_result(al,NULL);
	freeList(al,0);
}

void main(){
	queen(4);
}

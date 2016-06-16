//this file is for back_track and branch-and-bound method.
//for queen problem.
//answer_list.node.data is a pointer to struct An.
//use external complex data storage for answer_list.node.data for whole answer_list
#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "common.h"

typedef struct{
	int r;
	int c;
}An;


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
	int r1,r2,c1,c2;
	An *vn,*vi;	
	vi = (An *)a_i;
	r2=vi->r;
	c2=vi->c;
	ListNode *n= a_p_p->head;
	while(n){
		An *vn=(An *) n->data;
		r1 = vn->r;
		c1 = vn->c;
		if( !pass_1(r1,c1,r2,c2 ))
			return 0
		n=n->next;
	}	
	
	int size = ((_Context *)ctx)->n;	
	if ( r2 ==  size-1 )
		return 2;
	
	return 1;
}

/*
static int _is_answer(List *a_p_p,PData ctx){
	ListNode *n= a_p_p->tail;
	An *a= (An *)n->data;
	int n = ((_Context *)ctx)->n;
	return n-1 == a->r;
}
*/
static int _first_child(List *a_p_p,PData *a_i,PData ctx){
	_Context *context = (_Context *)ctx;
	int nl = a_p_p->n;
	An *vi = context.mem + nl;
	An *p=(a_p_p->tail->data);
	vi->r= p->r + 1;
	vi->c = 0;
	
	An **pp =(An **)a_i;
	*pp = vi;
	int n =  c->n;
	return vi->r  < n;
}

static int _next_sibling(List *a_p_p,PData a_i,PData ctx){
	An *vi = *(An **)a_i;
	vi->c ++;
	int n = ((_Context *)ctx) ->n;
	return vi->c < n ;
}	

static _print_result(List *a,PData ctx){
	ListNode *n= a_p_p->head;
	while (n){
		An *a = (An *)n->data;
		printf("[%d, %d] , "a->r,a->c);
		n = n->next;
	}
}

typedef struct{
	int n;
	An *mem;
}_Context;

int queen(int n){
	Bt_callbacks cb;
	cb.pass =_pass;
	//cb.is_answer =_is_answer;
	cb.first_child =_first_child;
	cb.next_sibling =_next_sibling;
	
	_Context ctx;
	ctx.n = 4;
	ctx.mem = (An *)malloc(sizeof(An)*4);
	
	List *al =newList();
	An *a=(An *)malloc(sizeof An);
	a->r = 0;
	a->c = 0;
	
	if (back_track(al,a,&cb,n))
		_print_result(al,NULL);
	
	freeList(al,0);
	free(ctx.mem);
}

void main(){
	queen(4);
}

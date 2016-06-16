//this file provide a framework for back_track and branch-and-bound method.
//normally, PData ctx is a pointer to the struct of the problem-definition 
//and exchangable data between functions
#ifndef __CLH_BBB_H
#define __CLH_BBB_H


#include "ds.h"

typedef int (*F_bt1)(List *path,PData a_i,PData ctx);
//typedef int (*F_bt2)(List *path,PData *a_i,PData ctx);

typedef struct{
	F_bt1 pass;  //return 0: fail;1: pass; 2: final answer.
	//F_bt2 is_answer;
	F_bt1 get_first_child; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 get_next_sibling; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 on_append_tail;
	F_bt1 on_delete_tail;
} Bt_callbacks;


int back_track_to_get_next(List *path,PData a_i,F_bt1 get_next_sibling,PData ctx);
void back_track(List *ans_pre_part,PData *ans_incr,Bt_callbacks *cb,PData ctx);

//for convienent use. callback->.on_append.. =  on_append_.copy ... free.
int on_append_tail__copy(List *path,PData a_i,int size,PData ctx);
int on_delete_tail__free(List *path,PData a_i,PData ctx);

#endif


/*  Example code for 4 square Queen problem.
//for queen problem.
//answer_list.node.data is a pointer to struct An.
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

static int _pass(List *path,PData a_i,PData ctx){
	int r1,r2,c1,c2;
	An *vn,*vi;	
	vi = (An *)a_i;
	r2=vi->r;
	c2=vi->c;
	ListNode *n= path->head;
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

static int _get_first_child(List *path,PData *a_i,PData ctx){
	_Context *context = (_Context *)ctx;
	int nl = path->n;
	An *vi = context.mem + nl;
	An *p=(path->tail->data);
	vi->r= p->r + 1;
	vi->c = 0;
	
	An **pp =(An **)a_i;
	*pp = vi;
	int n =  c->n;
	return vi->r  < n;
}

static int _next_sibling(List *path,PData a_i,PData ctx){
	An *vi = *(An **)a_i;
	vi->c ++;
	int n = ((_Context *)ctx) ->n;
	return vi->c < n ;
}	

static _print_result(List *a,PData ctx){
	ListNode *n= path->head;
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
	cb.get_first_child =_get_first_child;
	cb.get_next_sibling =_next_sibling;
	
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

*/
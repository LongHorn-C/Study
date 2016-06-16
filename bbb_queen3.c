//this file is for back_track and branch-and-bound method.
//for queen problem.
//answer_list.node.data is a pointer to struct An.
// use on_delete_tail on_append_tail

#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "common.h"

typedef struct{
	int r;
	int c;
}An;

static int _on_append_tail(List *a_p_p,PData a_i,PData ctx){
	An * a1= (An *)a_i;
	An * a2= (An *)malloc(sizeof An);
	*a2 =*a1; //or copymem
	a_p_p.tail.data = (PData) a2;
	return 0;
}

static int _on_delete_tail(List *a_p_p,PData a_i,PData ctx){
	free(a_i);
	return 0;
}


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
	
	int size =(int)ctx;
	if ( r2>=size || c2>=size)
		return 0;
	
	ListNode *n= a_p_p->head;
	while(n){
		An *vn=(An *) n->data;
		r1 = vn->r;
		c1 = vn->c;
		if( !pass_1(r1,c1,r2,c2 ))
			return 0
		n=n->next;
	}	
	
	if ( r2 ==  size-1 )
		return 2;
	
	return 1;
}

static int _first_child(List *a_p_p,PData a_i,PData ctx){
	int nl = a_p_p->n;
	An *vi = (An *)a_i;
	An *p=(a_p_p->tail->data);
	vi->r= p->r + 1;
	vi->c = 0;
	
	int n =  (int)ctx;

	return vi->r  < n;
}

static int _next_sibling(List *a_p_p,PData a_i,PData ctx){
	An *vi = (An *)a_i;
	vi->c ++;

	int n = (int)ctx;
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


int queen(int n){
	Bt_callbacks cb;
	cb.pass =_pass;
	cb.first_child =_first_child;
	cb.next_sibling =_next_sibling;
	cb.on_append_tail = _on_append_tail;
	cb.on_delete_tail = _on_delete_tail;
	
	
	List *al =newList();
	An a;
	a.r = 0;
	a.c = 0;
	
	if (back_track(al,&a,&cb,(PData)n))
		_print_result(al,NULL);
	
	freeList(al,0);	
}

void main(){
	queen(4);
}

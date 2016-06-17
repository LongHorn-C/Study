//this file provide a framework for back_track and branch-and-bound method.
//normally, PData ctx is a pointer to the struct of the problem-definition 
//and exchangable data between functions
#ifndef __CLH_BBB_H
#define __CLH_BBB_H

#include "ds.h"
#include "tree.h"


typedef int (*F_bt1)(Tree *path,PData a_i,PData ctx);
//typedef int (*F_bt2)(List *path,PData *a_i,PData ctx);

typedef struct{
	F_bt1 pass;  //return 0: fail;1: pass; 2: final answer.
	//F_bt2 is_answer;
	F_bt1 get_first_child; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 get_next_sibling; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 on_append_tail;
	F_bt1 on_delete_tail;
} Bt_callbacks;


int back_track_to_get_next(Tree *path,PData a_i,int ans_size,Bt_callbacks *cb,PData ctx);
int back_track(Tree *path,PData ans_incr,int ans_size,Bt_callbacks *cb,PData ctx);



//Branch and bound algorithm
typedef struct{
	F_bt1 pass;  //return 0: fail;1: pass; 2: an optional sulution, maybe not optimal.
	F_bt1 get_first_child; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 get_next_sibling; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 on_append_tail;
	F_bt1 on_delete_tail;
} Bb_callbacks;

int branch_bound(TreeNode *path,PData ans_incr,int ans_size,Bb_callbacks *cb,PData ctx);

//t means 

#endif


/*  Example code for 4 square Queen problem.
#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "common.h"
#include "bbb.h"

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
	
	int size =(int)ctx;
	if ( r2>=size || c2>=size)
		return 0;
	
	ListNode *n= path->head;
	while(n){
		An *vn=(An *) n->data;
		r1 = vn->r;
		c1 = vn->c;
		if( !_pass_1(r1,c1,r2,c2 ))
			return 0;
		n=n->next;
	}	
	
	if ( r2 ==  size-1 )
		return 2;
	
	return 1;
}

static int _get_first_child(List *path,PData a_i,PData ctx){
	An *vi = (An *)a_i;
	An *p = (An *)(path->tail->data);
	vi->r = p->r + 1;
	vi->c = 0;
	
	int n =  (int)ctx;

	return vi->r  < n;
}

static int _get_next_sibling(List *path,PData a_i,PData ctx){
	An *vi = (An *)a_i;
	vi->c ++;

	int n = (int)ctx;
	return vi->c < n ;
}	



static int _print_ans_node(ListNode *n,PData ctx){
	An *a = n->data;
	printf("[%d, %d] , ",a->r,a->c);
}

int queen(int n){
	Bt_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
		cb.on_append_tail = NULL;
		cb.on_delete_tail = NULL;
	List *al =newList();
	An a;
		a.r = 0;
		a.c = 0;
	
	if (back_track(al,&a,sizeof(An),&cb,(PData)n)){
		printf("soluction found: \n\t");
		list_foreach(al,_print_ans_node,(PData)n);
	}
	
	freeList(al,1);	
}

void main(){
	queen(4);
}

*/
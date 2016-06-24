//this file is for back_track and branch-and-bound method.
//for queen problem.
//build command: gcc -o q3 bbb_queen3.c bbb.c tree.c ds.c

#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "common.h"
#include "bbb.h"

typedef struct{
	int r;
	int c;
}_Answer_step;


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

static int _pass(Tree *path,PData a_i,PData ctx){
	int r_i,rp,c_i,cp;
	_Answer_step *vn,*vi;
	vi = (_Answer_step *)a_i;
	r_i=vi->r;
	c_i=vi->c;

	int size =(int)ctx;
	if ( r_i>=size || c_i>=size)
		return 0;

	TreeNode *n= path->cursor;
	while(n && n!= path->root){
		_Answer_step *vn=(_Answer_step *) n->data;
		rp = vn->r;
		cp = vn->c;
		if( !_pass_1(r_i,c_i,rp,cp ))
			return 0;
		n= n->parent;
	}

	if ( r_i >=  size-1 )
		return 2;

	return 1;
}

static int _get_first_child(Tree *path,PData a_i,PData ctx){
	_Answer_step *vi = (_Answer_step *)a_i;
	if (path->cursor == path->root){
		vi->r =0 ;
		vi->c =0 ;
	}
	else{
		_Answer_step *p = (_Answer_step *)(path->cursor->data);
		vi->r = p->r + 1;
		vi->c = 0;
	}
	int n =  (int)ctx;

	return vi->r  < n;
}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	_Answer_step *vi = (_Answer_step *)a_i;
	vi->c ++;

	int n = (int)ctx;
	return vi->c < n ;
}



static int _print_ans_node(TreeNode *n,PData ctx){
	_Answer_step *a = n->data;
	if (a)
		printf("[%d, %d] , ",a->r,a->c);
}

int queen(int n){
	Bt_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
	Tree *sst =newTree();
	_Answer_step a;

	if (back_track(sst,&a,sizeof(_Answer_step),&cb,(PData)n)){
		printf("solution found: \n\t");
		tree_foreach(sst->root,(PData)n,_print_ans_node);
		printf("\nend print solution \n");
	}

	freeTree(sst,1);
}

void main(){
	queen(4);
}

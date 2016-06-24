//this file is for back_track and branch-and-bound method.
//for subset-sum problem.
//build command: gcc -o s3 bbb_subset_sum.c bbb.c  tree.c ds.c


#include <stdio.h>
#include <stdlib.h>

#include "bbb.h"
#include "ds.h"
#include "common.h"

typedef struct {
	int *a;
	int n;
	int sum;
}_Prb;  //context

typedef struct {
	int accum;
	int idx;
	int sel;
}_Answer_step;  //type of  AnswerList -> Node->data.

/*

*/
static int _pass(Tree *path,PData a_i,PData ctx){
	_Prb *prb = ctx;
	_Answer_step *as = a_i;
	int idx = as->idx;
	if ( idx >= prb->n)
		return 0;

    int accum = as->accum;

	if (accum == prb->sum)
		return 2;

	if (accum > prb->sum)
		return 0;

	int rs = 0;
		for (int i= idx+1;i< prb->n ;i++)
			rs += prb->a[i];
	if (accum +rs < prb->sum)
		return 0;

	return 1;
}


static int _get_first_child(Tree *path,PData a_i,PData ctx){
	_Prb *prb = (_Prb *)ctx;
	_Answer_step *as = a_i;
	if (path->cursor == path->root){
		as->idx = 0;
		as->sel = 1;
		as->accum = prb->a[0];
	}
	else{
		_Answer_step *cursor_data = path->cursor->data;
		int idx= 0;
		int accum =0;
		if (path->cursor->data){
			_Answer_step *cursor_data = path->cursor->data;
			idx =  cursor_data->idx +1;
			accum = cursor_data->accum;
		}

		if ( idx >= prb->n )
			return 0;
		as->idx = idx;
		as->sel = 1;
		as->accum = accum + prb->a[idx];
	}
	return as->idx < prb->n;
}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	_Answer_step *as = a_i;
	if (as->sel == 0)
		return 0;
	as->sel = 0;
	_Prb *prb = (_Prb *)ctx;
	as->accum -=  prb->a[as->idx];
	return 1;
}

/*static int _print_ans_node(TreeNode *n,PData ctx){
	_Prb *prb = (_Prb *)ctx;
	if (n->data){
		_Answer_step * alnd = n->data;
		if (alnd->sel)
			printf("[index:%d, value: %d] ",alnd->idx,prb->a[alnd->idx]);
	}

}*/

static int _print_answer(Tree *sst,PData ctx){
	printf("solution found, begin printing solution: \n\t");
	_Prb *prb = (_Prb *)ctx;
	for (TreeNode *n = sst->root->first_child;n;n=n->first_child){
		_Answer_step * as = n->data;
		if (as->sel)
			printf("[index:%d, value: %d] ",as->idx,prb->a[as->idx]);
	}
	printf("\nend printing solution.");
}



int subset_sum(int *a,int n,int sum){
	_Prb prb;
		prb.a = a;
		prb.n = n;
		prb.sum = sum;

	Bt_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
	Tree *sst =newTree();
	_Answer_step a_i;

	if (back_track(sst,&a_i,sizeof(a_i),&cb,&prb))
		_print_answer(sst,&prb);
	else
        printf("\nno solution found. \n");
	freeTree(sst,1);
}

void main(){
	int na[5] = {1,2,5,6,8};
	subset_sum(na,5,9);
}

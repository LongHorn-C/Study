//this file is for back_track and branch-and-bound method.
//for subset_sum problem.
//answer_list.node.data is {0,1} which means selected or not selected.
//ctx is a pointer to the problem-definition structure.

#include <stdio.h>
#include <stdlib.h>

#include "bbb.h"
#include "ds.h"
#include "common.h"

typedef struct {
	int *a;
	int n;
	int sum;
}_Ctx;  //context

typedef struct {
	int accum;
	int idx;
	int sel;
}_Answer_step;  //type of  AnswerList -> Node->data.

/*

*/
static int _pass(Tree *path,PData a_i,PData ctx){
	_Ctx *prb = ctx;
	int idx= 0;
	if (path->cursor->data){
		_Answer_step *cursor_data = path->cursor->data;
		idx =  cursor_data->idx +1;
	}

	if ( idx >= prb->n)
		return 0;

	_Answer_step *as = a_i;

	/*int accum = prb->a[as->idx] * as->sel;
		if  (path->cursor != path->root ){
			_Answer_step *tail_d =  path->cursor->data;
			accum += tail_d->accum;
		}
*/
    //_Answer_step *as = a_i;
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
/*
static int _on_append_tail(Tree *path,PData a_i,PData ctx){;
	//update path.cursor.data.accum.
	_Ctx *prb = (_Ctx *)ctx;
	_Answer_step *tail_d = path->cursor->data;

	tail_d->accum = prb->a[tail_d->idx] * tail_d->sel;

	TreeNode *parent = path->cursor->parent;
	if (parent->data){
		_Answer_step *parent_d = parent->data;
		tail_d->accum += parent_d->accum;
	}
	return 1;
}
*/

static int _get_first_child(Tree *path,PData a_i,PData ctx){
	_Ctx *prb = (_Ctx *)ctx;
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
	_Answer_step *as = a_i;
	as->idx = idx;
	as->sel = 1;
	as->accum = accum + prb->a[cursor_data->idx];

	return 1;
}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	_Answer_step *as = a_i;
	if (as->sel == 0)
		return 0;
	as->sel = 0;
	_Ctx *prb = (_Ctx *)ctx;
	as->accum -=  prb->a[as->idx];
	return 1;
}

static int _print_ans_node(TreeNode *n,PData ctx){
	_Ctx *prb = (_Ctx *)ctx;
	if (n->data){
		_Answer_step * alnd = n->data;
		if (alnd->sel)
			printf("[index:%d, value: %d] ",alnd->idx,prb->a[alnd->idx]);
	}

}


int subset_sum(int *a,int n,int sum){
	Bt_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
		/*cb.on_delete_tail = NULL;
		cb.on_append_tail = _on_append_tail;*/

	_Ctx prb;
		prb.a = a;
		prb.n = n;
		prb.sum = sum;

	TreeNode *rn =newTreeNode();
	Tree *sst =newTree(rn);
		sst->cursor = rn;
	_Answer_step a_i;
		a_i.idx = 0;
		a_i.sel = 1;
		a_i.accum = prb.a[0];

	if (back_track(sst,&a_i,sizeof(a_i),&cb,&prb)){
		printf("solution found: \n\t");
		tree_foreach(sst->root,&prb,_print_ans_node);
		printf("\n end print solution \n");
		
	}
	else
        printf("\nno solution found. \n");
	freeTree(sst,1);
}

void main(){
	int na[5] = {1,2,5,6,8};
	subset_sum(na,5,9);
}

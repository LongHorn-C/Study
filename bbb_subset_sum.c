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
}_Al_n_d;  //type of  AnswerList -> Node->data.

/*

*/
static int _pass(Tree *path,PData a_i,PData ctx){
	_Ctx *prb = ctx;
	int idx= 0;
	if (path->cursor->data){
		_Al_n_d *cursor_data = path->cursor->data;
		idx =  cursor_data->idx +1;
	}
	
	if ( idx >= prb->n)
		return 0;

	_Al_n_d *alnd_i = a_i;

	int accum = prb->a[alnd_i->idx] * alnd_i->sel;
		if  (path->cursor != path->root ){
			_Al_n_d *tail_d =  path->cursor->data;
			accum += tail_d->accum;
		}

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

static int _on_append_tail(Tree *path,PData a_i,PData ctx){;
	//update path.cursor.data.accum.
	_Ctx *prb = (_Ctx *)ctx;
	_Al_n_d *tail_d = path->cursor->data;

	tail_d->accum = prb->a[tail_d->idx] * tail_d->sel;

	TreeNode *parent = path->cursor->parent;
	if (parent->data){
		_Al_n_d *parent_d = parent->data;
		tail_d->accum += parent_d->accum;
	}
	return 1;
}

static int _get_first_child(Tree *path,PData a_i,PData ctx){
	_Ctx *prb = (_Ctx *)ctx;
	_Al_n_d *cursor_data = path->cursor->data;
	int idx= 0;
	if (path->cursor->data){
		_Al_n_d *cursor_data = path->cursor->data;
		idx =  cursor_data->idx +1;
	}

	if ( idx >= prb->n )
		return 0;
	_Al_n_d *alnd_i = a_i;
	alnd_i->idx = idx;
	alnd_i->sel = 1;
	return 1;
}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	_Al_n_d *alnd_i = a_i;
	if (alnd_i->sel == 0)
		return 0;
	alnd_i->sel = 0;
	return 1;
}

static int _print_ans_node(TreeNode *n,PData ctx){
	_Ctx *prb = (_Ctx *)ctx;
	if (n->data){
		_Al_n_d * alnd = n->data;
		if (alnd->sel)
			printf("[index:%d, value: %d] ",alnd->idx,prb->a[alnd->idx]);
	}

}


int subset_sum(int *a,int n,int sum){
	Bt_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
		cb.on_delete_tail = NULL;
		cb.on_append_tail = _on_append_tail;

	_Ctx prb;
		prb.a = a;
		prb.n = n;
		prb.sum = sum;

	TreeNode *rn =newTreeNode();
	Tree *sst =newTree(rn);
		sst->cursor = rn;
	_Al_n_d a_i;
		a_i.idx = 0;
		a_i.sel = 1;

	if (back_track(sst,&a_i,sizeof(a_i),&cb,&prb)){
		printf("solution found: \n\t");
		tree_foreach(sst->root,_print_ans_node,&prb);
	}
	freeTree(sst,1);
}

void main(){
	int na[5] = {1,2,5,6,8};
	subset_sum(na,5,9);
}

//this file is for back_track and branch-and-bound method.
//for knapsack problem.
//build command: gcc -o ks bbb_knapsack.c bbb.c ds.c tree.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "bbb.h"
#include "ds.h"
#include "sort.h"
#include "common.h"


typedef struct{
	int w;
	int v;
} _Item;


typedef struct {
	_Item *items;
	int n;
	int limit;
}_Prb;  //problem as context

typedef struct {
	int idx;
	int sel;
	int t_w; //total weight;
	int value;  //tatal value
	int bound;
}_Answer_step;  //type of  AnswerList -> Node->data.

/*

*/
static int _pass(Tree *path,PData a_i,PData ctx){
	_Prb *prb = ctx;
	_Answer_step *as = a_i;
	int idx = as->idx;
	if ( idx >= prb->n)
		return 0;

    int t_w = as->t_w;

	if (t_w == prb->limit)
		return 2;

	if (t_w > prb->limit)
		return 0;

	if (idx == prb->n -1)
		return 2;

	return 1;
}

static float _calc_bound(Tree *sst,PData answer_step,PData ctx){
	_Prb *prb = (_Prb *)ctx;
	_Answer_step *as = answer_step;
	float ub = as->value;
	int i = as->idx+1;
	if (i<prb->n){
		float r = prb->items[i].v *1.0 / prb->items[i].w;
		ub += (prb->limit - as->t_w) * r;
	}
	return round(ub);

	//return 1000;
}

static int _get_first_child(Tree *path,PData a_i,PData ctx){
	_Prb *prb = (_Prb *)ctx;
	_Answer_step *as = a_i;
	if (path->cursor == path->root){
		as->idx = 0;
		as->sel = 1;	
		as->value =  prb->items[0].v;
		as->t_w =  prb->items[0].w;
	}
	else{
		_Answer_step *cursor_data = path->cursor->data;
		_Answer_step *cursor_as = bb_get_answer_step(path->cursor);
		
		as->idx =  cursor_as->idx +1;
		if ( as->idx >= prb->n )
			return 0;
		as->sel = 1;
		as->value = cursor_as->value + prb->items[as->idx].v;
		as->t_w = cursor_as->t_w + prb->items[as->idx].w;
	}
	//calc bound
	as->bound = _calc_bound(path,a_i,ctx);
	//if (! bb_check_bound(path,as->bound))
	//	return 0;
	return 1;
}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	_Answer_step *as = a_i;
	if (as->sel == 0)
		return 0;
	as->sel = 0;
	_Prb *prb = (_Prb *)ctx;
	as->value -=  prb->items[as->idx].v;
	as->t_w -=  prb->items[as->idx].w;
	//printf("\nnext-sibling ");
	//calc bound
	as->bound = _calc_bound(path,a_i,ctx);
	//if (! bb_check_bound(path,as->bound))
	//	return 0;
	return 1;
}


static float _get_value(Tree *sst,PData answer_step,PData ctx){
	_Answer_step *si = answer_step;
	return si->value;
}

static float _get_bound(Tree *sst,PData answer_step,PData ctx){
	_Answer_step *si = answer_step;
	return si->bound;
}

static int _print_answer(Tree *sst,PData ctx){
	printf("solution found, begin printing solution: \n");
	printf("\toptimal value:%f\n",bb_get_optimal(sst));
	//tree_foreach(sst->root,&prb,_print_ans_node);
	_Prb *prb = (_Prb *)ctx;
	for (TreeNode *n = sst->root->first_child;n;n=n->first_child){
		_Answer_step * alnd = bb_get_answer_step(n);
		if (alnd->sel)
			printf("\t[index:%d,weight:%d,value:%d]\n ",
				alnd->idx,prb->items[alnd->idx].w,prb->items[alnd->idx].v);
	}
	printf("end printing solution.");	
}

int knapsack(_Item *a,int n,int limit){
	_Prb prb;
		prb.items = a;  //sort the array by v/w ratio.  omitted.
		prb.n = n;
		prb.limit = limit;

	Bb_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
		cb.get_value = _get_value;
		cb.get_bound = _get_bound;
	Tree *sst =newTree();
	_Answer_step s;

	if ( branch_bound(sst,&s,sizeof(_Answer_step),&cb,1,&prb))
		_print_answer(sst,&prb);
	else
		printf("no solution\n");
	free_all_sst_node_data(sst);
	freeTree(sst,0);
}

void main(){
	_Item a[4] = {{4,40},{7,42},{5,25},{3,12}};
	knapsack(a,4,10);
}

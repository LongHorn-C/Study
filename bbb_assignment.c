//this file is for back_track and branch-and-bound method.
//for Hamiltonian circuit problem.
//build command: gcc -o as bbb_assignment.c bbb.c  tree.c ds.c


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "bbb.h"
#include "g.h"
#include "common.h"


typedef struct {
	int *a;
	int n;
//	int *min;
}_Prb;  //type of  AnswerList -> Node->data.


typedef struct {
	int iPerson;
	int iJob;
	int value;
	int bound;
}_Answer_step;  //type of  AnswerList -> Node->data.



static int _pass(Tree *sst,PData a_i,PData ctx){
	//int iP,iJ,iPP,iJJ;
	_Answer_step *vn,*vi;
	vi = a_i;

	_Prb *prb = ctx;

	if ( vi->iPerson >= prb->n || vi->iJob >= prb->n )
		return 0;

	for (TreeNode *n = sst->cursor;n && n!= sst->root;n = n->parent){
		vn = bb_get_answer_step(n);
		if (vi->iJob == vn->iJob)
			return 0;
	}

	if ( vi->iPerson >=  prb->n -1 )
		return 2;

	return 1;
}

static float _calc_bound(Tree *sst,PData answer_step,PData ctx){
	_Prb *prb  = ctx;
	_Answer_step *si = answer_step;
	int n = prb->n;
	float bound = prb->a[n*si->iPerson + si->iJob];

	for (TreeNode *node = sst->cursor;node!=sst->root;node=node->parent){
		_Answer_step *as = bb_get_answer_step(node);
		bound += prb->a[n*as->iPerson + as->iJob];
	}

	for (int i= si->iPerson+1;i<n;i++){
		int r_min = INT_MAX;
		for (int j=0;j<n;j++){
            int ig = 0;
			if (j==si->iJob)
				ig = 1;
			for (TreeNode *node = sst->cursor;!ig && node!=sst->root;node=node->parent){
				_Answer_step *as = bb_get_answer_step(node);
				if (j == as->iJob)
					ig = 1;
			}
			if (!ig && prb->a[i*n+j] < r_min)
				r_min = prb->a[i*n+j];
		}
		bound += r_min;
	}
	//printf("%d,%d,bound=%f\n",si->iPerson,si->iJob,bound);
	return bound;
}


static int _get_first_child(Tree *sst,PData answer_step,PData ctx){
	_Answer_step *si = answer_step;
	if (sst->cursor == sst->root)
		si->iPerson = 0;
	else{
		_Answer_step *sc = bb_get_answer_step(sst->cursor);
		si->iPerson = sc->iPerson + 1;
	}
	
	si->iJob = 0;

	_Prb *prb  = ctx;
	if (si->iPerson >= prb->n)
		return 0;

	int *a = prb->a;
	int sz = prb->n;

	//calc value
	int accum = a[sz*si->iPerson];
	if (sst->cursor->data){
		_Answer_step *sc = bb_get_answer_step(sst->cursor);
		accum += sc->value;
	}
	si->value = accum;

	si->bound = _calc_bound(sst,answer_step,ctx);

	if (! bb_check_bound(sst,si->bound))
		return 0;
	return 1;
}

static int _get_next_sibling(Tree *sst,PData answer_step,PData ctx){
	_Prb *prb  = ctx;
	_Answer_step *si = answer_step;

	int *a = prb->a;
	int n = prb->n;
	int p = si->iPerson;

	si->value -=  a[n*p+si->iJob];
	si->iJob ++;
	si->value +=  a[n*p+si->iJob];
	if (si->iJob >= n)
		return 0;  // before returning, si->value and si->iJob must be calculated correctly.
					// because it still will be used later.

	si->bound = _calc_bound(sst,answer_step,ctx);
	if (! bb_check_bound(sst,si->bound))
		return 0;
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


static int _print_ans_node(TreeNode *n,PData ctx){
	if (!n->data)
		return 0;
	_Answer_step *a = bb_get_answer_step(n);
	if (a)
		printf("[%d, %d] , ",a->iPerson,a->iJob);
}

int assignment(int *a,int n){
	_Prb prb;
		prb.a = a;
		prb.n = n;

	Bb_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
		cb.get_value = _get_value;
		cb.get_bound = _get_bound;
	Tree *sst =newTree();
	_Answer_step s;
	if ( branch_bound(sst,&s,sizeof(_Answer_step),&cb,0,&prb)){
		printf("solution found: \n\t");
		tree_foreach(sst->root,NULL,_print_ans_node);
		printf("\nend printing solution.");
	}
	else
		printf("no solution\n");
	free_all_sst_node_data(sst);
	freeTree(sst,0);
}

void main(){
	int a[4][4] ={
		{9,2,7,8},
		{6,4,3,7},
		{5,8,1,8},
		{7,6,9,4}
	};
	assignment((int *)a,4);
}

#include <stdio.h>
#include <stdlib.h>

#include "bbb.h"
#include "common.h"


int back_track_to_get_next(Tree *path,PData a_i,int ans_size,Bt_callbacks *cb,PData ctx){
	//free(a_i);
	int bFound = 0;
	while ( !bFound && path->cursor != path->root){
		TreeNode *tail = path->cursor;
		path->cursor = tail->parent;
		tree_delete(tail);
		//if (cb->on_delete_tail)
			//	cb->on_delete_tail(path,a_i,ctx);
		memcpy(a_i,tail->data,ans_size);
		free(tail->data);
		free(tail);//

		bFound = cb->get_next_sibling(path,a_i,ctx);
	}
	return bFound;
}

int back_track(Tree *path,PData ans_incr,int ans_size,Bt_callbacks *cb,PData ctx){
	int bFound;
	do{
		int iPass = cb->pass(path,ans_incr,ctx);
		if( iPass != 0){
			PData *p_new = malloc(ans_size);
			memcpy(p_new,ans_incr,ans_size);
			path->cursor = tree_append_child(path->cursor,p_new);
			//if (cb->on_append_tail)
			//	cb->on_append_tail(path,ans_incr,ctx);

			if ( iPass == 2 ){
				return 1;
			}
			bFound = cb->get_first_child(path,ans_incr,ctx);
		}
		else
			bFound = cb->get_next_sibling(path,ans_incr,ctx);
		if (!bFound)
			bFound = back_track_to_get_next(path,ans_incr,ans_size,cb,ctx);
	}while(bFound);
	return 0;
}

//Branch and bound method;

typedef struct {
	int pass;
	float value;
	float bound;
	PData data;
}_Node_data;

typedef struct {
	TreeNode *best_solution_sofar;
	//treenode best_bound_node;
	int bigger_better;
	PData data;
}_Tree_data;


#define _nd_of(n)  ((_Node_data *)((n)->data))
#define _td_of(t)  ((_Tree_data *)((t)->data))
#define _better_than(r,a,b) ( (r) ? ((a)>(b)?1:0) : ((a)<(b)?1:1) )

static int _kill_node(Tree *sst,TreeNode *node){
	if (node == sst->root)
		return 0;  //no solution. branch-bound terminate.
	TreeNode *parent = node->parent;
	tree_delete(node);
	free(_nd_of(node)->data);
	free(node->data);
	free(node);

	if(!parent || parent->first_child)
		return 1;
	else
		return _kill_node(sst,parent);
}

static PData _malloc_sst_node_data(PData a_i,int a_i_d_size){
	_Node_data *nd = malloc(sizeof(_Node_data));
	memcpy(nd,a_i,sizeof(_Node_data));

	PData new_a_i_data = malloc(a_i_d_size);
	memcpy(new_a_i_data,((_Node_data *) a_i)->data,a_i_d_size);

	nd->data = new_a_i_data;
	return nd;
}


typedef struct{
	Tree *sst;
	TreeNode *best;
	Queue *q;
}_Ctx_best;

static int _get_best_bound_node(TreeNode *n,PData ctx){
	if (n->first_child)
		return 0;
	TreeNode *best = ((_Ctx_best *)ctx)->best;

	if (!best)
		best = n;
	else{
		_Ctx_best * cb = (_Ctx_best *)ctx;
		int bigger_better = _td_of(cb->sst)->bigger_better;
		float nb = _nd_of(n)->bound;
		float bb =_nd_of(best)->bound;
		if ( _better_than(bigger_better,nb,bb) )
			best  = n;
	}
}

static  TreeNode  *_get_best_bound(Tree *sst){
	_Ctx_best ctx;
	ctx.sst =sst;
	ctx.best =0;
	tree_foreach(sst->root,&ctx,_get_best_bound_node);
	return ctx.best;
}



static int _prune_collect(TreeNode *n,PData ctx){
	if (n->first_child)
		return 0;

	Tree *sst =(Tree *)ctx;
	TreeNode *best = _td_of(sst)->best_solution_sofar;
	int bigger_better = _td_of(sst)->bigger_better;

	float best_value =_nd_of(best)->value;

	_Node_data *ndn = _nd_of(n);
	float nv;
	if (ndn->pass ==2 )
		nv = ndn->value;
	else
		nv = ndn->bound;

	_Ctx_best *cb = ctx;
	if ( bigger_better && nv < best_value || !bigger_better && nv > best_value  )
			enqueue(cb->q,n);

}

static int _prune_inferior_nodes(Tree *sst){
	_Ctx_best ctx;
	ctx.sst = sst;
	ctx.best =0;
	ctx.q= newQueue();

	tree_foreach(sst->root,&ctx,_prune_collect);
	int result =ctx.q->n;
	while (queue_is_empty(ctx.q))
		_kill_node(sst,(TreeNode *)dequeue(ctx.q));
	freeQueue(ctx.q,0);
	return result;
}

static int _check_inferior(Tree *sst,PData ans_incr){
	_Tree_data *td = _td_of(sst);
	TreeNode *b = td->best_solution_sofar;
	float bv = _nd_of(b)->value;
	int bb = td->bigger_better;
	_Node_data *nd = ans_incr;
	float v = nd->value;
	if (! _better_than(bb,v,bv))
		return 0;
	else
		return 1;

}

static int _check_optimal(Tree *sst){
	//if coming to cursor is a single path from root
				//	the optimal solution found.
	TreeNode *n = sst->root;
	while (n){
		if (n->child_count > 1 )
			return 0;
		n = n->first_child;
	}
	return 1;
}

static int _pass_bound(Tree *sst,PData a_i,PData ctx){
	_Tree_data *td = _td_of(sst);
	TreeNode *best_sofar = td->best_solution_sofar;

	if (best_sofar){
		int bigger_better = td->bigger_better;
		float b = ((_Node_data *) a_i)->bound;
		float bv= _nd_of(best_sofar)->value;
		if (!_better_than(bigger_better,b,bv))
				return 0;
	}
	return 1;
}


int branch_bound(Tree *sst,PData ans_incr,int ans_size,Bb_callbacks *cb,PData ctx){
	sst->cursor = _get_best_bound(sst);
	while (sst->cursor){
		int pass12 = 0,pass2 = 0;

		//begin to branch out cursor
		int bFound = cb->get_first_child(sst,ans_incr,ctx);
		while (bFound){
			int iPass = cb->pass(sst,ans_incr,ctx);
			if(iPass){
				cb->get_bound(sst,ans_incr,ctx);
				if (!_pass_bound(sst,ans_incr,ctx))
					iPass = 0;
			}
			if (iPass ==2 ){
				float value = cb->get_value(sst,ans_incr,ctx);
				if ( _check_inferior(sst,ans_incr) ){
					//update best bound.
					_Node_data *nd = _malloc_sst_node_data(ans_incr,ans_size);
					_td_of(sst)->best_solution_sofar = tree_append_child(sst->cursor,nd);
					pass2 ++;
					pass12 ++;
				}
			}
			else if ( iPass ==1){
				_Node_data *nd = _malloc_sst_node_data(ans_incr,ans_size);
				tree_append_child(sst->cursor,ans_incr);
				pass12 ++;
			}

			bFound = cb->get_next_sibling(sst,ans_incr,ctx);
		}
		//end of branch out cursor

		if (pass2){
				_prune_inferior_nodes(sst);
				if (_check_optimal(sst))
					return 1;
		}
		else if (pass12 ==0)
			if ( _kill_node(sst,sst->cursor) )
				return 0; //no solution. teminate.

		sst->cursor = _get_best_bound(sst);
	}

}

PData bb_get_answer_step(PData sstNodeData){
	_Node_data *nd= sstNodeData;
	return nd->data;
}

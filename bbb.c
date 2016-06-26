#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bbb.h"
#include "g.h"
#include "heap.h"


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
	if (!path->root)
		path->root = newTreeNode();
	path->cursor = path->root;
	int bFound = cb->get_first_child(path,ans_incr,ctx);
	while(bFound){
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
	}
	return 0;
}

//Branch and bound method;
typedef struct{
	Tree *sst;
	TreeNode *best;
	Queue *q;
}_Ctx_best;

typedef struct {
	int pass;
	float value;
	float bound;
	int i_in_q; // index in min_queue;
	PData data;
}_Node_data;

typedef struct {
	TreeNode *best_solution_sofar;
	//treenode best_bound_node;
	int bigger_better;
	Heap *min_queue;
	PData data;
}_Tree_data;

#define _nd_of(n)  ((_Node_data *)((n)->data))
#define _td_of(t)  ((_Tree_data *)((t)->data))
#define _better_than(r,a,b) ( (r) ? (a)>(b) : (a)<(b) )
//#define _better_than(r,a,b) ( (r) ? ((a)>(b)?1:0) : ((a)<(b)?1:0) )


//for debug
/*
typedef struct {
	int i;
	ListNode *edge_node;
	float value;
	float bound;
}_Answer_step;  //type of  AnswerList -> Node->data.

static int _print_node(TreeNode *n,PData ctx){
	G *g =ctx;
	_Node_data *nd = _nd_of(n);
	if (!nd){
		printf("root:%x\n",n);
		return 0;
	}
	_Answer_step *as = nd->data;
	G_e *e = as->edge_node->data;
	printf("addr:%x, parent:%x, idx:%d, iBegin:%s, iEnd:%s, weigth:%f, bound:%f, pass=%d, value:%f \n"
		,n,n->parent,as->i,g->v[e->iBegin].name,g->v[e->iEnd].name,e->weight,as->bound,nd->pass,as->value);
}
*/
/*
typedef struct {
	int iPerson;
	int iJob;
	int value;
	int bound;
}_Answer_step;  //type of  AnswerList -> Node->data.

static int _print_node(TreeNode *n,PData ctx){

	_Node_data *nd = _nd_of(n);
	if (!nd){
		printf("root:%x\n",n);
		return 0;
	}
	_Answer_step *as = nd->data;
	printf("addr:%x, parent:%x,pass=%d,i_in_q:%d\n"
		,n,n->parent,nd->pass,nd->i_in_q);
}

static int _print_sst(Tree *sst,PData ctx){
	printf("\n***Begin print tree\n");
	tree_foreach(sst->root,ctx,_print_node);
	printf("***END print tree\n");
}
*/

static int _kill_node(Tree *sst,TreeNode *node){
	_Tree_data *td = _td_of(sst);
	_Node_data *nd =  _nd_of(node);
		if (nd->i_in_q != -1)
			max_heap_delete(td->min_queue,nd->i_in_q);
	while(node != sst->root && node->first_child ==NULL){
		TreeNode *parent = node->parent;
		tree_delete(node);
		free(_nd_of(node)->data);
		free(node->data);
		free(node);

		node = parent;
	}
	if (node->first_child)
		return 1;
	else
		return 0;
}

static PData _malloc_sst_node_data(PData new_sst_node_data,int ans_step_size){
	_Node_data *nd = malloc(sizeof(_Node_data));
	memcpy(nd,new_sst_node_data,sizeof(_Node_data));

	PData new_ans_step = malloc(ans_step_size);
	memcpy(new_ans_step,((_Node_data *) new_sst_node_data)->data,ans_step_size);

	nd->data = new_ans_step;
	return nd;
}

static int _get_best_bound_node(TreeNode *n,PData ctx){
	//printf("entered getbestbound");
	if (n->first_child)
		return 0;
	if (n->parent ==NULL) //root
		return 0;
	_Node_data *nd = _nd_of(n);
	if ( !nd || nd->pass != 1)
		return 0;
	//printf("\n leaf");

	_Ctx_best *cb = (_Ctx_best *)ctx;

	if (! cb->best)
		cb->best = n;
	else{
		int bigger_better = _td_of(cb->sst)->bigger_better;
		float nb = nd->bound;
		float bb =_nd_of(cb->best)->bound;
		//printf("\nnb=%f,bb=%f\n",nb,bb);
		if ( _better_than(bigger_better,nb,bb) )
			cb->best  = n;
	}
}

//to-do: can be implemented by a priority queue.
//but in most case it can't improve the upper bound of branch-bound-algorithm's efficiency.
//because _prune_inferior_nodes will cost O(tree_node_count) anyway.
static  TreeNode  *_get_best_bound(Tree *sst){
	_Ctx_best ctx;
	ctx.sst =sst;
	ctx.best =NULL;
	tree_foreach(sst->root,&ctx,_get_best_bound_node);
	//printf("\n best_bound= %f",_nd_of(ctx.best)->bound);
	return ctx.best;
}

static int _prune_collect(TreeNode *n,PData ctx){
	if (n->first_child)
		return 0;
    _Ctx_best *cb = ctx;
	Tree *sst = cb->sst;
	TreeNode *best = _td_of(sst)->best_solution_sofar;
	int bigger_better = _td_of(sst)->bigger_better;
    if (!best)
        return 0;
    if (best ==n)
        return 0;
	float best_value =_nd_of(best)->value;

	_Node_data *ndn = _nd_of(n);
	float nv;
	if (ndn->pass ==2 )
		nv = ndn->value;
	else
		nv = ndn->bound;

	//if ( bigger_better && nv < best_value || !bigger_better && nv > best_value  )
	if (!_better_than(bigger_better,nv,best_value))
			enqueue(cb->q,n);
}

static int _prune_inferior_nodes(Tree *sst){
	_Ctx_best ctx;
	ctx.sst = sst;
	if (! _td_of(sst)->best_solution_sofar)
		return 0;
	ctx.best =0;
	ctx.q= newQueue();

	tree_foreach(sst->root,&ctx,_prune_collect);
	int result =ctx.q->n;
	while (! queue_is_empty(ctx.q))
		_kill_node(sst,(TreeNode *)dequeue(ctx.q));
	freeQueue(ctx.q,0);
	return result;
}

/*
static int _check_inferior(Tree *sst,PData new_sst_node_data){
	_Tree_data *td = _td_of(sst);
	TreeNode *b = td->best_solution_sofar;
	if (!b)
        return 1;
	float bv = _nd_of(b)->value;
	int bb = td->bigger_better;
	_Node_data *nd = new_sst_node_data;
	float v = nd->value;
	if (! _better_than(bb,v,bv))
		return 0;
	else
		return 1;
}
*/

static int _check_optimal(Tree *sst){
	//if coming to cursor is a single path from root
				//	the optimal solution found.
	for (TreeNode *n = sst->root;n;n = n->first_child){
		if (n->first_child != n->last_child )
			return 0;
		_Node_data *nd = _nd_of(n);
		if (nd && nd->pass ==2){
			//_td_of(sst)->best_solution_sofar = n;
			return 1;
		}
	}
	return 0;
}

int bb_check_bound(Tree *sst,float a_bound){
	_Tree_data *td = _td_of(sst);
	TreeNode *best_sofar = td->best_solution_sofar;

	if (best_sofar){
		int bigger_better = td->bigger_better;
		float bv= _nd_of(best_sofar)->value;
		if (!_better_than(bigger_better,a_bound,bv))
				return 0;
	}
	return 1;
}


/*
static int _check_bound(Tree *sst,PData new_sst_node_data,PData ctx){
	_Tree_data *td = _td_of(sst);
	TreeNode *best_sofar = td->best_solution_sofar;

	if (best_sofar){
		int bigger_better = td->bigger_better;
		float b = ((_Node_data *) new_sst_node_data)->bound;
		float bv= _nd_of(best_sofar)->value;
		if (!_better_than(bigger_better,b,bv))
				return 0;
	}
	return 1;
}
*/

static int _big_compare_bound(PData a,PData b){
	TreeNode *na = a;
	TreeNode *nb = b;
	_Node_data *da=_nd_of(na);
	_Node_data *db=_nd_of(nb);

	if (da->bound == db->bound)
		return 0;
	else
		return da->bound > db->bound ? 1 : -1;

}

static int _small_compare_bound(PData a,PData b){
	return -_big_compare_bound(a,b);
}

static int _on_track_tree_node(enum Op_type ot,int i,PData di,int j,PData dj){
	TreeNode *tn_i = di;
	_Node_data *nd_i = _nd_of(tn_i);
	switch (ot){
		case otSwap:
			nd_i->i_in_q = i;
			TreeNode *tn_j = dj;
			_Node_data *nd_j = _nd_of(tn_j);
			nd_j->i_in_q = j;
			break;
		case otSet:
			nd_i->i_in_q = i;
			break;
		case otRemove:
			nd_i->i_in_q = -1;
			break;
		case otInsert:
			nd_i->i_in_q = i;
			break;
	}
	return 0;
}




int branch_bound(Tree *sst,PData new_ans_step,int ans_size,Bb_callbacks *cb,int bigger_better,PData ctx){
	_Tree_data *td = sst->data;
	if (! td){
		td = malloc(sizeof(_Tree_data));
		td->bigger_better = bigger_better;
		td->best_solution_sofar = NULL;
		sst->data = td;
	}
	if (!sst->root)
		sst->root = newTreeNode();

	if (bigger_better)
		td->min_queue = newHeap(_DEFAULT_SIZE,_big_compare_bound);
	else
		td->min_queue = newHeap(_DEFAULT_SIZE,_small_compare_bound);
	td->min_queue->on_track = _on_track_tree_node;
	sst->cursor = sst->root;//branch out root
	_Node_data new_sst_node_data;
	new_sst_node_data.data = new_ans_step;
	new_sst_node_data.i_in_q =-1;
	while (sst->cursor){
		int pass12 = 0,pass2 = 0;
		//begin to branch out cursor
		int bFound = cb->get_first_child(sst,new_ans_step,ctx);
		while (bFound){
			int iPass = cb->pass(sst,new_ans_step,ctx);
			if(iPass){
				new_sst_node_data.bound = cb->get_bound(sst,new_ans_step,ctx);
				if (!bb_check_bound(sst,new_sst_node_data.bound))
					iPass = 0;
			}
			new_sst_node_data.pass = iPass;
			if (iPass ==2 ){
				new_sst_node_data.value = cb->get_value(sst,new_ans_step,ctx);
				new_sst_node_data.bound = new_sst_node_data.value;
				if ( bb_check_bound(sst,new_sst_node_data.bound) ){
					//update best bound.
					_Node_data *nd = _malloc_sst_node_data(&new_sst_node_data,ans_size);
					TreeNode *tn = tree_append_child(sst->cursor,nd);
					//max_heap_insert(td->min_queue,tn);
					_td_of(sst)->best_solution_sofar = tn;
					pass2 ++;
					pass12 ++;
				}
			}
			else if ( iPass ==1){
				_Node_data *nd = _malloc_sst_node_data(&new_sst_node_data,ans_size);
				TreeNode *tn = tree_append_child(sst->cursor,nd);
				max_heap_insert(td->min_queue,tn);
				pass12 ++;
			}

			bFound = cb->get_next_sibling(sst,new_ans_step,ctx);
		}
	//	_print_sst(sst,ctx);
		//end of branch out cursor
		if (pass2)
			_prune_inferior_nodes(sst);
		else if (pass12 ==0)
			_kill_node(sst,sst->cursor);
        
        sst->cursor = heap_extract_max(td->min_queue);
	}
	/*printf("final sst print:\n");
	_print_sst(sst,ctx);*/
	int result= _check_optimal(sst);
	sst->data = td->best_solution_sofar;
	freeHeap(td->min_queue,0);
	free(td);
	return result;
}

PData bb_get_answer_step(TreeNode *tn){
	_Node_data *nd= tn->data;
	return nd->data;
}

static int _free_node_data(TreeNode *n,PData ctx){
	_Node_data *nd = n->data;
    if (nd){
        free(nd->data);//free answer_step pointer inside of node-data
        free(nd);
    }
	n->data = NULL;
}

void free_all_sst_node_data(Tree *sst){
	if (!sst->root)
		return;
	tree_foreach(sst->root,NULL,_free_node_data);
}


float bb_get_optimal(Tree *sst){
	float result =0;
	TreeNode *op = sst->data;
	_Node_data *nd = _nd_of(op);
	if (op)
		result = _nd_of(op)->value;
	//printf("node addr:%x value = %f ",op,nd->value);
	return result;
}

//this file is for back_track and branch-and-bound method.
//build command: gcc -o tsp bbb_TSP.c bbb.c g.c heap.o ds.c tree.c

#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"
#include "g.h"
#include "bbb.h"
#include "common.h"

typedef struct {
	int i;
	ListNode *edge_node;
	float value;
	float bound;
}_Answer_step;  //type of  AnswerList -> Node->data.


static G_e *_edge_of_tree_node(TreeNode *tn){
	_Answer_step *as = bb_get_answer_step(tn);
	ListNode *edge_node = as->edge_node;
	G_e *edge = edge_node->data;
	return edge;
}

static G_e *_edge_of_answer_step(_Answer_step *as){
	ListNode *edge_node = as->edge_node;
	G_e *edge = edge_node->data;
	return edge;
}


/*
static float _calc_value(Tree *sst,PData answer_step,PData ctx){
	G *g =ctx;
	_Answer_step *si = answer_step;
	float result = _edge_of_answer_step(si)->weight;
	for (TreeNode *tn = sst->cursor;tn != sst->root;tn = tn->parent)
		result += _edge_of_tree_node(tn)->weight;
	return result;
}
*/

//time is O(1)
static float _calc_value(Tree *sst,PData answer_step,PData ctx){
	_Answer_step *si = answer_step;
	float result = _edge_of_answer_step(si)->weight;
	if (sst->cursor != sst->root){
		_Answer_step *asc =  bb_get_answer_step(sst->cursor);
		result += asc->value;
	}
	return result;
}



static int _same_edge(G_e *a,G_e *b){
	if (a->iBegin == b->iBegin && a->iEnd == b->iEnd)
		return 1;
	else if (a->iEnd == b->iBegin && a->iBegin ==b->iEnd)
		return 1;
	else
		return 0;
}

//_calc_bound_1 using Dset, bound can be gotten almost in O(v) time for each call.
//_calc_bound_2 is O(v^2)
//there is an O(1) method. similiar to bbb_knapsack.c->value -calc method. it will be
//implemented in _calc_bound_3
static float _calc_bound_1(Tree *sst,PData answer_step,PData ctx){
	float result =0,pp=0,pi=0,pr=0,pir=0,p0r=0;
	G *g =ctx;
    G_e *e,*em1,*em2;
    ListNode *n;
	Dset *sv =(Dset *)malloc(sizeof(Dset)*g->n);
	for (int i=0;i<g->n;i++)
		set_make(&(sv[i]));
	for (TreeNode *tn = sst->cursor;tn != sst->root;tn = tn->parent){
		e = _edge_of_tree_node(tn);
		int v = e->iEnd;
		set_union(&(sv[0]),&(sv[v]));
		pp += e->weight * 2;
	}
	e = _edge_of_answer_step(answer_step);
	set_union(&(sv[0]),&(sv[e->iEnd]));
	pi += e->weight * 2;

	for (int i=0;i<g->n;i++){
		if (set_find(&(sv[i])) != set_find(&(sv[0])) ){
			n = g->v_adj[i]->head;
			em1 = n->data;
			n = g->v_adj[i]->head->next;
			em2 =n->data;
			pr += em1->weight +em2->weight;
		}
	}

    //now only 0 and last->iEnd need to be considered.
	//find the second least edge adjacent to 0 and last-v.
	int v = e->iEnd;
	n = g->v_adj[v]->head;
	em1 = n->data;
	n = g->v_adj[v]->head->next;
	em2 =n->data;
	if (_same_edge(e,em1))
		pir += em2->weight;
	else
		pir += em1->weight;

	if (sst->root->first_child){
        e = _edge_of_tree_node(sst->root->first_child);
        n = g->v_adj[0]->head;
        em1 = n->data;
        n = g->v_adj[0]->head->next;
        em2 =n->data;
        if (_same_edge(e,em1))
            p0r += em2->weight;
        else
            p0r += em1->weight;
	}
	//vertex 0 is OK, now to vertex	the last one.

	//also, last vertex is completed now.
	result += pp+pi+pr + pir +p0r;

	free(sv);
	return (int)(result / 2.0 + 0.5 );
}


//time bound is O(v^2).  not efficient.
static float _calc_bound_2(Tree *sst,PData answer_step,PData ctx){
	G *g =ctx;
	_Answer_step *si = answer_step;
	G_e *ei = _edge_of_answer_step(si);
	float result =0;
	for (int i=0;i<g->n;i++){
		ListNode *n = g->v_adj[i]->head;
		G_e *em1 = n->data;
		n = g->v_adj[i]->head->next;
		G_e *em2 =n->data;

		G_e *ep1 =NULL, *ep2 =NULL;
		if (ei->iBegin ==i || ei->iEnd ==i)
			ep1 = ei;

		for (TreeNode *tn=sst->cursor;ep2==NULL && tn!=sst->root;tn=tn->parent){
			G_e *ep = _edge_of_tree_node(tn);
			if (ep->iBegin ==i || ep->iEnd == i){
				if (ep1==NULL)
					ep1 = ep;
				else
					ep2 = ep;
			}
		}
		if (ep2){
			em1 = ep1;
			em2 = ep2;
		}
		else if (ep1){
			if (!_same_edge(ep1,em1) && !_same_edge(ep1,em2))
				em2 = ep1;
		}
		result += em1->weight +em2->weight;
	}
	return (int)(result / 2.0 + 0.5 );
}

//average time is O(1).
static float _calc_bound_3(Tree *sst,PData answer_step,PData ctx){
	G *g =ctx;
	_Answer_step *si = answer_step;
	G_e *ei = _edge_of_answer_step(si);
	ListNode *n;
	G_e *e1,*em1,*em2;
	int vi = ei->iEnd;
	float result =0;
	result = ei->weight*2;
	if (sst->cursor == sst->root){
		for (int i=0;i<g->n;i++){
			n = g->v_adj[i]->head;
			em1 = n->data;
			n = g->v_adj[i]->head->next;
			em2 =n->data;
			result += em1->weight +em2->weight;
		}
	}
	else{
		_Answer_step *asc = bb_get_answer_step(sst->cursor);
		result += asc->bound *2;
	}

	int t;
	if (sst->cursor == sst->root)
		t =0;
	else
		t = _edge_of_tree_node(sst->cursor)->iEnd;
	n = g->v_adj[t]->head;
	em1 = n->data;
	n = g->v_adj[t]->head->next;
	em2 =n->data;
	if (_same_edge(ei,em1))
		result -= em1->weight;
	else
		result -= em2->weight;

	t = ei->iEnd;
	n = g->v_adj[t]->head;
	em1 = n->data;
	n = g->v_adj[t]->head->next;
	em2 =n->data;
	if (_same_edge(ei,em1))
		result -= em1->weight;
	else
		result -= em2->weight;

	return (int)(result / 2.0 + 0.5 );
}

//time bound :O(v)
static int _pass(Tree *sst,PData a_i,PData ctx){
	G *g = ctx;
	_Answer_step *si = a_i;
	int v = _edge_of_answer_step(si)->iEnd;
	int v_count = si->i + 1;
	if( v_count == g->n + 1){
		if (0 ==v )
			return 2;
		else
			return 0;
	}
	else if (v_count > g->n +1 )
        return 0;
	else{
		if (v == 0)
			return 0;
		int f1 =0;
		for (TreeNode *n= sst->cursor;n != sst->root;n=n->parent){
			int vp = _edge_of_tree_node(n)->iEnd;
			if (vp == v)
				return 0;
			//if (v==1 && vp==2)
			//	return 0;
			if (vp == 1)
				f1 =1;
		}
		if (v==2 && f1 ==0)//vertex 1 must before vertex 2
			return 0;
	}
	return 1;
}


static int _get_first_child(Tree *sst,PData a_i,PData ctx){
	G *g = ctx;
	_Answer_step *si = a_i;
	int u;
	if (sst->cursor == sst->root){
		u = 0;
		si->i = 1;
	}
	else{
		_Answer_step *ast = bb_get_answer_step(sst->cursor);
		u = _edge_of_answer_step(ast)->iEnd;
		si->i = ast->i + 1;
	}
	ListNode *node = g->v_adj[u]->head;
	si->edge_node = node;
	//since calc bound is O(1) but _pass is O(v), so calc and check bound before pass
	if (si->edge_node){
		si->value = _calc_value(sst,a_i,ctx);
		si->bound = _calc_bound_3(sst,a_i,ctx);
	}
	return  (si->edge_node != NULL) && bb_check_bound(sst,si->bound);
}

static int _get_next_sibling(Tree *sst,PData a_i,PData ctx){
	_Answer_step *si = a_i;
	si->edge_node = si->edge_node->next;
	//since calc bound is O(1) but _pass is O(v), so calc and check bound before pass
	if (si->edge_node){
		si->value = _calc_value(sst,a_i,ctx);
		si->bound = _calc_bound_3(sst,a_i,ctx);
	}
	
	return (si->edge_node != NULL && bb_check_bound(sst,si->bound));
}



static float _get_value(Tree *sst,PData answer_step,PData ctx){
	_Answer_step *si = answer_step;
	return si->value;
	//return _calc_value(sst,answer_step,ctx);
}

static float _get_bound(Tree *sst,PData answer_step,PData ctx){
	_Answer_step *si = answer_step;
	return si->bound;
	//return _calc_bound_3(sst,answer_step,ctx);
}


static ListNode *_min(ListNode *a,ListNode *b){
		if (a==NULL)
			return b;
		else if (b==NULL)
			return a;

		G_e *ae = a->data;
		float aw= ae->weight;

		G_e *be = b->data;
		float bw= be->weight;

		return aw<bw? a:b;
}

static void _swap(ListNode *a,ListNode *b){
	if (a==NULL || b==NULL)
			return;
	G_e *e = a->data;
	a->data = b->data;
	b->data = e;
	G_e *e2 = a->data;
	G_e *e3 = b->data;
}

//get the two shortest adj_edge to the front of it's adj_list.
static int _prepare(G *g){
	//G_e *min1,*min2;
	for (int i=0;i<g->n;i++){
		if (g->v_adj[i]->head ==NULL || g->v_adj[i]->head->next == NULL)
			return 0;

		ListNode *mn =NULL;
		for(ListNode *n = g->v_adj[i]->head;n;n=n->next)
			mn = _min(mn,n);
		_swap(g->v_adj[i]->head,mn);

		mn =NULL;
		for(ListNode *n = g->v_adj[i]->head->next;n;n=n->next)
			mn = _min(mn,n);
		_swap(mn,g->v_adj[i]->head->next);
	}
	return 1;
}

static int _print_answer(Tree *sst,PData ctx){
	G *g = ctx;
	printf("solution found, begin printing solution: \n");
	printf("\toptimal value:%f\n",bb_get_optimal(sst));
	printf("\t%s -> ",g->v[0].name);

	for (TreeNode *n = sst->root->first_child;n;n=n->first_child)
		printf("%s -> ",g->v[ _edge_of_tree_node(n)->iEnd ].name);

	printf("\nend printing solution.");
}


int TSP(G *g){
	Bb_callbacks cb;
		cb.pass =_pass;
		cb.get_first_child = _get_first_child;
		cb.get_next_sibling = _get_next_sibling;
		cb.get_value = _get_value;
		cb.get_bound = _get_bound;
	Tree *sst =newTree();
	_Answer_step s;
	if (_prepare(g)){
		if( branch_bound(sst,&s,sizeof(_Answer_step),&cb,0,g))
			_print_answer(sst,g);
		else
			printf("no solution\n");
	}
	else
		printf("this kind of graph have no hamilton circuit.");
	free_all_sst_node_data(sst);
	freeTree(sst,1);
}

int main(){
	G *g =newG(5);
	//{  assign vertices and edges
		assignVertex(g,0,"a");
		assignVertex(g,1,"b");
		assignVertex(g,2,"c");
		assignVertex(g,3,"d");
		assignVertex(g,4,"e");

		addEdge_NoDirection(g,0,1,3);
		addEdge_NoDirection(g,0,2,1);
		addEdge_NoDirection(g,0,3,5);
		addEdge_NoDirection(g,0,4,8);

		addEdge_NoDirection(g,1,2,6);
		addEdge_NoDirection(g,1,3,7);
		addEdge_NoDirection(g,1,4,9);

		addEdge_NoDirection(g,2,3,4);
		addEdge_NoDirection(g,2,4,2);

		addEdge_NoDirection(g,3,4,3);

	//}
	TSP(g);

	freeG(g);
}

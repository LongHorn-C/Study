//this file is for back_track and branch-and-bound method.
//for Hamiltonian circuit problem.
//build command: gcc -o hm bbb_Hamilton.c bbb.c g.c heap.o ds.c tree.c

#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"
#include "g.h"
#include "common.h"

typedef struct {
	int i;
	ListNode *edge_node;
}_Answer_step;  //type of  AnswerList -> Node->data.

static G_e *_edge_of_tree_node(TreeNode *tn){
	_Answer_step *as = tn->data;
	ListNode *edge_node = as->edge_node;
	G_e *edge = edge_node->data;
	return edge;	
}

static G_e *_edge_of_answer_step(_Answer_step *as){
	ListNode *edge_node = as->edge_node;
	G_e *edge = edge_node->data;
	return edge;	
}

static int _pass(Tree *path,PData a_i,PData ctx){
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
		for (TreeNode *n= path->cursor;n != path->root;n=n->parent){
			int vp = _edge_of_tree_node(n)->iEnd;
			if (vp == v)
				return 0;
			//if (v==1 && vp==2)//vertex 1 must before vertex 2
			//	return 0;
		}
	}
	return 1;
}

static int _get_first_child(Tree *path,PData a_i,PData ctx){
	G *g = ctx;
	_Answer_step *si = a_i;
	int u;
	if (path->cursor == path->root){
		u = 0;
		si->i = 1;
	}
	else{
		_Answer_step *ast = path->cursor->data;
		u = _edge_of_answer_step(ast)->iEnd;
		si->i = ast->i + 1;
	}
	ListNode *node = g->v_adj[u]->head;
	si->edge_node = node;
	return  (si->edge_node != NULL);
}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	_Answer_step *si = a_i;
	si->edge_node = si->edge_node->next;

	return si->edge_node != NULL;
}

/*
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
*/

static int _print_answer(Tree *sst,PData ctx){
	G *g = ctx;
	printf("solution found, begin printing solution: \n");
	printf("\t%s -> ",g->v[0].name);
	for (TreeNode *n = sst->root->first_child;n;n=n->first_child)
		printf("%s -> ",g->v[ _edge_of_tree_node(n)->iEnd ].name);
	
	printf("\nend printing solution.");	
}

int Hamilton(G *g){
	Bt_callbacks cb;
	//{
		cb.pass =_pass;
		cb.get_first_child =_get_first_child;
		cb.get_next_sibling =_get_next_sibling;
	//}

    Tree * sst = newTree();
	_Answer_step a_i;
	//_prepare(g);
	if (back_track(sst,&a_i,sizeof(_Answer_step),&cb,g))
		_print_answer(sst,g);
	else
		printf("no solution\n");	
	freeTree(sst,1);
}

int main(){
	G *g =newG(6);
	//G *g =newG(5);
	//{  assign vertices and edges
		assignVertex(g,0,"a");
		assignVertex(g,1,"b");
		assignVertex(g,2,"c");
		assignVertex(g,3,"d");
		assignVertex(g,4,"e");
		assignVertex(g,5,"f");

		addEdge_NoDirection(g,0,1,1);
		addEdge_NoDirection(g,0,2,1);
		addEdge_NoDirection(g,0,3,1);

		addEdge_NoDirection(g,1,2,1);
		addEdge_NoDirection(g,1,5,1);

		addEdge_NoDirection(g,2,3,1);
		addEdge_NoDirection(g,2,4,1);

		addEdge_NoDirection(g,3,4,1);

		addEdge_NoDirection(g,4,5,1);
		/*assignVertex(g,0,"a");
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

		addEdge_NoDirection(g,3,4,3);*/
	//}
	Hamilton(g);

	freeG(g);
}

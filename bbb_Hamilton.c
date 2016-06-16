//this file is for back_track and branch-and-bound method.
//for Hamiltonian circuit problem.
//answer_list.node.data is a pointer to G.adj[i].ListNode,
#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"
#include "g.h"
#include "common.h"

//to do :deal with the last node return to the first.
static int _pass(List *path,PData a_i,PData ctx){
	G *g = ctx;
	ListNode *edge_node = a_i;
	G_e *edge = edge_node->data;
	int v = edge->iEnd;

	if( path->n == g->n ){
		edge_node = path->head->data;
		edge = edge_node->data;
		int v0 = edge->iEnd;

		if (v0 ==v )
			return 2;
	}
	else if (path->n > g->n)
        return 0;
	else{
		ListNode *n= path->head;
		while(n){
			edge_node = n->data;
			edge = edge_node->data;
			int vp = edge->iEnd;
			if (vp == v)
				return 0;

			n = n->next;
		}
	}
	return 1;
}


static int _get_first_child(List *path,PData a_i,PData ctx){
	G *g = ctx;

	ListNode *ans_tail_node = path->tail;
	ListNode *node = ans_tail_node->data;
	G_e *edge = node->data;
	int u = edge->iEnd;

	node = g->v_adj[u]->head;

	if (node)
		*(ListNode *)a_i = *node;

	return  (node != NULL);

}

static int _get_next_sibling(List *path,PData a_i,PData ctx){
	ListNode * node = a_i;
	node = node->next;

	if (node)
		*(ListNode *)a_i = *node;

	return node != NULL;
}

static int _print_node(ListNode *n,PData ctx){
	G *g= ctx;
	ListNode *edge_node = (ListNode *)n->data;
	G_e *edge= (G_e *)edge_node->data;
	printf("%s -> ",g->v[edge->iEnd].name);
}


int Hamilton(G *g){
	Bt_callbacks cb;

	cb.pass =_pass;
	cb.get_first_child =_get_first_child;
	cb.get_next_sibling =_get_next_sibling;


    List * ans = newList();
	ListNode a;
	a.next =NULL;
	G_e e;
	e.iBegin = 0;
	e.iEnd = 0;
	e.weight = 0;
	e.data = (PData)-1;
	a.data = &e;
	list_append(ans,&a);

	ListNode node;
	_get_first_child(ans,&node,g);

	if (back_track(ans,&node,sizeof(ListNode),&cb,g))
		list_foreach(ans,_print_node,g);
	freeList(ans,0);

}

void main(){
	G *g =newG(6);
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

	Hamilton(g);
}

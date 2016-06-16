//this file is for back_track and branch-and-bound method.
//for Hamiltonian circuit problem.
//answer_list.node.data is a pointer to G.adj[i].ListNode,
#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"
#include "common.h"

//to do :deal with the last node return to the first.
static int _pass(List *a_p_p,PData a_i,PData ctx)
	G *g =(G *)ctx;
	int v=((G_e *)((ListNode *)a_i)->data)->iEnd;
	
	if(a_p_p->n < g->n){
		int vn;
		ListNode *n= a_p_p->head;
		while(n){
			vn = ((G_e *)n->data)->iEnd;
			if ( a_p_p->n < g->n+1 && vn ==v)
				return 0;
			
			n=n->next;
		}	
	}
	else if( a_p_p->n == g->n ){
		int v0 = ((G_e *)a_p_p->head->data)->iEnd;
		if (v0 ==v )
			return 2;
	}
	
	return 1;
}

/*
static int _is_answer(List *a_p_p,PData ctx){
	G *g =(G *)ctx;
	return (a_p_p->n == g->n+1);
}
*/

static int _first_child(List *a_p_p,PData a_i,PData ctx){
	G *g =(G *)ctx;
	G_e *e,*e1;
	ListNode n = a_p_p->tail;
	e = (G_e *) n->data;
	int u = e->iEnd;
	e1 = g->adj[u]->head;
	Answer_node ** ppa= (Answer_node **)a_i;
	*ppa = el;
	result =  (el != NULL);
	
}	

static int _next_sibling(List *a_p_p,PData a_i,PData ctx){
	Answer_node ** ppa= (Answer_node **)a_i;
	*ppa = (*ppa)->next;
	return *a_i != NULL;	
}

static _print_result(List *a,PData ctx){
	G *g= (G *)ctx;
	ListNode *n= a_p_p->head;
	while (n){
		ListNode *a = (ListNode *)n->data;
		G_e *e= (G_e *)a->data;
		printf("%s -> ",g->v[e->iEnd].name);
		n = n->next;
	}
}


int Hamilton(G *g){
	Bt_callbacks cb;
	cb.pass =_pass;
	//cb.is_answer =_is_answer;
	cb.first_child =_first_child;
	cb.next_sibling =_next_sibling;

	
    List * ans = newList();
	ListNode a;
	a.next =NULL;
	G_e e;
	e.iEnd = 0;
	a->data = &e;
	list_append(ans,(PData) &a);

	ListNode **ppn;
	_first_child(ans,ppn,(PData)g);
	
	if (back_track(ans,*ppn,&cb,(PData)g))
		_print_result(ans,g);
	
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

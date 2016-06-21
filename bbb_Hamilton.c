//this file is for back_track and branch-and-bound method.
//for Hamiltonian circuit problem.
//answer_list.node.data is a pointer to _Answer_step *

#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"
#include "g.h"
#include "common.h"

typedef struct {
	int i;
	ListNode *edge_node;
}_Answer_step;  //type of  AnswerList -> Node->data.

#define _ad_of(p) ((_Answer_step *)p)

static int _pass(Tree *path,PData a_i,PData ctx){
	G *g = ctx;
	ListNode *edge_node = _ad_of(a_i)->edge_node;
	G_e *edge = edge_node->data;
	int v = edge->iEnd;
	int v_count = _ad_of(a_i)->i - 1;
	if( v_count == g->n ){
		if (0 ==v )
			return 2;
		else
			return 0;
	}
	else if (v_count > g->n)
        return 0;
	else{
		TreeNode *n= path->cursor;
		while(n && n!= path->root){
			edge_node =_ad_of( n->data)->edge_node;
			edge = edge_node->data;
			int vp = edge->iEnd;
			if (vp == v)
				return 0;

			n = n->parent;
		}
	}
	return 1;
}

/*
static int _on_append_tail(Tree *path,PData a_i,PData ctx){;
	path->data ++;
	return 1;
}

static int _on_delete_tail(Tree *path,PData a_i,PData ctx){;
	path->data --;
	return 1;
}
*/

static int _get_first_child(Tree *path,PData a_i,PData ctx){
	G *g = ctx;

	TreeNode *ans_tail_node = path->cursor;
	ListNode *node = _ad_of(ans_tail_node->data)->edge_node;
	G_e *edge = node->data;
	int u = edge->iEnd;

	node = g->v_adj[u]->head;

	if (node){
		//*(ListNode *)a_i = *node;
		 _ad_of(a_i)->edge_node = node;
		_ad_of(a_i)->i ++;
	}
	return  (node != NULL);

}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	ListNode * node = _ad_of(a_i)->edge_node;
	node = node->next;

	if (node)
		//*(ListNode *)a_i = *node;
		_ad_of(a_i)->edge_node = node;

	return node != NULL;
}

static int _print_ans_node(TreeNode *n,PData ctx){
	G *g= ctx;
	if (n->data){
		ListNode *edge_node = _ad_of(n->data)->edge_node;
		G_e *edge= (G_e *)edge_node->data;
		printf("%s -> ",g->v[edge->iEnd].name);
	}
}


int Hamilton(G *g){
	Bt_callbacks cb;
	//{
		cb.pass =_pass;
		cb.get_first_child =_get_first_child;
		cb.get_next_sibling =_get_next_sibling;
	//}

	TreeNode *rn =newTreeNode();
    Tree * sst = newTree(rn);
	//sst->data = (PData)0; //count of vertices in state-space-tree.
	sst->cursor = rn;

	ListNode node;
	//{
			G_e e;
			e.iBegin = 0;
			e.iEnd = 0;
			e.weight = 0;
			e.data = NULL;
		//}
		node.data = &e;
		node.next =NULL;
	_Answer_step a_i;
	a_i.i =1;
	a_i.edge_node = &node;

	if (back_track(sst,&a_i,sizeof(node),&cb,g)){
		printf("solution found: \n\t");
		tree_foreach(sst->root,g,_print_ans_node);
	/*	TreeNode * nn= sst->root;
		while (nn){
            _print_ans_node(nn,g);
            nn= nn->first_child;
		}
    */
		printf("\nend print solution\n");

	}
	freeTree(sst,1);
}

int main(){
	G *g =newG(6);
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
	//}
	Hamilton(g);

	freeG(g);
}

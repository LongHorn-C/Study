//this file is for back_track and branch-and-bound method.
//for Hamiltonian circuit problem.
//answer_list.node.data is a pointer to G.v_adj[i].ListNode,

#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"
#include "g.h"
#include "common.h"

static int _pass(Tree *path,PData a_i,PData ctx){
	G *g = ctx;
	ListNode *edge_node = a_i;
	G_e *edge = edge_node->data;
	int v = edge->iEnd;
	int v_count =(int)path->data;
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
			edge_node = n->data;
			edge = edge_node->data;
			int vp = edge->iEnd;
			if (vp == v)
				return 0;

			n = n->parent;
		}
	}
	return 1;
}

static int _on_append_tail(Tree *path,PData a_i,PData ctx){;
	path->data ++;
	return 1;
}

static int _on_delete_tail(Tree *path,PData a_i,PData ctx){;
	path->data --;
	return 1;
}


static int _get_first_child(Tree *path,PData a_i,PData ctx){
	G *g = ctx;

	TreeNode *ans_tail_node = path->cursor;
	ListNode *node = ans_tail_node->data;
	G_e *edge = node->data;
	int u = edge->iEnd;

	node = g->v_adj[u]->head;

	if (node)
		*(ListNode *)a_i = *node;

	return  (node != NULL);

}

static int _get_next_sibling(Tree *path,PData a_i,PData ctx){
	ListNode * node = a_i;
	node = node->next;

	if (node)
		*(ListNode *)a_i = *node;

	return node != NULL;
}

static int _print_ans_node(TreeNode *n,PData ctx){
	G *g= ctx;
	if (n->data){
		ListNode *edge_node = (ListNode *)n->data;
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
		cb.on_append_tail = _on_append_tail;
		cb.on_delete_tail = _on_delete_tail;
	//}

	TreeNode *rn =newTreeNode();
    Tree * sst = newTree(rn);
	sst->data = (PData)0; //count of vertices in state-space-tree.
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
/*  or use following code to construct initial state-space-tree
	TreeNode *rn =newTreeNode();
    Tree * sst = newTree(rn);
	sst->data = (PData)0; //count of vertices in state-space-tree.
	sst->cursor = rn;
	ListNode *rnode =malloc(sizeof(ListNode));
	//{
			G_e e;
			e.iBegin = 0;
			e.iEnd = 0;
			e.weight = 0;
			e.data = NULL;
		//}
		rnode->data = &e;
		//rnode->next =NULL; don't need this clause.
	rn->data = rnode;

	ListNode node;
	node = *(g->v_adj[0]->head);


*/
	if (back_track(sst,&node,sizeof(node),&cb,g)){
		printf("solution found: \n\t");
		tree_foreach(sst->root,_print_ans_node,g);
		printf("end print solution");
		
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

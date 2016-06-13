#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "heap.h"
#include "G.h"
#include "common.h"

static char * testG_BFS(){
	G *g =newG(8);
	assignVertex(g,0,"r");
	assignVertex(g,1,"s");
	assignVertex(g,2,"t");
	assignVertex(g,3,"u");
	assignVertex(g,4,"v");
	assignVertex(g,5,"w");
	assignVertex(g,6,"x");
	assignVertex(g,7,"y");

	addEdge_NoDirection(g,0,1,1);
	addEdge_NoDirection(g,2,3,1);
	addEdge_NoDirection(g,0,4,1);
	addEdge_NoDirection(g,1,5,1);
	addEdge_NoDirection(g,2,5,1);
	addEdge_NoDirection(g,2,6,1);
	addEdge_NoDirection(g,3,7,1);
	addEdge_NoDirection(g,5,6,1);
	addEdge_NoDirection(g,6,7,1);

	int d[8],pi[8];
	int i;

	BFS(g,1,d,pi);

	printf("\nd array: ");
	for (i=0;i<8;i++)
		printf("%d,",d[i]);

	printf("\nPi array:%d ");
	for (i=0;i<8;i++)
		printf("%d,",pi[i]);
	printf("\nPath from 1 to 2:  ");
	print_path(g,1,2,pi);

	freeG(g);
}


static char * testG_DFS(){
	G *g =newG(6);
	assignVertex(g,0,"u");
	assignVertex(g,1,"v");
	assignVertex(g,2,"w");
	assignVertex(g,3,"x");
	assignVertex(g,4,"y");
	assignVertex(g,5,"z");

	addEdge(g,0,3,1);
	addEdge(g,0,1,1);
	addEdge(g,3,1,1);
	addEdge(g,1,4,1);
	addEdge(g,2,5,1);
	addEdge(g,2,4,1);
	addEdge(g,4,3,1);
	addEdge(g,5,5,1);


	int d[6],f[6],pi[6];
	int i;

	DFS(g,d,f,pi,0);

	printf("\nd array: ");
	for (i=0;i<6;i++)
		printf("%d,",d[i]);

	printf("\nf array: ");
	for (i=0;i<6;i++)
		printf("%d,",f[i]);


	printf("\nPi array: ");
	for (i=0;i<6;i++)
		printf("%d,",pi[i]);

	freeG(g);
}



static char * testG_topological_sort(){
	G *g =newG(9);
	assignVertex(g,0,"undersh");
	assignVertex(g,1,"socks");
	assignVertex(g,2,"watch");
	assignVertex(g,3,"pants");
	assignVertex(g,4,"shoes");
	assignVertex(g,5,"belt");
	assignVertex(g,6,"shirt");
	assignVertex(g,7,"tie");
	assignVertex(g,8,"jacket");

	addEdge(g,0,3,1);
	addEdge(g,0,4,1);
	addEdge(g,3,1,1);
	addEdge(g,3,5,1);
	addEdge(g,1,4,1);
	addEdge(g,6,5,1);
	addEdge(g,6,7,1);
	addEdge(g,7,8,1);
	addEdge(g,5,8,1);


	int d[9],f[9],pi[9];
	int i;

	List *ts =newList();
	DFS(g,d,f,pi,ts);

	printf("\nd array: ");
	for (i=0;i<9;i++)
		printf("%d,",d[i]);

	printf("\nf array: ");
	for (i=0;i<9;i++)
		printf("%d,",f[i]);


	printf("\nPi array: ");
	for (i=0;i<9;i++)
		printf("%d,",pi[i]);

	printf("\ntop sort: ");
	ListNode *n=ts->head;
	while(n){
		i = (int)(n->data);
		printf(" -> %s",g->v[i].name);
		n = n->next;
	}

	freeG(g);
	freeList(ts,0);
}

static char * testG_topological_sort_1(){
	G *g =newG(9);
	assignVertex(g,0,"undersh");
	assignVertex(g,1,"socks");
	assignVertex(g,2,"watch");
	assignVertex(g,3,"pants");
	assignVertex(g,4,"shoes");
	assignVertex(g,5,"belt");
	assignVertex(g,6,"shirt");
	assignVertex(g,7,"tie");
	assignVertex(g,8,"jacket");

	addEdge(g,0,3,1);
	addEdge(g,0,4,1);
	addEdge(g,3,1,1);
	addEdge(g,3,5,1);
	addEdge(g,1,4,1);
	addEdge(g,6,5,1);
	addEdge(g,6,7,1);
	addEdge(g,7,8,1);
	addEdge(g,5,8,1);


	int d[9],f[9],pi[9];
	int i;

	List *ts =newList();
	DFS_1(g,d,f,pi,ts);

	printf("\nd array: ");
	for (i=0;i<9;i++)
		printf("%d,",d[i]);

	printf("\nf array: ");
	for (i=0;i<9;i++)
		printf("%d,",f[i]);


	printf("\nPi array: ");
	for (i=0;i<9;i++)
		printf("%d,",pi[i]);

	printf("\ntop sort: ");
	ListNode *n=ts->head;
	while(n){
		i = (int)(n->data);
		printf(" -> %s",g->v[i].name);
		n = n->next;
	}

	freeG(g);
	freeList(ts,0);
}

static char * testG_DFS_1(){
	G *g =newG(6);
	assignVertex(g,0,"u");
	assignVertex(g,1,"v");
	assignVertex(g,2,"w");
	assignVertex(g,3,"x");
	assignVertex(g,4,"y");
	assignVertex(g,5,"z");

	addEdge(g,0,3,1);
	addEdge(g,0,1,1);
	addEdge(g,3,1,1);
	addEdge(g,1,4,1);
	addEdge(g,2,5,1);
	addEdge(g,2,4,1);
	addEdge(g,4,3,1);
	addEdge(g,5,5,1);


	int d[6],f[6],pi[6];
	int i;

	DFS_1(g,d,f,pi,0);

	printf("\nd array: ");
	for (i=0;i<6;i++)
		printf("%d,",d[i]);

	printf("\nf array: ");
	for (i=0;i<6;i++)
		printf("%d,",f[i]);


	printf("\nPi array: ");
	for (i=0;i<6;i++)
		printf("%d,",pi[i]);

	freeG(g);
}


static char * test_mst_Kruskal(){
	G *g =newG(9);
	assignVertex(g,0,"a");
	assignVertex(g,1,"b");
	assignVertex(g,2,"c");
	assignVertex(g,3,"d");
	assignVertex(g,4,"e");
	assignVertex(g,5,"f");
	assignVertex(g,6,"g");
	assignVertex(g,7,"h");
	assignVertex(g,8,"i");

	addEdge_NoDirection(g,0,1,4);
	addEdge_NoDirection(g,1,2,8);
	addEdge_NoDirection(g,2,3,7);
	addEdge_NoDirection(g,3,4,9);
	addEdge_NoDirection(g,4,5,10);
	addEdge_NoDirection(g,5,6,2);
	addEdge_NoDirection(g,6,7,1);
	addEdge_NoDirection(g,7,8,7);

	addEdge_NoDirection(g,0,7,8);
	addEdge_NoDirection(g,1,7,11);
	addEdge_NoDirection(g,2,8,2);
	addEdge_NoDirection(g,8,6,6);
	addEdge_NoDirection(g,2,5,4);
	addEdge_NoDirection(g,3,5,14);


	List *mst = newList();
	mst_Kruskal(g,mst);
	ListNode *n =mst->head;
	G_e *e;
	while (n){
		e = (G_e *) n->data;
		printf("\n(u:%d: %s)\t--\t%f\t->\t(v:%d: %s)",\
							e->iBegin,g->v[e->iBegin].name,e->weight,e->iEnd,g->v[e->iEnd].name);
		n = n->next;
	}

	freeList(mst,0);
	freeG(g);
}

static char * testG_mst_Prim(){
	G *g =newG(9);
	assignVertex(g,0,"a");
	assignVertex(g,1,"b");
	assignVertex(g,2,"c");
	assignVertex(g,3,"d");
	assignVertex(g,4,"e");
	assignVertex(g,5,"f");
	assignVertex(g,6,"g");
	assignVertex(g,7,"h");
	assignVertex(g,8,"i");

	addEdge_NoDirection(g,0,1,4);
	addEdge_NoDirection(g,1,2,8);
	addEdge_NoDirection(g,2,3,7);
	addEdge_NoDirection(g,3,4,9);
	addEdge_NoDirection(g,4,5,10);
	addEdge_NoDirection(g,5,6,2);
	addEdge_NoDirection(g,6,7,1);
	addEdge_NoDirection(g,7,8,7);

	addEdge_NoDirection(g,0,7,8);
	addEdge_NoDirection(g,1,7,11);
	addEdge_NoDirection(g,2,8,2);
	addEdge_NoDirection(g,8,6,6);
	addEdge_NoDirection(g,2,5,4);
	addEdge_NoDirection(g,3,5,14);


	int *pi =(int *)malloc(sizeof(int)*g->n);
	mst_Prim(g,0,pi);

	printf("PI array: ");
	print_ii(pi,9);

	free(pi);
	freeG(g);
}


static char * test_Bellman_Ford(){
	G *g =newG(5);
	assignVertex(g,0,"s");
	assignVertex(g,1,"t");
	assignVertex(g,2,"x");
	assignVertex(g,3,"y");
	assignVertex(g,4,"z");


	addEdge(g,0,1,6);
	addEdge(g,0,3,7);

	addEdge(g,1,2,5);
	addEdge(g,1,3,8);
	addEdge(g,1,4,-4);

	addEdge(g,2,1,-2);

	addEdge(g,3,2,-3);
	addEdge(g,3,4,9);

	addEdge(g,4,0,2);
	addEdge(g,4,2,7);


	float *d =(float *)malloc(sizeof(float)*g->n);
	int *pi =(int *)malloc(sizeof(int)*g->n);

	int ac = Bellman_Ford(g,0,d,pi);
	if (ac)
		printf("acyc graph\n");
	else
		printf("cyclic graph\n");

	printf("d array: ");
	for (int i=0;i<g->n;i++)
		printf("%f,",d[i]);
	printf("\n");

	printf("PI array: ");
	print_ii(pi,5);

	free(d);
	free(pi);
	freeG(g);
}

static char * test_dag_shortest_path(){
	G *g =newG(6);
	assignVertex(g,0,"r");
	assignVertex(g,1,"s");
	assignVertex(g,2,"t");
	assignVertex(g,3,"x");
	assignVertex(g,4,"y");
	assignVertex(g,5,"z");

	addEdge(g,0,1,5);
	addEdge(g,0,2,3);

	addEdge(g,1,2,2);
	addEdge(g,1,3,6);

	addEdge(g,2,3,7);
	addEdge(g,2,4,4);
	addEdge(g,2,5,2);

	addEdge(g,3,4,-1);
	addEdge(g,3,5,1);

	addEdge(g,4,5,-2);


	float *d =(float *)malloc(sizeof(float)*g->n);
	int *pi =(int *)malloc(sizeof(int)*g->n);

	dag_shortest_paths(g,1,d,pi);

	printf("d array: ");
	for (int i=0;i<g->n;i++)
		printf("%f,",d[i]);
	printf("\n");

	printf("PI array: ");
	print_ii(pi,6);

	free(d);
	free(pi);
	freeG(g);
}

static char * test_Dijkstra(){
	G *g =newG(5);
	assignVertex(g,0,"s");
	assignVertex(g,1,"t");
	assignVertex(g,2,"x");
	assignVertex(g,3,"y");
	assignVertex(g,4,"z");

	addEdge(g,0,1,10);
	addEdge(g,0,3,5);

	addEdge(g,1,2,1);
	addEdge(g,1,3,2);

	addEdge(g,2,4,4);

	addEdge(g,3,1,3);
	addEdge(g,3,2,9);
	addEdge(g,3,4,2);

	addEdge(g,4,0,7);
	addEdge(g,4,2,6);


	float *d =(float *)malloc(sizeof(float)*g->n);
	int *pi =(int *)malloc(sizeof(int)*g->n);

	Dijkstra(g,0,d,pi);

	printf("d array: ");
	for (int i=0;i<g->n;i++)
		printf("%f,",d[i]);
	printf("\n");

	printf("PI array: ");
	print_ii(pi,5);

	free(d);
	free(pi);
	freeG(g);
}

static void test_Johnson(){
	G *g =newG(5);
	assignVertex(g,0,"1");
	assignVertex(g,1,"2");
	assignVertex(g,2,"3");
	assignVertex(g,3,"4");
	assignVertex(g,4,"5");

	addEdge(g,0,1,3);
	addEdge(g,0,2,8);
	addEdge(g,0,4,-4);

	addEdge(g,1,3,1);
	addEdge(g,1,4,7);

	addEdge(g,2,1,4);

	addEdge(g,3,0,2);
	addEdge(g,3,2,-5);

	addEdge(g,4,3,6);


	float *d =(float *)malloc(sizeof(float)*g->n * g->n);
	int *pi =(int *)malloc(sizeof(int)*g->n * g->n);

	Johnson(g,d,pi);

	for (int u=0;u<5;u++){
		printf("u:%d\n",u);
		printf("d array: ");
		for (int i=0;i<g->n;i++)
			printf("%f,",d[u*5+i]);
		printf("\n");

		printf("pi array: ");
		print_ii(pi+u*5,5);
		printf("\n*********************\n");
	}

/*	print_ii(pi,25);
	for (int i=0;i<25;i++){
		printf("%f,",d[i]);
	}
	*/
	free(d);
	free(pi);
	freeG(g);

}

static void test_Edmonds_Karp(){
	G *g =newG(6);
	assignVertex(g,0,"s");
	assignVertex(g,1,"v1");
	assignVertex(g,2,"v2");
	assignVertex(g,3,"v3");
	assignVertex(g,4,"v4");
	assignVertex(g,5,"t");

	addEdge(g,0,1,16);
	addEdge(g,0,2,13);

	addEdge(g,1,2,10);
	addEdge(g,1,3,12);

	addEdge(g,2,1,4);
	addEdge(g,2,4,14);

	addEdge(g,3,2,9);
	addEdge(g,3,5,20);

	addEdge(g,4,3,7);
	addEdge(g,4,5,4);


	Edmonds_Karp(g,0,5);

	//print the maximum flow
	printf("\nMaximum flow: ");
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			float flow = * (float *) edge->data;
			printf("\n(%s,%s): \t%f  ",g->v[u].name,g->v[v].name,flow);
			ne = ne->next;
		}

	}
	printf("\n");

	free_graph_flow_data(g);
	freeG(g);

}

static void test_generic_push_relabel(){
	G *g =newG(6);
	assignVertex(g,0,"s");
	assignVertex(g,1,"v1");
	assignVertex(g,2,"v2");
	assignVertex(g,3,"v3");
	assignVertex(g,4,"v4");
	assignVertex(g,5,"t");

	addEdge(g,0,1,16);
	addEdge(g,0,2,13);

	addEdge(g,1,2,10);
	addEdge(g,1,3,12);

	addEdge(g,2,1,4);
	addEdge(g,2,4,14);

	addEdge(g,3,2,9);
	addEdge(g,3,5,20);

	addEdge(g,4,3,7);
	addEdge(g,4,5,4);


	generic_push_relabel(g,0,5);

	//print the maximum flow
	printf("\nMaximum flow: ");
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			float flow = * (float *) edge->data;
			printf("\n(%s,%s): \t%f  ",g->v[u].name,g->v[v].name,flow);
			ne = ne->next;
		}

	}
	printf("\n");

	free_graph_flow_data(g);
	freeG(g);

}


static void test_relabel_to_front(){
	G *g =newG(6);
	assignVertex(g,0,"s");
	assignVertex(g,1,"v1");
	assignVertex(g,2,"v2");
	assignVertex(g,3,"v3");
	assignVertex(g,4,"v4");
	assignVertex(g,5,"t");

	addEdge(g,0,1,16);
	addEdge(g,0,2,13);

	addEdge(g,1,2,10);
	addEdge(g,1,3,12);

	addEdge(g,2,1,4);
	addEdge(g,2,4,14);

	addEdge(g,3,2,9);
	addEdge(g,3,5,20);

	addEdge(g,4,3,7);
	addEdge(g,4,5,4);


	relabel_to_front(g,0,5);

	//print the maximum flow
	printf("\nMaximum flow: ");
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			float flow = * (float *) edge->data;
			printf("\n(%s,%s): \t%f  ",g->v[u].name,g->v[v].name,flow);
			ne = ne->next;
		}

	}
	printf("\n");

	free_graph_flow_data(g);
	freeG(g);

}


int main(){
	
	//test_relabel_to_front();
	test_generic_push_relabel();
	//test_Edmonds_Karp();
	//test_Johnson();
	//test_Dijkstra();
	//test_dag_shortest_path();
	//test_Bellman_Ford();
	//test_mst_Kruskal();
}

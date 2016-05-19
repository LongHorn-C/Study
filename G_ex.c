#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "heap.h"
#include "tcut.h"
#include "G.h"

int nr_tests = 0;


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


static char * all_tests()
{
    //tcut_run_test(testG_BSF);
    //tcut_run_test(test_bar);
    return 0;
}

int main(){
	//testG_DFS();
	//printf("\ntest dfs_1:\n");
	//testG_DFS_1();
	
	testG_topological_sort();
	testG_topological_sort_1();

    /*char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", nr_tests);

    return result != 0;
	*/
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ds.h"
#include "tree.h"

#include "common.h"

int _print(TreeNode *n,PData ctx){
	printf("[data: %d, child_count: %d], ",(int)n->data, n->child_count);
}

void test_tree(){
	TreeNode *r =newTreeNode();
	r->data =(PData) 1;
	TreeNode * n2 = tree_append_child(r,(PData)2);
	TreeNode * n3 = tree_append_child(r,(PData)3);
	TreeNode * n10 = tree_append_child(r,(PData)10);

	TreeNode * n9 = tree_append_child(n2,(PData)9);

	TreeNode * n4 = tree_append_child(n3,(PData)4);
	TreeNode * n5 = tree_append_child(n3,(PData)5);
	TreeNode * n8 = tree_append_child(n3,(PData)8);

	TreeNode * n7 = tree_append_child(n5,(PData)7);

	tree_foreach(r,_print,0);

	printf("\nlocate \n");
	TreeNode *s =tree_search(r,(PData)5,pointerCompare);
	if (s)
		printf("[data: %d, child_count: %d], ",(int)s->data, s->child_count);

	freeTreeNode(r,0);

/*	PData a[10];
	produce_exx(a,10);
	print_exx(a,10);

	List *l =newList();
	for (int i=0;i<10;i++)
		list_append(l,a[i]);


	printf("List :");

	ListNode *n;
	n = l->head;
	printf("begin List:\n");
	while (n){
		print_ex(n->data);
		n = n->next;
	}
	printf("end List:\n");

	printf("test foreach\n");
	list_foreach(l,_print,0);

	freeList(l,1);
*/
}

int main(){
	//typedef enum {RED,WHITE} Color;
	test_tree();
	//test_track();
	//test_heap();
	//Color c = RED;
}



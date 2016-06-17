#ifndef __CLH_TREE_H
#define __CLH_TREE_H
#include "ds.h"

//TreeNode *******************************************
struct tree_node{
	void *data;
	struct tree_node * parent;
	struct tree_node * first_child;
	struct tree_node * last_child;
	struct tree_node * prev;
	struct tree_node * next;
	
	int child_count;
};

typedef struct tree_node TreeNode;

typedef int (*FTreeNode)(TreeNode *n,PData ctx);


typedef struct tree{
	TreeNode *root;
	TreeNode *cursor;
	PData data;
	//int n; // total node count
}Tree;

Tree *newTree();
void freeTree(Tree *t,int bFreeTarget);

TreeNode *newTreeNode();
void freeTreeNode(TreeNode *tn,int bFreeTarget);

TreeNode *tree_search(TreeNode *l,PData matchKey,CompareFunction fCompare);
TreeNode *tree_prepend_child(TreeNode *l,PData aItem);//new item became new head;
TreeNode *tree_append_child(TreeNode *l,PData aItem);//new item became new tail;
int tree_delete(TreeNode *x);//detache node from its neighbors,won't free the node deleted 
void tree_clear(TreeNode *tn,int bFreeTarget);

void tree_foreach(TreeNode *tn,FTreeNode f,PData ctx);

#endif
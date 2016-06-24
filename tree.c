#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

TreeNode *newTreeNode(){
	TreeNode * x=(TreeNode *)malloc(sizeof(TreeNode));

	x->parent = NULL;
	x->prev = NULL;
	x->next = NULL;
	x->first_child = NULL;
	x->last_child = NULL;

	x->data = NULL;
	x->child_count = 0;
	return x;
}

void freeTreeNode(TreeNode *l,int bFreeTarget){
	TreeNode *t, *x;
	x= l->first_child;
	while (x){
		t= x->next;
		freeTreeNode(x,bFreeTarget);
		x = t;
	}
	if (bFreeTarget)
		free(l->data);
	free(l);
}

TreeNode * tree_search(TreeNode *l,PData matchKey,CompareFunction fC){
	if (!l)
		return NULL;
	printf(", (%d ,%d),\n",(int)l->data,(int)matchKey);
	if (fC(matchKey,l->data) == 0)
		return l;
	TreeNode *x= l->first_child;
	while(x){
		TreeNode *y = tree_search(x,matchKey,fC);
		if (y)
			return y;
		else
			x=x->next;
	}
	return NULL;
}

void tree_clear(TreeNode *l,int bFreeTarget){
	TreeNode *t, *x;
	x= l->first_child;
	while (x){
		t= x->next;
		freeTreeNode(x,bFreeTarget);
		x = t;
	}
	x->first_child = NULL;
	x->last_child = NULL;
	x->child_count = 0;
}


static TreeNode * _prepend_child(TreeNode *l,TreeNode *x){
	x->parent = l;
	x->next = l->first_child;
	if (l->first_child != NULL)
		l->first_child->prev = x;

	l->first_child = x;
	x->prev =NULL;
	if(l->last_child ==NULL)
		l->last_child = x;
	l->child_count++;
}


static TreeNode * _append_child(TreeNode *l,TreeNode *x){
	x->parent = l;
	x->prev = l->last_child;
	if (l->last_child != NULL)
		l->last_child->next = x;

	l->last_child = x;
	x->next =NULL;

	if(l->first_child ==NULL)
		l->first_child = x;
	l->child_count++;
	return x;
}


int tree_delete(TreeNode *x){
	if (!x)
		return 0;
	if (x->prev != NULL)
		x->prev->next = x->next;
	else{ //no prev means that x  is the head;
		if (x->parent)
			x->parent->first_child = x->next;
	}

	if (x->next !=NULL)
		x->next->prev = x->prev;
	else//no prev means that x  is the tail;
		if (x->parent)
			x->parent->last_child = x->prev;
	if (x->parent)
		x->parent->child_count --;

	//x->parent = NULL;
	//x->prev = NULL;
	//x->next = NULL;

	return 1;
}

TreeNode * tree_prepend_child(TreeNode *l,PData aItem){
	TreeNode * x= newTreeNode();
	x->data = aItem;
	return _prepend_child(l,x);
}

TreeNode * tree_append_child(TreeNode *l,PData aItem){
	//TreeNode * x=(TreeNode *)malloc(sizeof(TreeNode));
	TreeNode * x = newTreeNode();
	x->data = aItem;
	return _append_child(l,x);
}

int tree_delete_by_data(TreeNode *l,PData matchKey,CompareFunction fCompare){
	TreeNode *x = tree_search(l,matchKey,fCompare);
	int r = tree_delete(x);
	//free(x);
	return r;
}

//parent first, child next;
void tree_foreach(TreeNode *l,PData ctx,FTreeNode f){
	TreeNode *x;
	f(l,ctx);

	x= l->first_child;
	while (x){
		tree_foreach(x,ctx,f);
		x = x->next;
	}
}

/*
Tree *_newTree(TreeNode *rn){
	Tree *x =malloc(sizeof(Tree));
	//x->n = 1;
	x->root = rn;
	x->cursor =NULL;
	x->data =NULL;
	return x;
}

Tree *newTree(PData root_data){
	TreeNode *rn = newTreeNode(root_data);
	return _newTree(rn);
}
*/

Tree *newTree(){
	Tree *x =malloc(sizeof(Tree));
	//x->n = 1;
	x->root = NULL;
	x->cursor =NULL;
	x->data =NULL;
	return x;
}

void freeTree(Tree *t,int bFreeTarget){
	if (t->root)
		freeTreeNode(t->root,bFreeTarget);
	free(t);
}

#ifndef __GRAPH_H
#define __GRAPH_H

#include <stdio.h>
#include "ds.h"

#define N 100

//typedef int[N][N] G_matrix;

struct G_vertex{
	char name[10];
	//ID always is vertex array member's index
		//PData data;
	//List adjacent;
	//int weight;//weight for adjacent edge, NOT for vertex 
	//struct G_vertex * parent;//for set 
	//int rank;                //for set 
};

struct G_edge{
	int iBegin;//often not use this member when in adjacent form.
	int iEnd;
	float weight;
};

typedef struct G_vertex G_v;
typedef struct G_edge G_e;

typedef struct{
	int n ;
	G_v *v;
	List **v_adj;//every list node data is pointer to G_edge *;
} G;

G *newG(int n);
void freeG(G *g);
void assignVertex(G *g,int i,char *name);
void addEdge(G* g,int u,int v,float weight);
void addEdge_NoDirection(G* g,int u,int v,float weight);
//enum v_color {white,gray,black}; 

void BFS(G *g,int s,int *d,int *pi);
void print_path(G *g,int s,int v,int *pi);

void DFS(G *g,int *d,int *f,int *pi,List *topsort);
void DFS_1(G *g,int *d,int *f,int *pi,List *topsort);// non recursive version. use stack

// minimum-span-tree
void mst_Kruskal(G *g,List *mst);

//Single source shortest path ,return 0 means there is negtive weight cycle. otherwise, 1;
int Bellman_Ford(G *g,int s,int *d,int *pi);
void dag_shortest_paths(G *g,int s,int *d,int *pi);
#endif
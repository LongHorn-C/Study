#ifndef __GRAPH_H
#define __GRAPH_H

#include <stdio.h>
#include "ds.h"

#define N 100

//typedef int[N][N] G_matrix;

struct G_vertex{
	char name[20];
	PData data;//for extention use. could store some useful data here.
};

struct G_edge{
	int iBegin;//often not use this member when in adjacent form.
	int iEnd;
	float weight;
	PData data; //for extention use.
};

typedef struct G_vertex G_v;
typedef struct G_edge G_e;

typedef struct{
	int n ;
	G_v *v;
	List **v_adj;//every list node data is pointer to G_edge *;
	PData data; //for extension use
} G;

G *newG(int n);
void freeG(G *g);
void assignVertex(G *g,int i,char *name);
G_e * addEdge(G* g,int u,int v,float weight);
G_e * addEdge_NoDirection(G* g,int u,int v,float weight);
//enum v_color {white,gray,black}; 

void BFS(G *g,int s,int *d,int *pi);
void print_path(G *g,int s,int v,int *pi);

void DFS(G *g,int *d,int *f,int *pi,List *topsort);
// non recursive version. use stack
void DFS_1(G *g,int *d,int *f,int *pi,List *topsort);

// minimum-span-tree
void mst_Kruskal(G *g,List *mst);
void mst_Prim(G *g,int r,int *pi);
//Single source shortest path ,return 0 means there is negtive weight cycle. otherwise, 1;
int Bellman_Ford(G *g,int s,float *d,int *pi);
void dag_shortest_paths(G *g,int s,float *d,int *pi);

//when all the edges are nonnegative
void Dijkstra(G *g,int s,float *d,int *pi/* ,List *l */);
int Johnson(G *g,float *d,int *pi);

typedef int  (*F_find_augument_path)(G *g,int s,int t,List *path); //return 0 if find none,else return 1;

//the maximum flow result is restored in the edge.data, which point to a float address.
//so after calling this function and using the edge.data, the edge.data should be freed manually.
//ie: free_graph_flow_data(g);
void Ford_Fulkerson(G *g,int s,int t,F_find_augument_path fp);
void Edmonds_Karp(G *g,int s,int t);
void free_graph_flow_data(G *);


void generic_push_relabel(G *g,int s,int t);
void relabel_to_front(G *g,int s,int t);
#endif
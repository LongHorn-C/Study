#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "G.h"
#include "heap.h"
#define INFINITE 0x7fffffff
//#define WHITE 0
//#define GRAY 1
//#define BLACK 2
enum Color {WHITE,GRAY,BLACK};

G *newG(int n){
	G *g =(G *)malloc(sizeof(G));
	g->n = n;
	g->v = (G_v *)malloc(sizeof(G_v) * n);
	g->v_adj =(List **)malloc(sizeof(List *)*n);
	for (int i=0;i<n;i++)
		g->v_adj[i] =newList();
    return g;
}

void assignVertex(G *g,int i,char *name){
	strcpy(g->v[i].name,name);
}

void addEdge(G* g,int u,int v,float w){
	G_e *e=(G_e *)malloc(sizeof(G_e));
	e->iBegin = u;
	e->iEnd = v;
	e->weight = w;
	list_insert(g->v_adj[u],(PData)e);
}

void addEdge_NoDirection(G* g,int u,int v,float w){
	addEdge(g,u,v,w);
	addEdge(g,v,u,w);
}

void freeG(G *g){
	free(g->v);
	for (int i=0;i<g->n;i++){
		freeList(g->v_adj[i],1);
	}
	free(g->v_adj);
	free(g);
}

void BFS(G *g,int s,int *d,int *pi){
	int i;
	Color *color;
	color =(Color *)malloc(sizeof(Color)*g->n);
	for (i=0;i< g->n;i++){
		color[i] =0;
		d[i] = INFINITE;
		pi[i] = -1;
	}

	color[s] = GRAY;
	d[s] =0;
	Queue *q =newQueue(100);
	enqueue(q,(PData)s);
	while (!queue_is_empty(q)){
		int u =(int)dequeue(q);
		G_e *e;
		ListNode *n;
		n=g->v_adj[u]->head;
		while (n){
			e=(G_e *)(n->data);
			int v = e->iEnd;
			if(color[v] == WHITE){
				color[v] = GRAY;
				d[v] = d[u] +1;
				pi[v] = u;
				enqueue(q,(PData)v);
			}
			n=n->next;
		}
		color[u] = BLACK;
	}
	free(color);
	freeQueue(q,0);
}

void print_path(G *g,int s,int v,int *pi){
	if (v==s)
		printf(" (%d,%s) ",s,g->v[s].name);
	else{
		if( pi[v] == -1 )
			printf("No path from %d to %d exists.\n",s,v);
		else{
			print_path(g,s,pi[v],pi);
			printf(" -> (%d,%s) ",v,g->v[v].name);
		}
	}
}

static void DFS_visit(int u,G *g,int *d,int *f,int *pi,List *topsort,int *time,Color *color){
	int v;
	color[u] = GRAY;
	(*time)++;
	d[u] = *time;
	ListNode *nv = g->v_adj[u]->head;
	while (nv){
		v = ((G_e *)(nv->data))->iEnd;
		if(color[v] ==WHITE){
			pi[v] = u;
			DFS_visit(v,g,d,f,pi,topsort,time,color);
		}
		nv = nv->next;
	}
	color[u] = BLACK;
	(*time)++;
	f[u] = *time;
	if (topsort)
		list_insert(topsort,(PData)u);
}


void DFS(G *g,int *d,int *f,int *pi,List *topsort){
	
	int i,time;
	Color *color;
	color = (Color *)malloc(sizeof(Color)*g->n);
	for (i=0;i<g->n;i++){
		color[i] =WHITE;
		pi[i] = -1;
	}
	time =0;
	for (i=0;i<g->n;i++)
		if (color[i]==WHITE)
			DFS_visit(i,g,d,f,pi,topsort,&time,color);

	free(color);
}

// use for-loop in the main loop. inner loop use listnode.next.
void DFS_(G *g,int *d,int *f,int *pi,List *topsort){
	int i,u,v,time;
	ListNode *nu,*nv;
	Color *color;
	color = (Color *)malloc(sizeof(int)*g->n);
	for (i=0;i<g->n;i++){
		color[i] =WHITE;
		pi[i] = -1;
	}
	time =0;

	Stack *stack =newStack(100);
	for (i=0;i<g->n;i++){
		if (color[i]==WHITE){
			color[i] = GRAY;
			time++;
			d[i]=time;
			u =i;
			do{
				//get first white child
				//nv= getfirst white child of u;//if u == -1 then nv =0
				if (u == -1)
					nv = 0;
				else{
					nv = g->v_adj[u]->head;
					while (nv){
						v = ((G_e *)(nv->data))->iEnd;
						if (color[v] ==WHITE)
							break;
						nv = nv->next;
					}

				}

				if (nv){
					push(stack,(PData)nv);
					v = ((G_e *)(nv->data))->iEnd;;
					color[v] = GRAY;
					pi[v] = u;
					time++;
					d[v] =time;
					u =v;
				}
					//continue;
				else{
					if (stack_is_empty(stack))
						break;
					nv = (ListNode *)pop(stack);
					v =  ((G_e *)(nv->data))->iEnd;
					color[v]= BLACK;
					time++;
					f[v] =time;
					if (topsort)
						list_insert(topsort,(PData)v);

					//nu = get first white brother of nv;
					nu = nv->next;
					while(nu) {
						if (nu){
							u = ((G_e *)(nu->data))->iEnd;
							if (color[u] == WHITE)
								break;
						}
						nu = nu->next;

					}

					if (nu){
						push(stack,(PData)nu);
						u = ((G_e *)(nu->data))->iEnd;
						color[u] = GRAY;
						pi[u] = ((G_e *)(nu->data))->iBegin;
						time++;
						d[u] =time;
					}
					else
						u = -1;
				}
			} while (!stack_is_empty(stack));
			color[i]= BLACK;
			time++;
			f[i] =time;
			if (topsort)
				list_insert(topsort,(PData)i);
		}
	}

	free(color);
	freeStack(stack,0);
}

//use link list for the main loop. simplify the iteration.
void DFS_1(G *g,int *d,int *f,int *pi,List *topsort){
	int i,u,v,time;
	G_e *e;
	ListNode *nu,*nv;
	List *toplist=newList();
	Color *color = (Color *)malloc(sizeof(int)*g->n);
	for (i = g->n -1;i>=0;i--){
		color[i] =WHITE;
		pi[i] = -1;

		e=(G_e *)malloc(sizeof(G_e));
		e->iBegin = -1;
		e->iEnd = i;
		e->weight = 1;
		list_insert(toplist,(PData)e);
	}


	time =1;
	nu = toplist->head;
	u =  ((G_e *)(nu->data))->iEnd;
	color[u] =GRAY;
	d[u] = time;
	Stack *stack =newStack(100);
	push(stack,nu);
	//printf("\tpush first u=%d\n",u);

	do{
		//nv= getfirst white direct-child of u;//if u == -1 then nv =0
		if (u == -1)
			nv = 0;
		else{
			nv = g->v_adj[u]->head;
			while (nv){
				v = ((G_e *)(nv->data))->iEnd;
				if (color[v] ==WHITE)
					break;
				nv = nv->next;
			}

		}

		if (nv){
			push(stack,(PData)nv);
			v = ((G_e *)(nv->data))->iEnd;;
			//printf("\npush child v=%d\n",v);

			color[v] = GRAY;
			pi[v] = u;
			time++;
			d[v] =time;

			u =v;
			nu =nv;
		}
		else{
			nu = (ListNode *)pop(stack);
			u =  ((G_e *)(nu->data))->iEnd;
			//printf("\tpop u=%d\n",u);
			color[u]= BLACK;
			time++;
			f[u] =time;
			if (topsort)
				list_insert(topsort,(PData)u);

			//nv = get first white brother of nu;
			nv = nu->next;
			while(nv) {
				v = ((G_e *)(nv->data))->iEnd;
				if (color[v] == WHITE)
					break;
				nv = nv->next;
			}

			if (nv){
				push(stack,(PData)nv);
				v = ((G_e *)(nv->data))->iEnd;
				color[v] = GRAY;
				pi[v] = pi[u];
				time++;
				d[v] =time;

				u = v;
				nu = nv;
			}
			else
				u = -1;
		}
	} while (!stack_is_empty(stack));

	freeList(toplist,1);
	free(color);
	freeStack(stack,0);
}

static int wCompare(PData a,PData b){
	G_e *e;
	e = (G_e *)a;
	float k1 =e->weight;//e->v1 *100 + e->v2*10;
	e = (G_e *)b;
	float r =k1 - e->weight;// *100 + e->v2*10;


	if ( r > -1.0e-5 && r < 1.0e-5 )
		return 0;
	else
		return r; // convert max_heap to min_heap.
}


void mst_Kruskal(G *g,List *mst){
	int i,j,u,v;
	Dset *sv =(Dset *)malloc(sizeof(Dset)*g->n);
	for (i=0;i<g->n;i++)
		set_make(&(sv[i]));

	//omitted: clear all node in mst

	//Heap *weightSort = newHeap(g->n,wCompare);
	PData a[100];// =(PData *)malloc(sizeof(PData)*100);should use a dynamic min-heap.

	ListNode *n;
	j=0;
	for (i=0;i<g->n;i++){
		n = g->v_adj[i]->head;
		while(n){
			a[j++] = (PData)(n->data);
			n = n->next;
		}
	}

	heap_sort(a,j,wCompare);

	for(i=0;i<j;i++){
		G_e *e= (G_e *)(a[i]);
		u = e->iBegin;
		v = e->iEnd;
		if (set_find(&(sv[u])) != set_find(&(sv[v])) ){
			list_insert(mst,e);
			set_union(&(sv[u]),&(sv[v]));
		}
	}
	//free(a);
	free(sv);
}
/*
struct K{
	int pos;
	float value;
	int inQ;
}

static int keyCompare(PData a,PData b){
	
	float r = (K *)a->value - (K *)b->value; 
	if ( r > -1.0e-5 && r < 1.0e-5 )
		return 0;
	else
		return r; // convert max_heap to min_heap.
}

//no way to efficiently decreasekey of the min-queue. index can't be determned easyly. abort.

void mst_Prim(G *g,int r,int *pi){
	int i;
	K *key = (K *)malloc(sizeof(K)*g->n);
	Heap *h= newHeap(100,keyCompare);
	
	for (i =0 ;i< g->n; i++){
		key[i].pos=i;
		key[i].value = INFINITE;
		key[i].inQ =1;
		
		pi[i] = -1;
	}
	key[r] = 0;
	build_(h,r);
	
	
	
	
	free(key);
}
*/


static void initialize_single_source(G *g,int s,int *d,int *pi){
	for (int i =0 ;i< g->n ;i++){
		d[i] = INFINITE;
		pi[i] = -1;
	}
	d[s] = 0;
}

static void relax(G *g,int u,int v,float w,int *d,int *pi){
	float f =  d[u] + w;
	if (d[v] > f){
		d[v] = f;
		pi[v] = u;
	}
}

//Single source shortest path ,return 0 means there is negtive weight cycle. otherwise, 1;
int Bellman_Ford(G *g,int s,int *d,int *pi){
	initialize_single_source(g,s,d,pi);
	int i,ii,u,v;
	float w;
	ListNode *n;
	for (i=0;i< g->n - 1;i++){
		for (u=0;u< g->n ;u++){
			n= g->v_adj[u]->head;
			while (n){
				G_e *e = (G_e *)(n->data);
				v = e->iEnd;
				w = e->weight;
				relax(g,u,v,w,d,pi);
				n = n->next;
			}
			
		}
	}
	
	for (u=0;u< g->n ;u++){
		n= g->v_adj[u]->head;
		while (n){
			G_e *e = (G_e *)(n->data);
			v = e->iEnd;
			w = e->weight;
			if (d[v] > d[u] + w )
				return 0;
			
			n = n->next;
		}
		
	}
	return 1;
}

void dag_shortest_paths(G *g,int s,int *d,int *pi){
	int u,v,w;
	int *f = (int *)malloc(sizeof(int)*g->n);
	List *topsort = newList();
	ListNode *ns,*ne;
	DFS_1(g,d,f,pi,topsort);
	initialize_single_source(g,s,d,pi);
	ns = topsort->head;
	while(ns){
		u = (int)(ns->data);
		ne = g->v_adj[u]->head;
		while(ne){
			G_e *e = (G_e *)(ne->data);
			v = e->iEnd;
			w = e->weight;
			relax(g,u,v,w,d,pi);
			
			ne = ne->next;
		}
		
		ns = ns->next;
	}
	
	
	free(f);	
	freeList(topsort,0);
}



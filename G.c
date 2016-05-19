#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds.h"
#include "G.h"
#define INFINITE 0x7fffffff
#define WHITE 0
#define GRAY 1
#define BLACK 2

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

void addEdge(G* g,int u,int v,int w){
	G_e *e=(G_e *)malloc(sizeof(G_e));
	e->iBegin = u;
	e->iEnd = v;
	e->weight = w;
	list_insert(g->v_adj[u],(PData)e);
}

void addEdge_NoDirection(G* g,int u,int v,int w){
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
	int *color;
	color =(int *)malloc(sizeof(int)*g->n);
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

static void DFS_visit(int u,G *g,int *d,int *f,int *pi,List *topsort,int *time,int *color){
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
	int i,*color,time;
	color = (int *)malloc(sizeof(int)*g->n);
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


void DFS_(G *g,int *d,int *f,int *pi,List *topsort){
	int i,u,v,*color,time;
	ListNode *nu,*nv;
	color = (int *)malloc(sizeof(int)*g->n);
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


void DFS_1(G *g,int *d,int *f,int *pi,List *topsort){
	int i,u,v,*color,time;
	G_e *e;
	ListNode *nu,*nv;
	List *toplist=newList();
	color = (int *)malloc(sizeof(int)*g->n);
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


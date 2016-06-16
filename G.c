#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>

#include "G.h"
#include "heap.h"
#include "sort.h"
#include "common.h" //for debug

//#define STRANGE_BIG 0x7fffffbf
//INT_MAX INT_MIN #define INFINITE 0x7fffffff
//FLT_MAX,FLT_MIN #define F_INFINITE 1.0e100

//#define WHITE 0
//#define GRAY 1
//#define BLACK 2

#define _edge_in_node(n) ((G_e *) (n)->data)

G *newG(int n){
	G *g =(G *)malloc(sizeof(G));
	g->n = n;
	g->v = (G_v *)malloc(sizeof(G_v) * n);
	g->v_adj =(List **)malloc(sizeof(List *)*n);
	for (int i=0;i<n;i++){
		g->v_adj[i] =newList();
		g->v[i].data = 0;
	}
    return g;
}

void assignVertex(G *g,int i,char *name){
	strcpy(g->v[i].name,name);
}

G_e * addEdge(G* g,int u,int v,float w){
	G_e *e=(G_e *)malloc(sizeof(G_e));
	e->iBegin = u;
	e->iEnd = v;
	e->weight = w;
	e->data =0;
	list_prepend(g->v_adj[u],(PData)e);
	return e;
}

G_e * addEdge_NoDirection(G* g,int u,int v,float w){
	addEdge(g,u,v,w);
	return addEdge(g,v,u,w);
}

void freeG(G *g){
	int i;
	/*for (i=0;i<g->n;i++)
		free(g->v[i].data);
	*/
	free(g->v);
	for (i=0;i<g->n;i++){

		/*ListNode node = g->v_adj[i].head;
		while (node){
			G_e *edge =(G_e*)node->data;
			free(edge->data);
			//free(edge);
			node =node->next;
		}*/

		freeList(g->v_adj[i],1);
	}
	free(g->v_adj);
	free(g);
}

void g_foreach_vertex(G *g,FVertex f,PData ctx){
	for (int i=0;i<g->n;i++)
		f(&(g->v[i]),ctx);	
	
}
void g_foreach_edge(G *g,FEdge f,PData ctx){
	for (int i=0;i<g->n;i++){
	ListNode *n = g->v_adj[i]->head;
		while(n){
			f(_edge_in_node(n),ctx);
			n = n->next;
		}
	}
	
}

typedef enum {WHITE,GRAY,BLACK} Color;

void BFS(G *g,int s,int *d,int *pi){
	int i;
	Color *color =(Color *)malloc(sizeof(Color)*g->n);
	for (i=0;i< g->n;i++){
		color[i] =0;
		d[i] = INT_MAX;
		pi[i] = -1;
	}

	color[s] = GRAY;
	d[s] =0;
	Queue *q =newQueue();
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

static void _DFS_visit(int u,G *g,int *d,int *f,int *pi,List *topsort,int *time,Color *color){
	int v;
	color[u] = GRAY;
	(*time)++;
	d[u] = *time;
	ListNode *nv = g->v_adj[u]->head;
	while (nv){
		v = ((G_e *)(nv->data))->iEnd;
		if(color[v] ==WHITE){
			pi[v] = u;
			_DFS_visit(v,g,d,f,pi,topsort,time,color);
		}
		nv = nv->next;
	}
	color[u] = BLACK;
	(*time)++;
	f[u] = *time;
	if (topsort)
		list_prepend(topsort,(PData)u);
}


void DFS(G *g,int *d,int *f,int *pi,List *topsort){

	int i,time;
	Color *color = (Color *)malloc(sizeof(Color)*g->n);
	for (i=0;i<g->n;i++){
		color[i] =WHITE;
		pi[i] = -1;
	}
	time =0;
	for (i=0;i<g->n;i++)
		if (color[i]==WHITE)
			_DFS_visit(i,g,d,f,pi,topsort,&time,color);

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

	Stack *stack =newStack();
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
						list_prepend(topsort,(PData)v);

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
				list_prepend(topsort,(PData)i);
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
		list_prepend(toplist,(PData)e);
	}


	time =1;
	nu = toplist->head;
	u =  ((G_e *)(nu->data))->iEnd;
	color[u] =GRAY;
	d[u] = time;
	Stack *stack =newStack();
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
				list_prepend(topsort,(PData)u);

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

static int _wCompare(PData a,PData b){
	G_e *e;
	e = (G_e *)a;
	float k1 =e->weight;//e->v1 *100 + e->v2*10;
	e = (G_e *)b;
	float r =k1 - e->weight;// *100 + e->v2*10;


	if ( r > -1.0e-5 && r < 1.0e-5 )
		return 0;
	else
		return r>0? 1:-1; // convert max_heap to min_heap.
}


void mst_Kruskal(G *g,List *mst){
	int i,j,u,v;
	Dset *sv =(Dset *)malloc(sizeof(Dset)*g->n);
	for (i=0;i<g->n;i++)
		set_make(&(sv[i]));

	//omitted: clear all node in mst
	list_clear(mst,0);

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

	heap_sort(a,j,_wCompare);

	for(i=0;i<j;i++){
		G_e *e= (G_e *)(a[i]);
		u = e->iBegin;
		v = e->iEnd;
		if (set_find(&(sv[u])) != set_find(&(sv[v])) ){
			list_append(mst,e);
			set_union(&(sv[u]),&(sv[v]));
		}
	}
	//free(a);
	free(sv);
}

// for the use of vertex in min-piority-queue base on d
typedef struct _V_for_q{
	int i;  // vertex index in G. for debugging.
	float d;
	int i_in_q; //-1 means being not in queue.
} V_for_q;

static int _keyCompare(PData a,PData b){
	float f1= ((V_for_q *)a)->d;
	float f2= ((V_for_q *)b)->d;

	float r = f1 - f2;

	if ( r > -1.0e-5 && r < 1.0e-5 )
		return 0;
	else
		return f1 > f2 ? -1 : 1; // convert max_heap to min_heap.
}

static int _on_track_vq(enum Op_type ot,int i,PData di,int j,PData dj){
	V_for_q *ta=(V_for_q *)di;
	switch (ot){
		case otSwap:
			ta->i_in_q = i;
			V_for_q *tj=(V_for_q *)dj;
			tj->i_in_q = j;
			break;
		case otSet:
			ta->i_in_q = i;
			break;
		case otRemove:
			ta->i_in_q = -1;
			break;
		case otInsert:
			ta->i_in_q = i;
			break;
	}
	return 0;
}


void mst_Prim(G *g,int r,int *pi){
	int i;
	V_for_q *vv = (V_for_q *)malloc(sizeof(V_for_q)*g->n);
	PData *dd =	 (PData *)malloc(sizeof(PData)*g->n);
	for (i =0 ;i< g->n; i++){
		vv[i].i=i;
		vv[i].d = INT_MAX;
		//vv[i].d = STRANGE_BIG;

		vv[i].i_in_q =i;

		dd[i] = &(vv[i]);
		pi[i] = -1;
	}
	vv[r].d = 0.0;

	Heap min_queue;
	min_queue.a=NULL;// just in case. avoid freeing an uninitilized pointer.
	heap_attach(&min_queue,dd,g->n, _keyCompare, _on_track_vq);
	//printf("attach ok\n");
	V_for_q *uq;

	while( uq = (V_for_q *)heap_extract_max(&min_queue) ){
	//printheap;
		int u = uq->i;
		//printf("U: %d-%s\n",u,g->v[u].name);
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			//int iq1 = vv[v].i_in_q;
			//int qi = ((V_for_q *)(min_queue.a[iq1]))->i;
			//printf("\t V:%d-%s  iq=%d, qi=%d  \n",v,g->v[v].name,edge->weight,iq1,qi);
			if( vv[v].i_in_q != -1 && edge->weight < vv[v].d){
				pi[v] = u;
				vv[v].d = edge->weight;
				int iq = vv[v].i_in_q;
				//int qi = ((V_for_q *)(min_queue.a[iq]))->i;

				max_heap_update_key(&min_queue,iq);

			}


			ne = ne->next;
		}

	}
	//printf("q empty ok\n");
	free(vv);
}



static void _initialize_single_source(G *g,int s,float *d,int *pi){
	for (int i =0 ;i< g->n ;i++){
		d[i] = FLT_MAX;
		pi[i] = -1;
	}
	d[s] = 0;
}

//if no update d, then return 0, else return 1
static int _relax(int u,int v,float w,float *d,int *pi){
	int r = 0;
	float f =  d[u] + w;
	if (d[v] > f){
		d[v] = f;
		pi[v] = u;

		r=1;
	}
	return r;
}

//Single source shortest path ,return 0 means there is negtive weight cycle. otherwise, 1;
int Bellman_Ford(G *g,int s,float *d,int *pi){
	_initialize_single_source(g,s,d,pi);
	int i,u,v;
	float w;
	ListNode *n;
	for (i=0;i< g->n - 1;i++){
		for (u=0;u< g->n ;u++){
			n= g->v_adj[u]->head;
			while (n){
				G_e *e = (G_e *)(n->data);
				v = e->iEnd;
				w = e->weight;
				_relax(u,v,w,d,pi);
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
				return 0; //negative cycle found

			n = n->next;
		}

	}
	return 1;
}

void dag_shortest_paths(G *g,int s,float *d,int *pi){
	int u,v,w;
	int *pd = (int *)malloc(sizeof(int)*g->n);
	int *pf = (int *)malloc(sizeof(int)*g->n);

	List *topsort = newList();
	DFS_1(g,pd,pf,pi,topsort);
	free(pd);
	free(pf);

	ListNode *ns,*ne;
	_initialize_single_source(g,s,d,pi);
	ns = topsort->head;
	while(ns){
		u = (int)(ns->data);
		ne = g->v_adj[u]->head;
		while(ne){
			G_e *e = (G_e *)(ne->data);
			v = e->iEnd;
			w = e->weight;
			_relax(u,v,w,d,pi);

			ne = ne->next;
		}

		ns = ns->next;
	}

	freeList(topsort,0);
}

void Dijkstra(G *g,int s,float *d,int *pi/*,List *l*/){
	_initialize_single_source(g,s,d,pi);
	//omitted: clear the list;
	int i;
	V_for_q *vv = (V_for_q *)malloc(sizeof(V_for_q)*g->n);
	PData *dd =	 (PData *)malloc(sizeof(PData)*g->n);

	for (i =0 ;i< g->n; i++){
		vv[i].i=i;
		vv[i].d = d[i];
		vv[i].i_in_q =i;

		dd[i] = &(vv[i]);
		pi[i] = -1;

	}

	Heap min_queue;
	min_queue.a=NULL;// just in case. avoid freeing an uninitilized pointer.
	heap_attach(&min_queue,dd,g->n, _keyCompare, _on_track_vq);
	V_for_q *uq;
	while( uq = (V_for_q *)heap_extract_max(&min_queue) ){
		int u = uq->i;
		//list_append(l,(PData)u);

		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			int iq = vv[v].i_in_q;

			float w = edge->weight;

			if ( iq != -1 && _relax(u,v,w,d,pi) ){
				vv[v].d = d[v];
				max_heap_update_key(&min_queue,iq);
			}
			ne = ne->next;
		}
	}

	free(vv);
}

static void _copy_graph(G *s,G *d){
	for (int i =0 ;i<s->n;i++)
		assignVertex(d,i,s->v[i].name);
	//assignVertex(g_, g->n ,"s_");

	for (int u=0;u<s->n;u++){
			ListNode *ne = s->v_adj[u]->head;
			while (ne){
				G_e *edge =(G_e *)(ne->data);
				int v = edge->iEnd;
				float w = edge->weight;
				addEdge(d,u,v,w);

				ne = ne->next;
			}
	}
}


//All pair shortest path
//d and pi are n*n array.
int Johnson(G *g,float *d,int *pi){
	int result;
	//new g_ with additional vertex s_
	G *g_ =newG(g->n + 1);
	_copy_graph(g,g_);
	assignVertex(g_, g_->n - 1,"s_");
	for (int i=0;i<g->n;i++)
		addEdge(g_,g->n,i,0);
/*
	for (int i=0;i<g_->n;i++)
		printf("  i:%d,name:%s",i,g_->v[i].name);
	printf("  end of v \n");

*/

	//
	float *d_ =(float *)malloc(sizeof(float) * g_->n);
	int *pi_ = (int *)malloc(sizeof(int) * g_->n);
	if ( Bellman_Ford(g_,g_->n-1,d_,pi_) == 0){
		printf("the input graph contains a negative-weight cycle\n");
		result = 0;
	}
	else{
/*		printf("d_:");
		print_ff(d_,6);
		printf("pi_:");
		print_ii(pi_,6);
	*/
		G *g_c =newG(g->n);
		_copy_graph(g,g_c);
		for (int u=0;u<g->n;u++){
				ListNode *ne = g_c->v_adj[u]->head;
				while (ne){
					G_e *edge =(G_e *)(ne->data);
					int v = edge->iEnd;
					edge->weight = edge->weight + d_[u] - d_[v];

					ne = ne->next;
				}

		}
		float *d_c =(float *)malloc(sizeof(float) * g_c->n);
		int *pi_c = (int *)malloc(sizeof(int) * g_c->n);
		for (int u=0;u<g_c->n;u++){
			Dijkstra(g_c,u,d_c,pi_c);
		//	printf(" dc: ");
		//	print_ff(d_c,5);
			for (int v=0;v<g_c->n;v++){
				d[u * g_c->n + v] = d_c[v] + d_[v] - d_[u];
				pi[u * g_c->n + v] = pi_c[v];
			}
		}
		free(g_c);
		free(d_c);
		free(pi_c);
		result = 1;
	}

	free(g_);
	free(d_);
	free(pi_);

	return result;
}


//Maximum Flow
/*
Ford_Fulkerson_Method(G *g,s,t)
initilize flow f to 0
while there exists an augumenting path p
	do augument flow f along p
return f;

*/

typedef struct{
	float f;
	float c;
	G_e * reverse;//if current edge is edge(u,v),then /reverse/ points to edge(v,u)
} _E_attr;

//parameter is pointer
#define _attr_f(e) ((_E_attr *)((e)->data))->f
#define _attr_c(e) ((_E_attr *)((e)->data))->c
#define _attr_r(e) (((_E_attr *)((e)->data))->reverse)

static G_e * _locate_edge(G *g,int u,int v){
	ListNode *ne = g->v_adj[u]->head;
	while (ne){
			G_e *edge =(G_e *)(ne->data);
			if (edge->iEnd == v)
				return edge;
			ne = ne->next;
		}
	return 0;
}

static void _initialize_flow_capacity(G *g){
	_E_attr *fc;
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			fc = (_E_attr *)edge->data;
			if (fc ==0){
				fc = (_E_attr *)malloc(sizeof(_E_attr));
				fc->f = 0;
				fc->c = edge->weight;

				edge->data = fc;
			}

			int v = edge->iEnd;
			G_e *edgeR =_locate_edge(g,v,u);
			if (edgeR == 0)
				edgeR =addEdge(g,v,u,0);
			fc->reverse = edgeR;

			if (edgeR->data ==0){
				edgeR->data = malloc(sizeof(_E_attr));
				_E_attr *fcR = (_E_attr *)(edgeR->data);
				fcR->f = 0;
				fcR->c = edgeR->weight;
				fcR->reverse = edge;
			}
			ne = ne->next;
		}

	}
}

static void _finalize_flow_capacity(G *g){
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			free(edge->data);

			ne = ne->next;
		}

	}
}

//for debug
static void  _chk_e(G *g){

	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			_E_attr *f = (_E_attr *) edge->data;

			printf("(%s,%s)\tweight:%f,\tflow:%f,\tcapacity:%f\n",g->v[edge->iBegin].name,g->v[edge->iEnd].name,edge->weight,f->f,f->c);

			ne = ne->next;
		}

	}
}

//the result maximum flow is restored in the edge.data which point to a float address.
void Ford_Fulkerson(G *g,int s,int t,F_find_augument_path fap){
	G *g_c =newG(g->n);
	_copy_graph(g,g_c);
	_initialize_flow_capacity(g_c);

	//_chk_g(g_c);

	List *path =newList();
	while ( fap(g_c,s,t,path) ){
        //print path;
		//printf("\nfind 1  ");
		float cf = FLT_MAX;
		ListNode *node =path->head;
		while(node){
			G_e *edge = (G_e *) node->data;
			_E_attr *f = (_E_attr *) edge->data;
			if ( f->c < cf )
				cf = f->c;
            //printf("(%d->%d)",edge->iBegin,edge->iEnd);
			node = node->next;
		}
		//printf("  end path.");
		//now cf is the minimum augumentable flow

		node =path->head;
		while(node){
			G_e *edge = (G_e *) node->data;
			_E_attr *fc = (_E_attr *) edge->data;
			fc->f = fc->f + cf;
			fc->c = fc->c - fc->f;

			G_e *edgeR = fc->reverse;
			_E_attr *fcR = (_E_attr *) edgeR->data;
			fcR->f = - fc->f;
			fcR->c =fcR->c + fc->f;

			node = node->next;
		}
	}
	freeList(path,0);

	//copy maximum flow from g_c to original graph g;
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			float *mf =(float *)malloc(sizeof(float));
			G_e *es =_locate_edge(g_c,u,v);
			_E_attr *fcs = es->data;
			*mf = fcs->f;
			edge->data = mf;

			ne = ne->next;
		}

	}

	_finalize_flow_capacity(g_c);
	freeG(g_c);
}


static int _BFS_for_Edmond_Karp(G *g,int s,int t,List *path){
	list_clear(path,0);

	int *d =(int *)malloc(sizeof(int)*g->n);
	int *pi =(int *)malloc(sizeof(int)*g->n);

//{begin BSF
	int i;
	Color *color =(Color *)malloc(sizeof(Color)*g->n);
	for (i=0;i< g->n;i++){
		color[i] =0;
		d[i] = INT_MAX;
		pi[i] = -1;
	}

	color[s] = GRAY;
	d[s] =0;
	Queue *q =newQueue();
	enqueue(q,(PData)s);
	while (!queue_is_empty(q)){
		int u =(int)dequeue(q);
		G_e *e;
		ListNode *n;
		n=g->v_adj[u]->head;
		while (n){
			e=(G_e *)(n->data);
			_E_attr *fc = (_E_attr *) e->data;
			int v = e->iEnd;
			if( fc->c > 0 ){   //this part is different from the original BFS
				if(color[v] == WHITE){
					color[v] = GRAY;
					d[v] = d[u] +1;
					pi[v] = u;
					enqueue(q,(PData)v);
				}
			}
			n=n->next;
		}
		color[u] = BLACK;
	}
	free(color);
	freeQueue(q,0);
//}end of BFS


	int child =t;
	int parent =pi[child];
	if ( parent == -1 )
		return 0;
	while (parent != -1){
		PData data=(PData)_locate_edge(g,parent,child);
		list_prepend(path,data);

		child =parent;
		parent = pi[parent];
	}
	free(d);
	free(pi);
	return 1;
}

void Edmonds_Karp(G *g,int s,int t){
	Ford_Fulkerson(g,s,t,_BFS_for_Edmond_Karp);
}

void free_graph_flow_data(G *g){
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			free(edge->data);

			ne = ne->next;
		}

	}
}

//following code is for push-relabel method.
typedef struct{
	int e;
	int h;
	List *neighbor;//admissable adjacent edges,each node pointer to an adjacent edge
	ListNode * nc;//current position on neighbor list
} _V_attr;

//parameter is NOT a pointer
#define _attr_e(v) ((_V_attr *)((v).data))->e
#define _attr_h(v) ((_V_attr *)((v).data))->h
#define _attr_n(v) (((_V_attr *)((v).data))->neighbor)
#define _attr_nc(v) (((_V_attr *)((v).data))->nc)

//for debug
static void  _chk_v(G *g){
    G_e *e;
	for (int u=0;u<g->n;u++){
		printf("%s: \te: %d,\th: %d, \tn:[",g->v[u].name,_attr_e(g->v[u]),_attr_h(g->v[u]));
		ListNode *node = _attr_n(g->v[u])->head;
		while (node){
			e =_edge_in_node(node);
			printf("%d,",e->iEnd);
			node =node->next;
		}
		int ncv = -1;
        node = _attr_nc(g->v[u]);
		if (node){
            e =  _edge_in_node(node);
            if( e )
                ncv = e->iEnd;
		}

		printf("] \tnc: %d",ncv);
		printf("\n");
	}
}
//for debug
static void _chk_g(G *g){
	_chk_v(g);
	_chk_e(g);
}


static void _initialize_preflow(G *g,int s, int t){
	_initialize_flow_capacity(g);
	for (int i=0;i<g->n;i++){
		_V_attr *va =  (_V_attr *)malloc(sizeof(_V_attr));
		va->h =0;
		va->e =0;
		va->neighbor =newList();
		va->nc = 0;
		g->v[i].data = va;
	}
	//g->data = newQueue();


	_attr_h(g->v[s]) = g->n;

	ListNode *n;
	n=g->v_adj[s]->head;
	while (n){
		G_e *e=(G_e *)(n->data);
		int u = e->iEnd;
		float c =  _attr_c(e);
		_attr_f(e) = c;
		_attr_c(e) = 0;
		_attr_f(_attr_r(e)) = -c;
		_attr_c(_attr_r(e)) += c;

		_attr_e(g->v[u]) = c;

		_attr_e(g->v[s]) -= c;
		n=n->next;
	}
}


static void _finalize_preflow(G *g){
	//freeQueue((Queue *) g->data,0);
	for (int i=0;i<g->n;i++){
		freeList(_attr_n(g->v[i]),0);
		free(g->v[i].data);
	}
	_finalize_flow_capacity(g);
}

//return the latest e(u)
static int _push(G *g,G_e * edge){
	int u = edge->iBegin;
	int v = edge->iEnd;
	_V_attr *va_u = (_V_attr *) g->v[u].data;
	_E_attr *ea_e = (_E_attr *) edge->data;
	float e = va_u->e;
	float cf = ea_e->c;
	float df = e>cf ? cf: e;
	ea_e->f += df;
	ea_e->c -= df;

	G_e *er = _attr_r(edge);
	_attr_f(er) = - ea_e->f;
	_attr_c(er) += df;

	_attr_e( g->v[u] ) -= df;
	_attr_e( g->v[v] ) += df;
    int ee = _attr_e( g->v[u] );
	return _attr_e( g->v[u] );
}

static void _relabel(G *g,int u){
	int min_hv = 2*g->n+1;
	int v,hv;
	float cv;
//get min_hv
	ListNode *n;
	n=g->v_adj[u]->head;
	G_e *e;
	while (n){
		e = _edge_in_node( n );
        v =  e->iEnd;
        cv =_attr_c(e);
		if ( cv>0 ){
            hv = _attr_h(g->v[v]);
            min_hv = hv < min_hv? hv:min_hv;
		}
		n=n->next;
	}
//end getting min_hv
//update height of u
	_V_attr *va_u  =(_V_attr *) g->v[u].data;
	va_u->h = min_hv + 1;
//build neighbor list of u
	list_clear(va_u->neighbor,0);
	n = g->v_adj[u]->head;
	while (n){
		e = _edge_in_node( n );
		v =  e->iEnd;
		hv = _attr_h(g->v[v]);
		cv = _attr_c(e);
		if (hv == min_hv && cv > 0)
			list_append(va_u->neighbor,e);
		n=n->next;
	}
}

//for debug
void _print_list(List *l){
	printf("[ ");
	ListNode *n = l->head;
	while (n){
		printf("%d,",(int) n->data);
		n = n->next;
	}
	printf(" ]\n");
}


//this function will alter the inner structure of g.
//so it should be run on a copy of original G.
static void _generic_push_relabel(G *g,int s,int t){
	
	//_chk_g(g);

	Stack *sk= newStack();
	for (int i=0;i<g->n;i++){
		if (_attr_e(g->v[i]) > 0 )
			push(sk,(PData)i);
	}

	while( !stack_is_empty(sk) ){
		int u =(int) pop(sk);
		
		//printf("\nafter pop %d and before process\n",u);
		//_print_list(sk);
		//_chk_g(g);
		
		G_e* ae = 0;
		ListNode *h = _attr_n(g->v[u])->head;
		if (h){
			ae = _edge_in_node(h);

			int v = ae->iEnd;
			int cv = _attr_c(ae);
			int hv = _attr_h(g->v[v]);
			int hu = _attr_h(g->v[u]);

			if( cv>0 && hu == hv+1 ){
				int ev =_attr_e(g->v[v]);
				int eu = _push(g,ae);
				//printf("after push u:%d v:%d,eu:%d",u,v,eu);
				if (ev == 0 && v !=s && v != t)
					push(sk,(PData)v);
				
				if (eu >0)
					push(sk,(PData)u);
			}
			else{
				_relabel(g,u);
				push(sk,(PData)u);
			}
				
		}
		else{
			_relabel(g,u);
			push(sk,(PData)u);
		}
		//printf("\nafter pop %d and after process\n",u);
		//_print_list(sk);
		//_chk_g(g);
	}
	freeStack(sk,0);
}

void generic_push_relabel(G *g,int s,int t){
	G *g_c =newG(g->n);
	_copy_graph(g,g_c);

	_initialize_preflow(g_c,s,t);
	_generic_push_relabel(g_c,s,t);

	//copy maximum flow from g_c to original graph g;
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			float *mf =(float *)malloc(sizeof(float));
			G_e *es =_locate_edge(g_c,u,v);
			_E_attr *fcs = es->data;
			*mf = fcs->f;
			edge->data = mf;

			ne = ne->next;
		}

	}
	_finalize_flow_capacity(g_c);
	freeG(g_c);
}


static void _discharge(G *g,int u){
 	while(_attr_e(g->v[u]) >0){
        int eu = _attr_e(g->v[u]);
		ListNode *cur_node =  _attr_nc(g->v[u]);
		if (cur_node == NULL){
			_relabel(g,u);
			_attr_nc(g->v[u]) = _attr_n(g->v[u])->head;
			continue;
		}
		else {
			G_e * edge_uv = _edge_in_node(cur_node);
			int v = edge_uv->iEnd;
			int cv = _attr_c(edge_uv);
			int hv = _attr_h(g->v[v]);
			int hu = _attr_h(g->v[u]);

			if( cv>0 && hu == hv+1 ){
				_push(g,edge_uv);
				continue;
			}
		}
		_attr_nc(g->v[u]) = _attr_nc(g->v[u])->next;

	}
}

void _relabel_to_front(G *g,int s,int t){
	//_chk_g(g);
	List *l =newList();
	for (int i=0;i<g->n;i++){
		if (i != s && i != t){
			list_append(l,(PData)i);
			_attr_nc(g->v[i]) = _attr_n(g->v[i])->head;
		}

	}
	ListNode *ln = l->head;
	while(ln){
		int u = (int) ln->data;
		int h_old = _attr_h(g->v[u]);
		//printf("before discharge\n");
		//_chk_g(g);
		_discharge(g,u);
		//printf("after discharge\n");
		//_chk_g(g);
		if (_attr_h(g->v[u]) > h_old){
		//move ln to the front of the list
			list_delete(l,ln);
			free(ln);
			list_prepend(l,(PData)u);
			ln = l->head;
		}
		ln = ln->next;
	}

	freeList(l,0);
}

void relabel_to_front(G *g,int s,int t){
	G *g_c =newG(g->n);
	_copy_graph(g,g_c);
	_initialize_preflow(g_c,s,t);
	
	_relabel_to_front(g_c,s,t);
	
	//copy maximum flow from g_c to original graph g;
	for (int u=0;u<g->n;u++){
		ListNode *ne = g->v_adj[u]->head;
		while (ne){
			G_e *edge =(G_e *)(ne->data);
			int v = edge->iEnd;
			float *mf =(float *)malloc(sizeof(float));
			G_e *es =_locate_edge(g_c,u,v);
			_E_attr *fcs = es->data;
			*mf = fcs->f;
			edge->data = mf;

			ne = ne->next;
		}

	}
	_finalize_preflow(g_c);
	freeG(g_c);
}

#undef _edge_in_node
#undef _attr_e
#undef _attr_h
#undef _attr_n

#undef _attr_f
#undef _attr_c
#undef _attr_r

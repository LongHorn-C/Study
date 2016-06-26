#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heap.h"

static int _parent(int i){
	return i/2;
}

static int _left_child(int i){
	return i*2+1;
}

static int _right_child(int i){
	return i*2+2;
}


static PData * _expand_array(PData * a,int len){
	PData *new_a = malloc(sizeof(PData)*len*2);
	memcpy(new_a,a,sizeof(PData)*len);
	free(a);
	return new_a;
}

static PData * _contract_array(PData * a,int len){
	len /=2;
	PData *new_a = malloc(sizeof(PData)*len);
	memcpy(new_a,a,sizeof(PData)*len);
	free(a);
	return new_a;
}


static void _swap(Heap *hp,int i,int j){
	PData t = hp->a[i];
	hp->a[i] = hp->a[j];
	hp->a[j] =t;
	
	if(hp->on_track)
		hp->on_track(otSwap,i,hp->a[i],j,hp->a[j]);
	
		//record_swap(hp->track,i,j);
/*
	hp->c[hp->o[i]] = j;
	hp->c[hp->o[j]] = i;

	int oi =hp->o[i];
	hp->o[i] = hp->o[j];
	hp->o[j] = oi;*/
}


//void build_heap(PData *a,int n);

static void _max_heapify(Heap *hp,int i){
	int l, r, largest;
	l=_left_child(i);
	r=_right_child(i);
	if (l <= hp->size-1 && hp->fCompare(hp->a[l],hp->a[i]) > 0 )
		largest = l;
	else
		largest = i;
	if  (r <= hp->size-1 && hp->fCompare( hp->a[r],hp->a[largest]) > 0 )
		largest = r;
	if (largest != i){
/*		PData temp = hp->a[i];
		hp->a[i] = hp->a[largest];
		hp->a[largest] =temp;
*/
		_swap(hp,i,largest);

		_max_heapify(hp,largest);
	}
}


static void _build_max_heap(Heap *h){
	for (int i= h->size /2 -1;i>=0;i--)
		_max_heapify(h,i);
}

Heap * heap_attach(Heap* h,PData *a,int n,CompareFunction fCompare,Fun_on_track_queue on_track){
	if (h->a)
		free(h->a);
	h->a = a;
	h->len = n;
	h->size = n;
	h->fCompare = fCompare;
	h->on_track = on_track;
	_build_max_heap(h);
	return h;
}

void heap_sort(PData *a,int n,CompareFunction fCompare){
	int i;
	Heap h;
	h.a=NULL;// just in case. avoid freeing an uninitilized pointer.
	heap_attach(&h,a,n,fCompare,0);
	for (i = n-1 ;i>=1;i--){
		_swap(&h,i,0);
		h.size--;
		_max_heapify(&h,0);
	}
 
}

//following is a priority queue.
PData heap_maximum(Heap *hp){
	return hp->a[0];
}

PData heap_extract_max(Heap *hp){
	if (hp->size <1)
		return NULL;
	PData max	= hp->a[0];
	hp->a[0] = hp->a[hp->size-1];
	if (hp->on_track){
		hp->on_track(otRemove,0,max,0,0);
		hp->on_track(otSet,0,hp->a[0],0,0);
	}
	(hp->size)--;
	
	//contract table
	if (hp->len > _DEFAULT_SIZE && hp->size < hp->len/4){
		hp->a = _contract_array(hp->a,hp->len);	
		hp->len /= 2; 
	} 
	_max_heapify(hp,0);
	return max;
}

void max_heap_update_key(Heap *hp,int i){
	//printf("entering update\n");
	_max_heapify(hp,i);
	//printf("after max_heapify\n");
	int j =_parent(i);
	//printf("j=%d,i=%d\n",j,i);
	while (i >0 && hp->fCompare(hp->a[j],hp->a[i]) < 0 ){
		//printf("enter while loop: j=%d,i=%d\n",j,i);
		/*PData temp = hp->a[j];
		hp->a[j] = hp->a[i];
		hp->a[i] = temp;*/
		_swap(hp,i,j);
		//printf("after swap\n");
		i=j;
		j= _parent(i);
		//printf("j=%d,i=%d\n",j,i);
	}
	//printf("update done once\n");
}




int max_heap_insert(Heap *hp,PData aItem){
	if (hp->size == hp->len){
		hp->a = _expand_array(hp->a,hp->len);
		hp->len *= 2;
	}
	
	(hp->size)++;
	hp->a[hp->size-1] = aItem;
	if (hp->on_track){
		hp->on_track(otInsert,hp->size -1,aItem,0,0);
	}
	//printf("set last item OK\n");
	max_heap_update_key(hp,hp->size -1);
	return hp->size - 1;
}

int max_heap_delete(Heap *hp,int i){
	int j =_parent(i);
	while (i > 0){
		_swap(hp,i,j);
		i=j;
		j= _parent(i);
	}
	heap_extract_max(hp);
}



Heap * newHeap(int n,CompareFunction fCompare){
	Heap * hp=malloc(sizeof(Heap));
	hp->size=0;
	hp->len=n;
	hp->a=malloc(sizeof(PData)*n);
	hp->fCompare = fCompare;
	hp->on_track = 0;
	/*hp->c = (int *)malloc(sizeof(int)*n);
	hp->o = (int *)malloc(sizeof(int)*n);
	for (int i=0;i<n;i++){
		hp->o[i] = i;
		hp->c[i] = i;
	}*/
	return hp;
}

void freeHeap(Heap *hp,int bFreeTarget){
	if (bFreeTarget)
		for(int i=0;i<hp->size;i++)
			free(hp->a[i]);
	free(hp->a);
	/*free(hp->o);
	free(hp->c);*/
	free(hp);
}

Track * newTrack(int nn){
	Track *r=(Track *)malloc(sizeof(Track));
	r->n = nn;
	r->o = (int *)malloc(sizeof(int)*nn);
	r->c = (int *)malloc(sizeof(int)*nn);
	for (int i =0 ; i<nn ; i++){
		r->o[i] =i;
		r->c[i] =i;
	}
	return r;
}

void record_swap(Track *t,int i,int j){
	t->c[t->o[i]] = j;
	t->c[t->o[j]] = i;

	int oi =t->o[i];
	t->o[i] = t->o[j];
	t->o[j] = oi;
}

void record_update(Track *t,int iCurrent,int iOrigin){
	t->o[iCurrent] = iOrigin;
	t->c[iOrigin] = iCurrent;
	
}

void freeTrack(Track *t){
	free(t->o);
	free(t->c);
	free(t);
}


int track_origin_to_current(Track *t,int iOrigin){
	return t->c[iOrigin];
}

int track_current_to_origin(Track *t,int iCurrent){
	return t->o[iCurrent];
}

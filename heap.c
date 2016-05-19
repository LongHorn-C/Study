﻿#include <stdlib.h>
#include <stdio.h>
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
		PData temp = hp->a[i];
		hp->a[i] = hp->a[largest];
		hp->a[largest] =temp;
		
		_max_heapify(hp,largest);
	}
}

static void _build_max_heap(PData *a,int n,CompareFunction fCompare){
	Heap h;
	h.a = a;
	h.len = n;
	h.size = n;
	h.fCompare = fCompare;
	
	for (int i= n/2 -1;i>=0;i--)
		_max_heapify(&h,i);
}

void heap_sort(PData *a,int n,CompareFunction fCompare){
	
	_build_max_heap(a,n,fCompare);
	Heap h;
	h.a = a;
	h.len = n;
	h.size = n;
	h.fCompare = fCompare;
	
	for (int i = n-1 ;i>=1;i--){
		PData temp= h.a[0];
		h.a[0]=a[i];
		h.a[i]=temp;
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
	(hp->size)--;
	_max_heapify(hp,0);
	return max;
}

void max_heap_update_key(Heap *hp,int i){
	printf("entering update\n");
	_max_heapify(hp,i);
	printf("after max_heapify\n");
	int j =_parent(i);
	printf("j=%d,i=%d\n",j,i);
	while (i >0 && hp->fCompare(hp->a[j],hp->a[i]) < 0 ){
		printf("j=%d,i=%d\n",j,i);
		PData temp = hp->a[j];
		hp->a[j] = hp->a[i];
		hp->a[i] = temp;
		i=j;
		j= _parent(i);
		printf("j=%d,i=%d\n",j,i);
	}
	printf("update done once\n");	
}

void max_heap_insert(Heap *hp,PData aItem){
	(hp->size)++;
	hp->a[hp->size-1] = aItem;
	max_heap_update_key(hp,hp->size -1);
}


Heap * newHeap(int n,CompareFunction fCompare){
	Heap * hp=malloc(sizeof(Heap));
	hp->size=0;
	hp->len=n;
	hp->a=malloc(sizeof(PData)*n);
	hp->fCompare = fCompare;
	return hp;
}

void freeHeap(Heap *hp,int bFreeTarget){
	if (bFreeTarget)
		for(int i=0;i<hp->size;i++)
			free(hp->a[i]);
	free(hp->a);
	free(hp);
}
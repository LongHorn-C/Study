#ifndef __HEAP_H
#define __HEAP_H
#include "ds.h"

typedef struct {
	PData *a;
	int size;
	int len;	
	CompareFunction fCompare;
} Heap;

void heap_sort(PData *a,int n,CompareFunction fCompare);

void *heap_maximum(Heap *hp);
void *heap_extract_max(Heap *hp);
void max_heap_insert(Heap *hp,void *aItem);
//void max_heap_set_key(Heap *hp,int i,void *aItem);

Heap * newHeap(int n,CompareFunction fCompare);
void freeHeap(Heap *hp,int bFreeTarget);

#endif
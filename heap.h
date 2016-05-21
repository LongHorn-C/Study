#ifndef __HEAP_H
#define __HEAP_H
#include "ds.h"



// track original index and current index after several swap(i,j) operation. 
typedef struct {
	int n;	
	int *o;// m =o[n] means current position n in data array PData *a is moved from original data array postion m .
	int *c;	//m =c[n] means original postion  n have a current position m in data array 'a'.
} Track;

Track * newTrack(int nn);
void record_swap(Track *t,int i,int j);
void record_update(Track *t,int iDes,int iSrc);
void freeTrack(Track *t); 

enum Op_type {otSwap,otSet,otRemove,otInsert};

typedef int (*Fun_on_track_queue)(enum Op_type ot,int i,PData di,int j,PData dj);


// A Heap,can be used to sort array, or be used as priority queue.
typedef struct {
	PData *a;
	int size;
	int len;	
	CompareFunction fCompare;
	Fun_on_track_queue on_track;
	//Track *track;				
} Heap;

//void heap_sort(PData *a,int n,CompareFunction fCompare);

PData heap_maximum(Heap *hp);
PData heap_extract_max(Heap *hp);
int max_heap_insert(Heap *hp,PData aItem);
void max_heap_update_key(Heap *hp,int i);
int track_origin_to_current(Track *t,int iOrigin);
int track_current_to_origin(Track *t,int iCurrent);

Heap * newHeap(int n,CompareFunction fCompare);
void freeHeap(Heap *hp,int bFreeTarget);

#endif
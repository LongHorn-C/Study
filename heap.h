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
int track_origin_to_current(Track *t,int iOrigin);
int track_current_to_origin(Track *t,int iCurrent);



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

Heap * newHeap(int n,CompareFunction fCompare);
Heap * heap_attach(Heap *h,PData *,int n,CompareFunction fCompare,Fun_on_track_queue on_track);
void freeHeap(Heap *hp,int bFreeTarget);

/*
//code example of track piority queue

typedef struct _ta{
	int v;
	int q_index;//-1 means being not in Queue;
} Ta; 


static int on_track_q(enum Op_type ot,int i,PData di,int j,PData dj){
	Ta *ta=(Ta *)di;		
	switch (ot){
		case otSwap:
			ta->q_index = i;
			Ta *tj=(Ta *)dj;
			tj->q_index = j;
			break;
		case otSet:
			ta->q_index = i;
			break;
		case otRemove:
			ta->q_index = -1;
			break;
		case otInsert:
			ta->q_index = i;
			break;
	}
}

min_queue.on_track = on_track_q;

*/

#endif
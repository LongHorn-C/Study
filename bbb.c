#include <stdio.h>
#include <stdlib.h>

#include "bbb.h"
#include "common.h"


int back_track_to_get_next(List *path,PData a_i,int ans_size,Bt_callbacks *cb,PData ctx){
	//free(a_i);
	int bFound = 0;
	while ( !bFound && path->tail != NULL){
		ListNode *tail = path->tail;
		list_delete(path,tail);
		memcpy(a_i,tail->data,ans_size);
		free(tail->data);
		free(tail);//

		bFound = cb->get_next_sibling(path,a_i,ctx);
	}
	return bFound;
}

int back_track(List *path,PData ans_incr,int ans_size,Bt_callbacks *cb,PData ctx){
	int bFound;
	do{
		int iPass = cb->pass(path,ans_incr,ctx);
		if( iPass != 0){
			PData *p_new = malloc(ans_size);
			memcpy(p_new,ans_incr,ans_size);
			list_append(path,p_new);

			if ( iPass == 2 ){
				return 1;
			}
			bFound = cb->get_first_child(path,ans_incr,ctx);
		}
		else
			bFound = cb->get_next_sibling(path,ans_incr,ctx);
		if (!bFound)
			bFound = back_track_to_get_next(path,ans_incr,ans_size,cb,ctx);
	}while(bFound);
	return 0;
}
/*
int on_append_tail__copy(List *path,PData a_i,int size,PData ctx){

	PData a2= malloc(size);
	memcpy(a2,a_i,size);
	path->tail->data = (PData) a2;
	return 0;
}

int on_delete_tail__free(List *path,PData a_i,PData ctx){
	free(a_i);
	return 0;
}
*/

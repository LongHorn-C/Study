
#include <stdio.h>
#include <stdlib.h>

#include "bbb.h"
#include "common.h"


int back_track_to_get_next(List *a_p_p,PData *a_i,Bt_callbacks *cb,PData ctx){
	//free(a_i);
	int bFound;
	do{
		ListNode *tail = a_p_p->tail; 
		list_delete(a_p_p,tail);
		*a_i = tail.data;
		bFound = cb->get_next_sibling(a_p_p,a_i,ctx);


		if (cb->on_delete_tail)
			cb->on_delete_tail(a_p_p,tail.data,ctx);
		free(tail);//
		
	}while ( !bFound && a_p_p->tail != NULL);
	return bFound;
}

int back_track(List *ans_pre_part,PData ans_incr,Bt_callbacks *cb,PData ctx){
	int bFound;
	do{
		int iPass = cb->pass(ans_pre_part,ans_incr,ctx);
		if( iPass != 0){
			list_append(ans_pre_part,ans_incr);
			if (cb->on_append_tail)
				cb->on_append_tail(a_p_a,ans_incr,ctx);
			//if (cb->is_answer(ans_pre_part,ans_incr,ctx)){
			if ( iPass = 2 ){
				return 1;
			}
			bFound = cb->get_first_child(ans_pre_part,ans_incr,ctx);
		}
		else
			bFound = cb->get_next_sibling(ans_pre_part,ans_incr,ctx);
		if (!bFound)
			bFound = back_track_to_get_next(ans_pre_part,&ans_incr,cb,ctx);
	}while(bFound);
	return 0;	
}

int on_append_tail__copy(List *a_p_p,PData a_i,int size,PData ctx){
	An * a1= (An *)a_i;
	An * a2= (An *)malloc(sizeof An);
	*a2 =*a1; //or copymem
	a_p_p.tail.data = (PData) a2;
	return 0;
}

int on_delete_tail__free(List *a_p_p,PData a_i,PData ctx){
	free(a_i);
	return 0;
}

//this file provide a framework for back_track and branch-and-bound method.
//normally, PData ctx is a pointer to the struct of the problem-definition 
//and exchangable data between functions
#ifndef __CLH_BBB_H
#define __CLH_BBB_H

#include "ds.h"
#include "tree.h"


typedef int (*F_bt1)(Tree *path,PData a_i,PData ctx);

typedef struct{
	F_bt1 pass;  //return 0: fail;1: pass; 2: final answer.
	//F_bt2 is_answer;
	F_bt1 get_first_child; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 get_next_sibling; //return 0 :not found ;1: found. new child.data stored in para a_i
	//F_bt1 on_append_tail;
	//F_bt1 on_delete_tail;
} Bt_callbacks;


int back_track_to_get_next(Tree *path,PData a_i,int ans_size,Bt_callbacks *cb,PData ctx);
int back_track(Tree *path,PData ans_incr,int ans_size,Bt_callbacks *cb,PData ctx);



//Branch and bound algorithm
typedef float (*F_bb_get)(Tree *path,PData a_i,PData ctx);
typedef struct{
	F_bt1 pass;  //return 0: fail;1: pass; 2: an optional sulution, maybe not optimal.
	F_bt1 get_first_child; //return 0 :not found ;1: found. new child.data stored in para a_i
	F_bt1 get_next_sibling; //return 0 :not found ;1: found. new child.data stored in para a_i
	//F_bt1 on_append_tail;
	//F_bt1 on_delete_tail;
	
	F_bb_get get_value;
	F_bb_get get_bound;
	//on_calc_bound;
	//on_prune
} Bb_callbacks;

PData bb_get_answer_step(PData sstNodeData);
int branch_bound(Tree *path,PData ans_incr,int ans_size,Bb_callbacks *cb,PData ctx);

//t means 

#endif


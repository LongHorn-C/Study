﻿#include <stdio.h>
static unsigned cyc_hi=0;
static unsigned cyc_lo=0;

static void access_counter(unsigned * hi,unsigned * lo){
	asm("rdtsc \n\t"  
		"movl %%edx, %0 \n\t"
		"movl %%eax, %1 "
		:"=r"(*hi),"=r"(*lo)
		:
		:"%edx","%eax"
	);
}

void start_counter(){
	access_counter(&cyc_hi,&cyc_lo);
}

double get_counter(){
	unsigned ncyc_hi,ncyc_lo;
	unsigned hi,lo,borrow;
	double result;
	access_counter(&ncyc_hi,&ncyc_lo);
	lo = ncyc_lo - cyc_lo;
	borrow = lo > cyc_lo;
	hi = ncyc_hi -cyc_hi -borrow;
	result = (double)hi *(1<<30)*4 + lo;
	if(result <0)
		fprintf(stderr,"Error:count returns neg value of  %f  \n",result);
	return result;
	
}


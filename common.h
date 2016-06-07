#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <string.h>
#include "ds.h"

typedef struct {
	int v1;
	float v2;
	char memo[11];
} Item_ex;

int i_v_compare(int a,int b);
int f_v_compare(float a,float b);

int pointerCompare(PData a,PData b);
int fCompare(PData a,PData b);
int iCompare(PData a,PData b);
int xCompare(PData a,PData b);
int randomAB(int a,int b);

void produce_ii(int *a, int n);
void print_ii(int *a, int n);
void print_ff(float *a, int n);

void print_ex(PData i);
void produce_exx(PData *a,int n);
void print_exx(PData *a,int n);

/*
#include <limits.h>
#include <float.h>
//defined macro

CHAR_MIN = -128
CHAR_MAX = 127
SCHAR_MIN = -128
SCHAR_MAX = 127
UCHAR_MAX = 255
CHAR_BIT = 8
SHRT_MIN = -32768
SHRT_MAX = 32767
USHRT_MAX = 65535
INT_MIN = -2147483648
INT_MAX = 2147483647
UINT_MAX = 4294967295
LONG_MIN = -2147483648
LONG_MAX = 2147483647
ULONG_MAX = 4294967295
FLT_MIN = 1.17549e-038
FLT_MAX = 3.40282e+038
DBL_MIN = 2.22507e-308
DBL_MAX = 1.79769e+308
LDBL_MIN = 2.22507e-308
LDBL_MAX = 1.79769e+308
*/

#endif
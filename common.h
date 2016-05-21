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

int pointerCompare(PData a,PData b);
int fCompare(PData a,PData b);
int iCompare(PData a,PData b);
int xCompare(PData a,PData b);
int randomAB(int a,int b);

void produceA(int *a, int n);
void printA(int *a, int n);

void print_ex(PData i);
void produce_ex_a(PData *a,int n);
void print_ex_a(PData *a,int n);


#endif
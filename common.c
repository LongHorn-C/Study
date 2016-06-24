// gcc -o gt g_test.c g.c common.o ds.o heap.o sort.o
//gcc -o hm bbb_hamilton.c bbb.c g.o common.o heap.o ds.c tree.c
//gcc -o as bbb_assignment.c bbb.c  tree.c ds.c 


#include <stdlib.h>
#include "common.h"



int i_v_compare(int a,int b){
	if (a==b)
		return 0;
	else 
		return a>b ? 1 : -1;
}

int f_v_compare(float a,float b){
	float r = a - b;
	if (r < 1.0e-5  && r > -1.0e-5)
		return 0;
	else
		return a > b ? 1:-1;	
}


int fCompare(PData a,PData b){
	float f1 = *((float *)a);
	float f2 = *((float *)b);
	return f_v_compare(f1,f2);
}

int iCompare(PData a,PData b){
	printf(" iCom(%d ,%d)\n",(int)a,(int)b);
	return i_v_compare(*((int *)a) , *((int *)b));
}

int pointerCompare(PData a,PData b){
	return (int)a - (int)b;
}


int xCompare(PData a,PData b){
	Item_ex *e;
	e = (Item_ex *)a;
	int k1 =e->v1;//e->v1 *100 + e->v2*10;
	e = (Item_ex *)b;
	int k2 =e->v1;// *100 + e->v2*10;

	return k1 - k2;
}


int randomAB(int a,int b){
	return a + rand() % (b-a);
}


void produce_exx(PData *a,int n){
	Item_ex *k;
	for (int i =0;i<n;i++){
		a[i] = malloc(sizeof(Item_ex));
		k=(Item_ex *)(a[i]);
		k->v1 = randomAB(1,99);
		k->v2 = randomAB(1,99)/100.01;
		strcpy(k->memo,"work?");
	}
}

void print_exx(PData *a,int n){
//	Item_ex *k;
	printf("begin Data array:\n");
	for (int i =0;i<n;i++){
	//	k=(Item_ex *)(a[i]);
		print_ex(a[i]);
	}
	printf("end of DataArray\n");
}



void print_ex(PData i){
	Item_ex *k= (Item_ex *)i;
	printf("v1=%d,v2=%f,memo=%s\n",k->v1,k->v2,k->memo);
}


void print_ii(int *a, int n){
	for (int i=0;i<n;i++)
		printf("%d,",a[i]);
	printf("\n");
}


void produce_ii(int *a, int n)
{
	for (int i=0;i<n;i++)
		a[i] = 1 + rand() % 99;

}


void print_ff(float *a, int n){
	for (int i=0;i<n;i++)
		printf("%f,",a[i]);
	printf("\n");
}


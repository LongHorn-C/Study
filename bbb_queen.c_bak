//this file is for back_track and branch-and-bound method.
//for queen problem.
#include <stdio.h>
#include <stdlib.h>
#include "ds.h"
#include "sort.h"
#include "common.h"

int pass(int r1,int c1,int r2,int c2){
	if (r1==r2 || c1 ==c2)
		return 0;
	else if (r2-r1 ==c2 -c1)
		return 0;
	else if (r1 - r2 ==c2 -c1)
		return 0;
    else
		return 1;
}


int pass_all(int *x,int i_x,int a_o){
	for (int i=0;i<i_x;i++){
			if( !pass(i,x[i],i_x,a_o) )
			   return 0;
		}
	return 1;
}


int queen(int *x,int i_x,int a_o){
    printf("\n\n content of x: ");
	print_ii(x,i_x);
	printf("   try new option on row %d column %d\n",i_x,a_o);

	if (pass_all(x,i_x,a_o)){
		if (i_x == 3){
			x[i_x] = a_o;
			printf("answer gotten:\n");
			print_ii(x,4);
			return 1;
		}
		x[i_x] = a_o;
		queen(x,i_x+1,0);
	}
	else if	(a_o < 3){
		printf("failed this one, try next sibling\n");
		queen(x,i_x,a_o+1);
	}
	else{
        printf("invalid try, go back\n ");
		do{
			i_x--;
		}while(x[i_x] == 3);
			
		if (i_x >=0){
			a_o = x[i_x] +1;
            queen(x,i_x,a_o);
		}
		else{
            printf("no solution\n");
            return 0;
		}
	}
}

void main(){
	int x[4];
	queen(x,0,0);
	//int r =pass(0,0,2,1);
	//printf("r=%d\n",pass(0,0,2,1));
	//printf("r=%d\n",pass(0,1,1,0));
	//printf("r=%d\n",pass(0,1,1,2));

}

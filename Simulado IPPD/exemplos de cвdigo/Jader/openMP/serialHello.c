#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 1000000

int *initList();
void printList(int * list);

//gcc -fopenmp openMPhello.c -o out

int main(int argc, char *argv[]){

	int i;
	int *list = initList();
	//printList(list);

	double start = clock();
	for(i = 0; i < N; ++i){
		list[i] +=5;
		usleep(0.005);
	}
	double end = clock();
	//printList(list);
		
	double elapsed = (end - start) / CLOCKS_PER_SEC;
	printf("Tempo decorrido %gs\n", elapsed);
	return 0;
}


int *initList(){
	int i;
	int *ret = (int *)malloc(N*sizeof(int));

	for (i = 0; i < N; ++i){
		ret[i] = i + 10;
	}

	return ret;
}
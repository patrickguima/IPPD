#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#define N 1000000

void hello();
int *initList();
void printList(int * list);

//gcc -fopenmp openMPhello.c -o out

int main(int argc, char *argv[]){

	int num_thread;
	int i;
	

	omp_set_num_threads(4); //indica o numero de threads
	num_thread = omp_get_thread_num(); //retorna o ID da thread 

	int *list = initList();
	//printList(list);

	//# pragma omp parallel num_threads(num_thread)
	//hello();
	double start = clock();
	# pragma omp parallel for num_threads(num_thread)
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

void hello(){
	int my_rank = omp_get_thread_num();
	int num_thread = omp_get_num_threads();

	printf("oi da thread %d de %d threads\n", my_rank, num_thread );
}

int *initList(){
	int i;
	int *ret = (int *)malloc(N*sizeof(int));

	for (i = 0; i < N; ++i){
		ret[i] = i + 10;
	}

	return ret;
}

void printList(int * list){
	
	for (int i = 0; i < N; ++i)	{
		printf("%d ", list[i]);
	}
	printf("\n");
	return;
}
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <assert.h>



int main(){

	int *vetor;
	int  i =0;
	int sum=0;
	int n=10;
	vetor = (int *)malloc(10*sizeof(int));
	int n_per_thread; 
	n_per_thread = n/2;

	printf("n_per_thread %d\n", n_per_thread); 
	for (i = 0;i<n;i++){
		vetor[i] = 1;
	}

	#pragma omp parallel for reduction(+:sum)
	for (i = 0;i<n;i++){
		sum += vetor[i];
	}

	//for (i = 0;i<10;i++){
		printf("sum %d\n", sum);
	//}



	free(vetor);
}

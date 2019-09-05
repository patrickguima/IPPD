#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void merge(int vetor[],int vetAux[], int comeco, int meio, int fim) {
	int k;
    int com1 = comeco;
    int comAux = comeco;
    int com2 = meio+1;
    

    //int *vetAux;

    //vetAux = (int*)malloc(tam * sizeof(int));


    while((com1 <= meio) && (com2 <= fim)){

        if(vetor[com1] < vetor[com2]) {

            vetAux[comAux] = vetor[com1];

            com1++;

        } else {

            vetAux[comAux] = vetor[com2];

            com2++;

        }

        comAux++;

    }

    if(meio < com1) {
		
		for(k = com2; k <= fim; k++) {
			
			vetAux[comAux] = vetor[k];
			comAux++;
			
			}
			
		}
		
	else {
		
		for(k = com1; k <= meio; k++) {
			
			vetAux[comAux] = vetor[k];
			comAux++;
			
			}
			
		}


    for(k = comeco; k <= fim; k++){    //Move os elementos de volta para o vetor original

        vetor[k] = vetAux[k];

   		}

    

   

}


void mergeSort(int vetor[], int vetAux[],int comeco, int fim){

    if (comeco < fim) {

        int meio = (fim+comeco)/2;


        mergeSort(vetor,vetAux, comeco, meio);

        mergeSort(vetor,vetAux, meio+1, fim);

        merge(vetor,vetAux, comeco, meio, fim);

    }

}

int main(){
	int size_vec = 10;
	int *vetor = malloc(size_vec*sizeof(int)); 
	
	int i = 0;
	srand(time(NULL));
	for (i = 0;i<size_vec;i++){
		vetor[i] = rand() % size_vec;
		
	}
	for (i = 0;i<size_vec;i++){
		printf("%d ",vetor[i]);
		
	}
	

	int world_rank;
	int world_size;
	
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int size = size_vec/world_size;
	int comeco = 0;
	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(vetor, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	int *tmp_array = malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));
	//mergeSort(vetor,comeco,fim);


	int *sorted = NULL;
	if(world_rank == 0) {
		
		sorted = malloc(size_vec * sizeof(int));
		
		}
	
	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

	printf("\n");

	if(world_rank == 0) {
		
		int *other_array = malloc(size_vec * sizeof(int));
		mergeSort(sorted, other_array, 0, (size_vec - 1));
		
		/********** Display the sorted array **********/
		printf("Vetor ordenado: ");
		int c;
		for(c = 0; c < size_vec; c++) {
			
			printf("%d ", sorted[c]);
			
			}
			
		printf("\n");
		printf("\n");
			
		/********** Clean up root **********/
		free(sorted);
		free(other_array);
			
		}


	//i = 0;
	//for (i = 0;i<9;i++){
	//	printf("%d ",vetor[i]);
	//} 
	//printf("\n");

	free(vetor);
	free(sub_array);
	free(tmp_array);
	
	/********** Finalize MPI **********/
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
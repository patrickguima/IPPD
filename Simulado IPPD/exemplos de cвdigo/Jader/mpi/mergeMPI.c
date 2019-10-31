#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


// colocar o subarray da funcao mpi_merge dentro da struct
#define SIZE 100

int *generateRandomIntegers(int *vet);
void printArray(int *vet);
//mpi_data *initData();
//void initMPI();
//void MPI_SCAT(mpi_data *control_data, int *sub_Array);
//void MPI_GAT(mpi_data *control_data, int *sub_Array);
//void MPI_FIN();

void merge(int vetor[], int vetAux[], int comeco, int meio, int fim) {
    
    int com1 = comeco, com2 = meio+1, comAux = comeco, tam = fim-comeco+1, k;
    

    while((com1 <= meio) && (com2 <= fim)){

        if(vetor[com1] < vetor[com2]) {
            vetAux[comAux] = vetor[com1];
            com1++;
        } 
        else {
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

    for(k = comeco; k <= fim; k++){ // coloca os elementos no vetor original
        vetor[k] = vetAux[k];
  	}

}

void mergeSort(int vetor[], int aux_array[], int comeco, int fim){
    
    // printf("Olá eu sou o processo[%d]\n", rank);

    if (comeco < fim) {
        int meio = (fim+comeco)/2;
        mergeSort(vetor, aux_array, comeco, meio);
        mergeSort(vetor, aux_array, meio+1, fim);    
        merge(vetor, aux_array, comeco, meio, fim);
    }
}

int main(int argc, char *argv[]){

    // initMPI(); // inicializa a execução na biblioteca

    int i;

    int *array = (int *)malloc(SIZE * sizeof(int));

    srandom(time(NULL));
    for(i = 0; i < SIZE; i++){ //gera SIZE numeros aleatórios
        array[i] = (int) random() % SIZE; 
                 
    }

    int rank, num_proc;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc); // indica o número de processos envolvidos em um comunicador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Essa função fornece o rank do processo no grupo do comunicador específico 

    int sub_size = SIZE / num_proc; // tamanho dos subvetores que serão distribuídos
    int *sub_array = (int *) malloc(sub_size * sizeof(int)); // subvetor para distribuir pelos cores


    if(rank == 0)
        printArray(array); 

    MPI_Scatter(array, sub_size, MPI_INT,
    sub_array, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    int *aux_array = (int *)malloc(sub_size * sizeof(int));

    printf("Olá eu sou o processo[%d]\n", rank);

    mergeSort(sub_array, aux_array, 0, sub_size-1);
    
    
    int *sorted = NULL;
	if(rank == 0) {
		sorted = (int *) malloc(SIZE * sizeof(int));	
	}

    MPI_Gather(sub_array, sub_size, MPI_INT,
    sorted, sub_size, MPI_INT, 0,
    MPI_COMM_WORLD);

    if(rank == 0) {
		
		int *other_array = malloc(SIZE * sizeof(int));
		mergeSort(sorted, other_array, 0, (SIZE - 1));
		
		printf("Vetor ordenado: ");
		printArray(sorted);

		free(sorted);
		free(other_array);
			
	}

    MPI_Barrier(MPI_COMM_WORLD); // Sincronização entre processos MPI num grupo
	MPI_Finalize();

    return 0;
}

int *generateRandomIntegers(int *vet){
    
    int i;

    vet = (int *)malloc(SIZE*sizeof(int));
    srandom(time(NULL));
    for(i = 0; i < SIZE; i++){ //gera SIZE numeros aleatórios
        vet[i] = (int) random() % SIZE; 
                 
    }

    return vet;
}

void printArray(int *vet){
    int i;

    for(i = 0; i < SIZE; i++){
            printf("%d ", vet[i]);     
    }
    printf("\n");

}

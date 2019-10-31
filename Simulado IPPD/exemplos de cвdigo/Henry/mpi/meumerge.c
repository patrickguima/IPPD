#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int vetor[], int comeco, int meio, int fim);
void mergeSort(int vetor[], int comeco, int fim);
void Listar(int vetor[], int tamanho);
void PreencheVetor(int vetor[],int tamanho);
int main(int argc, char** argv){
    clock_t t_start, t_end;
    double sort_time_stamp, clone_data_time_stamp;
     int TAMANHO_TOTAL = 10; 
    int *vetor =(int *) malloc(TAMANHO_TOTAL * sizeof(int));
    PreencheVetor(vetor,TAMANHO_TOTAL);
	int numProcess,rank;
	
    /** Inicializa MPI **/
	MPI_Init(NULL, NULL);
     
    MPI_Comm_size(MPI_COMM_WORLD, &numProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /** quantidade de elementos que vai para cada 1**/
    int size = TAMANHO_TOTAL/numProcess;

    /*** cria um sub_array para cada processo **/
	int *sub_array =(int *) malloc(size * sizeof(int));
    t_start = clock();
    /** divide os dados igualmente entre os processos **/
	MPI_Scatter((void*)vetor, //buffer de envio
        size, //numero de elementos enviados
        MPI_INT, //tipo de dado do buffer
        (void*)sub_array, // buffer que recebe
        size, // numero de elementos que o buffer vai receber 
        MPI_INT,//tipo de dado do buffer
        0, //cabeça
        MPI_COMM_WORLD //comunicador
    );
    t_end = clock();
	clone_data_time_stamp = (double) t_end - t_start;
    t_start = clock();
    /** MergeSort para cada Processo **/
	    mergeSort(sub_array, 0, (size - 1));

    /** Reunir subraizes no processo 0 **/
    int *sorted = NULL;
	if(rank == 0) {
		/** vetor criado para reunir o que processos ordenaram **/
		sorted =(int *) malloc(TAMANHO_TOTAL * sizeof(int));
		t_end = clock();
		sort_time_stamp = (double) t_end - t_start;
		}
	/** Pega os dados de cada processo e reune em um**/
	MPI_Gather((void*)sub_array, // buffer de envio
        size, //numero de elementos enviados
        MPI_INT,//tipo de dado do buffer
        (void*)sorted, // buffer que recebe
        size,// numero de elementos que o buffer vai receber 
        MPI_INT,//tipo de dado do buffer
        0,//cabeça
        MPI_COMM_WORLD //comunicador
    );
    if(rank== 0) {
		/** MergeSort Final com todos os dados **/
		mergeSort(sorted, 0, (TAMANHO_TOTAL - 1));
        Listar(sorted,TAMANHO_TOTAL);
        printf("tem de dividir %lf\n", clone_data_time_stamp/CLOCKS_PER_SEC);
        printf("tem de juntar %lf\n",sort_time_stamp/CLOCKS_PER_SEC);

    }
    free(sorted);
	free(sub_array);
	free(vetor);
	MPI_Finalize();

    return 0;

}
void PreencheVetor(int vetor[],int tamanho){
    srand(time(NULL));
   for(int i = 0; i < tamanho; i++) 
    { 
        vetor[i] = rand()%100; 
    }
}
void merge(int vetor[], int comeco, int meio, int fim) {
    int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
    int *vetAux;
    vetAux = (int*)malloc(tam * sizeof(int));

    while(com1 <= meio && com2 <= fim){
        if(vetor[com1] < vetor[com2]) {
            vetAux[comAux] = vetor[com1];
            com1++;
        } else {
            vetAux[comAux] = vetor[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= meio){  //Caso ainda haja elementos na primeira metade
        vetAux[comAux] = vetor[com1];
        comAux++;
        com1++;
    }

    while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
        vetAux[comAux] = vetor[com2];
        comAux++;
        com2++;
    }

    for(comAux = comeco; comAux <= fim; comAux++){    //Move os elementos de volta para o vetor original
        vetor[comAux] = vetAux[comAux-comeco];
    }
    
    free(vetAux);
}

void mergeSort(int vetor[], int comeco, int fim){
    if (comeco < fim) {
        int meio = (fim+comeco)/2;

        mergeSort(vetor, comeco, meio);
        mergeSort(vetor, meio+1, fim);
        merge(vetor, comeco, meio, fim);
    }
}

void Listar(int vetor[], int tamanho) 
{ 
    for (int i = 0; i < tamanho; i++) 
    { 
        printf("%i\n", vetor[i]); 
    } 
}
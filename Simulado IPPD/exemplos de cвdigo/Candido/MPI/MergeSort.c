/* Compile: mpicc ex1.c -o ex1
   Run: mpiexec -n 2 ex1*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void mergeSort(int *vetor, int posicaoInicio, int posicaoFim);
void merge(int *saida, int *auxiliar, int inicio, int meio, int fim);

int main(int argc, char** argv) {
  int i;
  // Initialize the MPI environment  
  // Find out rank, size
  double inicio,fim;
  inicio=MPI_Wtime();
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
 
  int *vetor,*vetor_aux,*saida;
  
  vetor = malloc(10*sizeof(int));
  vetor_aux = malloc(10*sizeof(int));


  	if (world_rank == 0) {
      for(i=0;i<10;i++){
        vetor[i]=rand() % 100;
        printf("%d ",vetor[i]);
      }
      printf("\n");
  	}

  MPI_Scatter(vetor,(10/world_size),MPI_INT,vetor_aux,(10/world_size),MPI_INT,0,MPI_COMM_WORLD);
  mergeSort(vetor_aux,0,(10/world_size)-1);
  if (world_rank == 0) {
  	  saida = malloc(10*sizeof(int));
  }
  MPI_Gather(vetor_aux, (10/world_size), MPI_INT, saida, (10/world_size), MPI_INT, 0, MPI_COMM_WORLD);
  if (world_rank == 0) {
    mergeSort(saida,0,9);
    printf(" *** ok *** \n");
    for(i=0;i<=9;i++) {
        printf("%d ",saida[i]);
    } 
    printf("\n");
    	fim=MPI_Wtime();
	printf("Tempo %f \n",fim-inicio);
      }  
  MPI_Finalize();
}


void mergeSort(int *vetor, int posicaoInicio, int posicaoFim) {
    int inicio,i, metade, k, metadeTamanho, *vetorTemp;

    if(posicaoInicio == posicaoFim) return;

    // ordenacao recursiva das duas metades
    metadeTamanho = (posicaoInicio + posicaoFim )/ 2;
    mergeSort(vetor, posicaoInicio, metadeTamanho);
    mergeSort(vetor, metadeTamanho + 1, posicaoFim);

    // intercalacao no vetor temporario t
    inicio = posicaoInicio;
    metade = metadeTamanho + 1;
    k = 0;
    vetorTemp = (int *) malloc(sizeof(int) * (posicaoFim - posicaoInicio + 1));

    while(inicio < metadeTamanho + 1 || metade  < posicaoFim + 1) {
        if (inicio == metadeTamanho + 1 ) { // i passou do final da primeira metade, pegar v[metade]
            vetorTemp[k] = vetor[metade];
            metade++;
            k++;
        }
        else {
            if (metade == posicaoFim + 1) { // j passou do final da segunda metade, pegar v[inicio]
                vetorTemp[k] = vetor[inicio];
                inicio++;
                k++;
            }
            else {
                if (vetor[inicio] < vetor[metade]) {
                    vetorTemp[k] = vetor[inicio];
                    inicio++;
                    k++;
                }
                else {
                    vetorTemp[k] = vetor[metade];
                    metade++;
                    k++;
                }
            }
        }

    }
    // copia vetor intercalado para o vetor original
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        vetor[i] = vetorTemp[i - posicaoInicio];
    }
    free(vetorTemp);
}




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define TAM 100

void imprimeVetor(int *vet,int tam){
	int i;
	for(i=0;i<tam;i++){
		printf("%d ", vet[i]);
	}
	printf("\n");
}

void merge(int *vet,int inicio,int meio,int fim){
   int *aux, i, j, k,tam,t,fim1,fim2;
   i = inicio;
   j = meio+1;
   fim1 = 0;
   fim2 = 0;
   tam = fim - inicio + 1;

   aux = (int *)malloc(sizeof(int)*TAM);

   for(k = 0; k < tam; k++){
     if( (!fim1) && (!fim2) ){
      if(vet[i] < vet[j]){
        aux[k] = vet[i];
        i++;
      }
      else{
        aux[k] = vet[j];
        j++;  
      }

      if(i > meio)
        fim1 = 1;
      if(j > fim)
        fim2 = 1;
      }else{
        if(fim1 != 1){
          aux[k] = vet[i];
          i++;
        }else{
          aux[k] = vet[j];
          j++;
        }
      }
  }

  for(k = 0; k < tam; k++){
      vet[inicio+k] = aux[k];
  }
  
  free(aux);
  
}

int mergeSort(int *vet,int inicio,int fim){
     int meio;
     if(inicio < fim){
     	meio = floor(inicio+fim)/2;
        mergeSort(vet,inicio,meio);
        mergeSort(vet,meio+1,fim);
        merge(vet,inicio,meio,fim);
        return 0;
     }
     return 1;
}


int main(int argc, char const *argv[])
{
  int *vet,*aux,i,my_rank,num_proc,tam;
  double inicio_t = MPI_Wtime(),tempo_cpu;
  clock_t inicio,fim,t_proc;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&num_proc);
  MPI_Barrier(MPI_COMM_WORLD);

  vet = (int *)malloc(sizeof(int)*TAM);
  if(vet==NULL){
      exit(1);
  }
  srand(time(NULL));
  for(i=0;i<TAM;i++){
      vet[i] = rand()%TAM; //gera vetor com valores pseudo-aleatorios
  }

  if(my_rank==0){
    printf("Vetor Original:\n");
    imprimeVetor(vet,TAM);
  }

  tam = TAM/num_proc; 
  aux = (int *)malloc(tam*sizeof(int));

  inicio = MPI_Wtime();

  MPI_Scatter(vet,tam,MPI_INT,aux,tam,MPI_INT,0,MPI_COMM_WORLD);

  if(my_rank!=0){
     MPI_Send(&aux,tam,MPI_INT,0,0,MPI_COMM_WORLD);
     mergeSort(aux,0,tam-1);
    // MPI_Recv(&aux,tam,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  }

  t_proc += MPI_Wtime() - inicio; 

  inicio = MPI_Wtime();
  MPI_Gather(aux,tam,MPI_INT,vet,tam,MPI_INT,0,MPI_COMM_WORLD);

  fim = MPI_Wtime() - inicio_t;
  
  if(my_rank==0){  
       MPI_Send(&vet,TAM,MPI_INT,0,0,MPI_COMM_WORLD);
     if(mergeSort(vet,0,TAM-1)==0){
       MPI_Recv(&vet,TAM,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Vetor Ordenado:\n");
        imprimeVetor(vet,TAM);
    }
  }

  free(vet);
  free(aux);
  fim = clock();

  MPI_Barrier(MPI_COMM_WORLD);
  tempo_cpu = MPI_Wtime() - inicio_t;
  //printf("Função demorou %f segundos para ordenar\n", tempo_cpu);

  MPI_Finalize();
	return 0;
}

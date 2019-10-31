#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define OMP_NESTED 1

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


        //omp_set_nested(1);

        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
            {
                mergeSort(vetor, comeco, meio);
            }

            #pragma omp section
            {
                mergeSort(vetor, meio+1, fim);

            }
        }

        merge(vetor, comeco, meio, fim);
    }
}

void main(int argc, char** argv){

    time_t t;
    int v_size = atoi(argv[1]);
    int *v = malloc(v_size * sizeof(int));

    //initialize array
    printf("Tamanho do vetor: %d\n",v_size);
    srand((unsigned) time(&t));

    for(int i = 0; i < v_size; i++){
        v[i] = rand()%v_size;
    }

    for(int i =0; i < v_size;i++){
      printf("%d\n", v[i]);
    }
    mergeSort(v,0,v_size);
    printf("\n\n");
    
    for(int i =0; i < v_size;i++){
      printf("%d\n", v[i]);
    }

    //for(int i = 0; i < v_size; i++){
    //    printf("%d\n",v[i]);
    //}
    int *sorted = NULL;
    free(v);
}

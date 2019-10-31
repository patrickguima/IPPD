#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <time.h>
#include <unistd.h>

/*
    DUVIDAS:
        
*/

/*
    Eleva cada valor do vetor ao quadrado e soma com o seu indice
    
    Executar e compilar:
        $mpicc code.c -o out
        $mpirun -n 5 ./out
        $mpiexec -n 4 out
    
        -n : número de processos.
*/

#define SIZE_ARRAY 500000

int * init_array();
void print_array(int array[]);

int main(int argc, char const *argv[]){
    

    int rank, num_proc; 
    MPI_Init(NULL, NULL);  // Inicializa o ambiente de execução MPI
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);  // Retorna o tamanho do grupo associado a um comunicador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // determina a classificação do processo no comunicador

    
    int array_slice_size = SIZE_ARRAY / num_proc;
    int * array = init_array();
    int * array_aux = malloc(array_slice_size * sizeof(int)); // vetor fatiado para recebimento de mensagens
    int i;

    /*
    if (rank == 0)
        print_array(array);
    */
    
    /* 
        Scatter - envia dados de uma tarefa para todas as tarefas em um grupo
        Todos os argumentos para a função são significativos para a raíz do processo ,
        enquanto em outros processos, apenas os argumentos 4, 5, 6, 7 e 8 são significativos.
    */
        
    MPI_Scatter(array, array_slice_size, // Endereço do buffer de envio, num de elementos enviados para cada processo  
    MPI_INT, array_aux, array_slice_size,  // Tipo de dados dos elementos do buffer de envio, Endereço do buffer de recebimento, Num de elem no buffer de recebimento
    MPI_INT, 0, MPI_COMM_WORLD);  // Tipo de dados dos elementos do buffer de recebimento, rank do processo de envio, id do comunicador

    
    
    for (i = 0; i < array_slice_size; ++i){
        array_aux[i] = i + (array_aux[i] * array_aux[i]);
        usleep(0.5);
    }
    
    /* Gather - Reúne valores de um grupo de processos.
        Cada processo (processo raiz incluído) envia o conteúdo de seu buffer de envio para o processo raiz.
        O processo raiz recebe as mensagens e as concatena em ordem de classificação. 
        O buffer de recebimento(array) é ignorado para todos os processos não raiz.
    */

    MPI_Gather(array_aux, array_slice_size,   //  Endereço inicial do buffer de envio, num de elementos no buffer de envio
    MPI_INT, array, array_slice_size, MPI_INT, // tipos de dados do bufer de envios, Endereço do buffer de recebimento, tamanho do buff receb, tipo do buff de reb
    0, MPI_COMM_WORLD); // id do processo raiz e id comunicador

    /*
    if(rank == 0)
        print_array(array);
    */

    MPI_Finalize(); 
    return 0;
}

int * init_array(){
    
    int i;
    int * array = malloc(SIZE_ARRAY * sizeof(int));

    for (i = 0; i < SIZE_ARRAY; ++i){  // para ter valores para conferir resultado da soma
        array[i] = i+1;
    }
    

    return array;
}

void print_array(int array[]){
    int i;
    for (i = 0; i < SIZE_ARRAY; ++i){
        printf("%d ", array[i]);
    }
    printf("\n");
}
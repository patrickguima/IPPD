#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "merge_sort.h"
#include "../../utils/gen_random_ints.h"
#include "../../utils/cronometer.h"


/* Espera por uma tarefa e devolve o resultado */
void worker (void* recv_buffer, int recv_count, void *result_buffer, int root_rank, int my_rank) {
    int *arr = (int *) recv_buffer;
    if (my_rank != root_rank) 
        MPI_Scatter(
            NULL,
            -1,
            MPI_INT,
            recv_buffer,
            recv_count,
            MPI_INT,
            root_rank,
            MPI_COMM_WORLD
        );
    
    merge_sort(arr, 0, recv_count - 1);

    MPI_Gather(
        (void *) arr,
        recv_count,
        MPI_INT,
        result_buffer,
        recv_count,
        MPI_INT,
        root_rank,
        MPI_COMM_WORLD
    );
}


/* Processo que controla e divide as tarefas */
void maestro (
        void *recv_buffer, 
        int* result_buffer, 
        int my_rank, 
        int arr_length, 
        int range) {

    int *arr = gen_random_ints(arr_length, range);
    int num_workers;
    MPI_Comm_size(MPI_COMM_WORLD, &num_workers);
    int sub_length = arr_length / num_workers;
    MPI_Scatter(
        (void*) arr,
        sub_length,
        MPI_INT,
        recv_buffer,
        sub_length,
        MPI_INT,
        my_rank,
        MPI_COMM_WORLD        
    );

    worker(recv_buffer, sub_length, (void*) arr, my_rank, my_rank);

    int middle = sub_length;
    for (int i=0; 
            i<num_workers - 1; 
            ++i) {
        merge(arr, 0, middle - 1 , (middle + sub_length) - 1);
        middle += sub_length;
    }

    if (assert_asc_order(arr, arr_length))
        printf("Success: The Array is Sorted!\n");
    else { 
        printf("FAILURE: The Array is NOT Sorted!\n");
    }
    free(arr);
}


int main () {
    /* Init */
    MPI_Init(NULL, NULL);
    int root_rank = 0;
    int arr_length = 1000;
    int range = 100;
    int num_workers; 
    MPI_Comm_size(MPI_COMM_WORLD, &num_workers);
    int recv_count = arr_length / num_workers; 
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int *recv_buffer = malloc(sizeof(int) * arr_length);
    int *result_buffer = NULL;

    /* Work */
    if (my_rank == root_rank) maestro(recv_buffer, result_buffer, root_rank, arr_length, range);
    else worker((void *) recv_buffer, recv_count, NULL, root_rank, my_rank);
    
    free(recv_buffer);    
    MPI_Finalize();
    return 0;
}
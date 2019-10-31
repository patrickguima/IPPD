
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int count=0;
  int token;
 /* double inicial[world_size];
  double final[world_size];
  double fim[world_size];*/
  if (world_rank != 0) {

 /* for (int i = 0; i < world_size; ++i)
  {
    inicial[i] = MPI_Wtime();
  }
  count++;*/
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_rank - 1);
  } else {
    token = -1;
  }
  MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,
           MPI_COMM_WORLD);
  if (world_rank == 0) {
    MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
   // final[count]=MPI_Wtime();
    //count++; 
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_size - 1);
  }
  MPI_Finalize();
  /*for (int i = 0; i < world_size; ++i)
  {
     fim[i] = final[i]-inicial[i];
    printf("tempo %i %.2f\n",i,fim[i]);
  }*/
}
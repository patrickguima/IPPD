#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

int main(void) {

  int my_rank,num_proc,rank2,cont=0;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  assert(num_proc==2 || num_proc==1);

  rank2 = (my_rank + 1) % 2;
  while (cont < 10) {
  if(my_rank == cont % 2){
      cont++;
      printf("Processo %d está passando a bola para o processo %d\n",my_rank,rank2);
      MPI_Send(&cont, 1, MPI_INT, rank2, 0, MPI_COMM_WORLD);
    } else {
      printf("Processo %d está recebendo a bola do processo %d\n",my_rank,rank2);
      MPI_Recv(&cont, 1, MPI_INT, rank2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
  }
  MPI_Finalize();
  return 0;
}
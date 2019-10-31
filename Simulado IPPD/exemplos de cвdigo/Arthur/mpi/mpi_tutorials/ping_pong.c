#include <stdio.h>
#include <mpi.h>

const int PING_PONG_LIMIT = 10;

int main() {
  int world_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int ping_pong_count = 0;
  int partner_rank = (world_rank + 1) % 2;
  while (ping_pong_count < PING_PONG_LIMIT) {
      if (world_rank == ping_pong_count % 2) {
          // Increment the ping pong count before you send it
          ping_pong_count++;
          MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                  MPI_COMM_WORLD);
          printf("%d sent and incremented ping_pong_count "
                "%d to %d\n", world_rank, ping_pong_count,
                partner_rank);
      } else {
          MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("%d received ping_pong_count %d from %d\n",
                world_rank, ping_pong_count, partner_rank);
      }
  }

  MPI_Finalize();

  return 0;
}
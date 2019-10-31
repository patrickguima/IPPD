#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#include "cronometer.h"

int main () {
    const int PING_PONG_LIMIT = 10;
    MPI_Init(NULL, NULL);

    int world_rank;
    int ping_pong_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int partner_rank = (world_rank + 1) % 2;

    /* Time it */
    struct cronometer c_outer = init_cronometer();
    c_outer = start(c_outer);
    struct cronometer c = init_cronometer();
    while (ping_pong_count < PING_PONG_LIMIT) {
        if (world_rank == ping_pong_count % 2) {
            c = start(c);
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                    MPI_COMM_WORLD);
            printf("Process %d sent and incremented ping_pong_count "
                "%d to process %d\n", world_rank, ping_pong_count,
                partner_rank);
            c = end(c);
            printf("Took %lf to send message\n", diff(c));
            c_outer = end(c_outer);
            printf("Current CPU time %lf\n\n", diff(c_outer));
        }
        else {
            c = start(c);
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d received ping_pong_count %d from %d\n",
                world_rank, ping_pong_count, partner_rank);
            c = end(c);
            printf("Took %lf to recieve message\n", diff(c));
            c_outer = end(c_outer);
            printf("Current CPU time %lf\n\n", diff(c_outer));
        }
    }
    MPI_Finalize();
    c_outer = end(c_outer);
    printf("\nTook %lf to end program\n\n", diff(c_outer));
}

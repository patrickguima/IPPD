#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Comunicação em Anel

int main(void){
    int my_rank,num_proc,messagem=0;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); //determina o identificador do processo corrente
    MPI_Comm_size(MPI_COMM_WORLD,&num_proc); //determina o numero de processos
   
    int prox = (my_rank+1)%num_proc; //calcula o ramk do proximo processo no anel
    int proc_ant = num_proc - 1; //processo anterior
    int rank_ant = my_rank - 1; //rank anterior

    if(my_rank==0){
      messagem = -1;
      printf("Processo %d enviou %d do processo %d\n",my_rank,messagem,prox);
      MPI_Send(&messagem,1,MPI_INT,prox,0,MPI_COMM_WORLD); //envia a mensagem para o proximo processo
      MPI_Recv(&messagem,1,MPI_INT,proc_ant,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("Processo %d recebeu %d do processo %d\n",my_rank,messagem,proc_ant); //processo 0 recebe mensagem do ultimo processo
    }else{ 
      MPI_Recv(&messagem,1,MPI_INT,rank_ant,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //recebe a mensagem do processo anterior
      printf("Processo %d recebeu %d do processo %d\n",my_rank,messagem,rank_ant);
      printf("Processo %d enviou %d do processo %d\n",my_rank,messagem,prox);
      MPI_Send(&messagem,1,MPI_INT,prox,0,MPI_COMM_WORLD); //envia a mensagem para o proximo processo
    }
     
    MPI_Finalize();
    return 0;
}

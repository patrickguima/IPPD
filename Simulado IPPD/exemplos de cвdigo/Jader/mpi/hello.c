#include <stdio.h>
#include <mpi.h>

    /* Exercicio
        processo 0 envia valores para os outros processos, 
        os outros vão multiplicar o valor recebido e devolver
        para o processo 0, que vai somar todos os valores
    */


int main(int argc, char *argv[]){
    
    int size, myRank;
    int valor_inicial, valor_modificado, total, num_proc;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // descobrir quantos processos
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // para saber o id de cada um

    printf("Hello! sou o processo %d de um total de %d\n", myRank, size);
    fflush(stdout); //sincronização para não ter erro nas mensagens
    
    if(myRank == 0){ //se eu sou o processo mestre
        for(num_proc = 1; num_proc < size; ++num_proc){ //envia os valores para os escravos
            valor_inicial = num_proc * 10;
            MPI_Send(&valor_inicial, 1, MPI_INT, num_proc, 0, MPI_COMM_WORLD);
            printf("Mestre - Enviei %d para processo %d\n", valor_inicial, num_proc);
        }

        total = 0;

        for(num_proc = 1; num_proc < size; ++num_proc){ //recebe os valores dos escravos
            MPI_Recv(&valor_modificado, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &st); 
            // ANY_SOURCE porque pode vir as mensagens em qualquer ordem
            // Como o escravo me manda da tag 1, eu ecebo da tag 1
            printf("Mestre - recebi o valor modificado %d\n", valor_modificado);
            fflush(stdout);

            total+=valor_modificado;
        }
        printf("Mestre - total = %d\n", total);
        fflush(stdout);
    }
    else{ //se eu sou o processo escravo
        MPI_Recv(&valor_inicial, 1 , MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
        printf("Escravo[%d], recebi %d do mestre\n", myRank, valor_inicial);
        fflush(stdout);
        valor_modificado = valor_inicial * 10;
        MPI_Send(&valor_modificado, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("Escravo[%d] - valor modificado = %d\n", myRank, valor_modificado);
    }
    

    MPI_Finalize();

    return 0;
}

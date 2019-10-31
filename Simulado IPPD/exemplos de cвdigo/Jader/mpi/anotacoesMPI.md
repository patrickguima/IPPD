# MPI Anotações

## Primitivas

### MPI_Init : 
    inicialização para o ambiente MPI

### MPI_Comm_size :
    retorna o número de processadores

### MPI_Comm_rank : 
    retorna o "rank" (índice, identificador) do processador

### MPI_Send : 
    Envia mensagem mesmo q o recepetor não esteja executando Recv(Caso tamanho seja menor que o limite). 
    O MPI implementa um buffer que armazena a mensagem. MPI é envio de dados BLOQUEANTE. Caso seja > limite, o emissor aguarda o receptor sincronizar pra receber o dado.

    MPI_SEND(void *buf, int count, MPI_Datatype, int dest, int tag, MPI_COMM comm);    

    parâmetros : endereço inicial dos dados enviados, número de elementos a serem enviados, tipo do dado, processo destino, rótulo da mensagem, comunicador MPI

### MPI_Recv : 
    recebe uma mensagem.

    parâmetros : endereço inicial dos dados recebidos, número de elementos a serem recebidos, tipo do dado, processo fonte esperado, rótulo da mensagem esperado, comunicador MPI, processo fonte e rótulos efetivos(controle que diz, origem, tamanho e rótulos).

### MRP_Finilize : 
    sai do ambiente MPI

### Tipos básicos no MPI
    MPI_CHAR, MPI_SHORT, MPI_INT, MPI_LONG, MPI_FLOAT, MPI_DOUBLE, MPI_LONG_DOUBLE, MPI_BYTE, MPI_PACKED(mandar structs inteiras).


### Executar
    Baixar a biblioteca open-mpi;
    
    $mpicc code.c -o out
    $mpirun -n 5 ./out
    $mpiexec -n 4 out
    
    -n : número de processos.
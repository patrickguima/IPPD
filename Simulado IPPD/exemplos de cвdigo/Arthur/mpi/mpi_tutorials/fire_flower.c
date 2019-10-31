/*
  Nos dias atuais uma flor de fogo não é algo considerado estranho para muitos jovens.
  Isso porque um famoso jogo de videogame popularizou esse tipo de flor. Nesse jogo o
  protagonista ganhava superpoderes ao tocar em uma flor de fogo, passando a atirar
  pequenas bolas de fogo para derrotar seus inimigos.

  No entanto, já se falava sobre flores de fogo há muito tempo atrás. Na mitologia
  polonesa, flores de fogo são flores místicas de grande poder guardadas por espíritos
  malignos. Ela possuía esse nome porque brilhava tanto que era impossível olhá-la
  diretamente. Quem possuísse uma flor dessas ganharia a habilidade de ler a mente de
  outras pessoas, encontrar tesouros escondidos e repelir todos os males.

  Para obter uma flor de fogo, a pessoa deveria procurá-la em uma floresta antes da
  meia-noite na véspera do Noc Kupały. Exatamente à meia-noite ela floresceria. Para
  colhê-la seria preciso desenhar um círculo em volta dela. Parece uma tarefa fácil, no
  entanto, os espíritos malignos que guardam a flor tentariam de tudo para distrair qualquer
  um tentando colher a flor. Se a pessoa falhasse ao tentar desenhar um círculo em volta da
  flor, teria sua vida sacrificada.

  Dados dois círculos, um desenhado por um ambicioso caçador de flores de fogo e outro
  representando a área da flor, sua tarefa é determinar se o caçador morre ou fica rico com
  sua conquista.
*/

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

typedef struct {
  int R, X, Y;
} circle;

circle generate_circle(int chance) {
  circle c;
  c.R = rand() % chance;
  c.X = rand() % chance;
  c.Y = rand() % chance;

  return c;
}

char *judge(circle flower, circle attempt) {
  char *result = malloc(sizeof(char) * 5);
  if(attempt.R >= flower.R) {
    if(attempt.R + attempt.X >= flower.R + flower.X) {
      if(attempt.R + attempt.Y >= flower.R + flower.Y) {
        return "RICO";
      }
    }
  }

  return "MORTO";
}

int main() {
  char *result;

  int comm_sz;
  int my_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  srand((unsigned) time(NULL) * my_rank);

  if(comm_sz % 2 != 0) {
    printf("Número de processos deve ser par!\n");
    MPI_Finalize();
    exit(0);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int blocklengths[3] = {1, 1, 1};
  MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
  MPI_Datatype mpi_circle_type;
  MPI_Aint offsets[3];

  offsets[0] = offsetof(circle, R);
  offsets[1] = offsetof(circle, X);
  offsets[2] = offsetof(circle, Y);

  MPI_Type_create_struct(3, blocklengths, offsets, types, &mpi_circle_type);
  MPI_Type_commit(&mpi_circle_type);

  if (my_rank % 2 == 0) {
    circle flower = generate_circle(10);
    MPI_Send(&flower, 1, mpi_circle_type, my_rank + 1, 0, MPI_COMM_WORLD);
  } else {
    circle flower;
    MPI_Recv(&flower, 1, mpi_circle_type, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    circle c = generate_circle(14);
    result = judge(flower, c);
    printf("Caçador %d tentou desenhar na flor %d e terminou %s\n", my_rank, my_rank - 1, result);
  }

  MPI_Finalize();

  return 0;
}
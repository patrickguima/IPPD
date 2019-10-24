#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */
#include <mpi.h>

int ehPrimo(int n,int rank,int numProc){
   int i, j, prime, t=0;
   printf("[");
   for(j = 1 + rank; j <= n; j = j + numProc){
   		prime = 1;
   for(i = 2; i < j; i++){
      if(j%i==0){
         prime = 0;
         break;
      }
   }
   if(prime){
   		printf("%i ",i);
   }
   t = t + prime;
}
printf("]\n");
return t;
}

void timestamp(void){
#define TIME_SIZE 40

	static char timeBuffer[TIME_SIZE];
	const struct tm *tm;
	time_t now;

	now = time(NULL);
	tm = localtime(&now);

	strftime(timeBuffer,TIME_SIZE,"%d %B %Y %M: %S %p",tm); //Format time as string

	printf("%s\n", timeBuffer);
	return;
	#undef TIME_SIZE
}


int main(int argc, char *argv[]){

   int num_processos,rank_proc;
   int num,n_lo = 1, n_hi = 262144, n_factor=2;

   if(argc >= 2){
        n_hi = atoi(argv[1]); 
   }else{
   	    n_hi = 262144;
   }

   MPI_Init(&argc,&argv);

   MPI_Comm_size(MPI_COMM_WORLD,&num_processos);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank_proc);
   
   if(rank_proc==0){
   	timestamp();
   }

    num = n_hi;

    ehPrimo(num,rank_proc,num_processos);
    MPI_Finalize();
    if(rank_proc==0){
    	timestamp();
    }
	return 0;
}
#include <stdio.h>
#include <omp.h>
void matrix_mult(double A[][2], double B[][2], double C[][2], int k){
	int i, j,v;

	#pragma offload target(mic) in(A:length(k*k)) in(B:length(k*k)) out(C:length(k*k))
	{
		#pragma omp parallel for default(shared) private(i, j, v)
		for(i = 0; i < k; i++)
			for(j = 0; j < k; j++){
				#pragma vector aligned
				#pragma simd
				for(v = 0; v < k; v++)
					C[i * k + j] += A[i * k + v] * B[v * k + j];
			}
	}
}
void printa(double A[], double B[], double C[], int k){
	int i,j;
	printf("A =\n[");
	for ( i = 0; i < k; i++)
	{
		for (j = 0; i < k; j++)
		{
			printf(" %i",A[i][j]);
		}
		printf("\n");
		
	}
	printf("]");
	printf("B = \n[");
	for ( i = 0; i < k; i++)
	{
		for (j = 0; i < k; j++)
		{
			printf(" %i",B[i][j]);
		}
		printf("\n");
		
	}
	printf("]");
	printf("C = \n[");
	for ( i = 0; i < k; i++)
	{
		for (j = 0; i < k; j++)
		{
			printf(" %i",C[i][j]);
		}
		printf("\n");
		
	}
	printf("]");	
}
int main(){
	int i,j,k=2;
	double LEITE[2][2], NOVO[2][2],SENA[2][2];
	for ( i = 0; i < k; i++)
	{
		for (j = 0; i < k; j++)
		{
			LEITE[i][j] = i+j;
			NOVO[i][j] = (i*j) +2;
			SENA[i][j] =0;
		}
		
	}
	//printa(A,B,C,k);
    printf("hahh");
	matrix_mult(LEITE,NOVO,SENA,k);
	printf("\n\n###################\n");
	//printa(A,B,C,k);
}
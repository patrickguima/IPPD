_kernel void vector_multiply(_global const int *A, _global const int *B, _global int *C)
{
	//Pega o indice do elemento atual
	int i = get_global_id(0);

	//Faz a multiplicação entre dois vetores
	C[i] = A[i] * B[i];
}
#include <stdio.h>
#include <omp.h>
long long int sum(int *v, long long int N){
long long int i, sum = 0;
for(i = 0; i < N; i++)
sum += v[i];
return sum
}
int main(){
    int N=10;
    int v[N];
    v[N] = [1,2,3,4,5,6,7,8,9,10];
int sum = 0,i;

#pragma omp parallel private(i, myid)
{
int myid = omp_get_thread_num();
#pragma omp single
{
int nthreads = omp_get_num_threads();
}
for(i = myid; i < N; i += nthreads)
// RACE CONDITION
sum += v[i]; // ler sum, v[i]; somar; escrever sum;
} 
}
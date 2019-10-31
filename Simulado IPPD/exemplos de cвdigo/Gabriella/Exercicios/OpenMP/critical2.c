
#include <stdio.h>
#include <omp.h>
long long int func(int *v, long long int N){
long long int i, sum = 0;
for(i = 0; i < N; i++)
    sum += v[i];
return sum;
}
int main(){
    int N=10;
    int v[10] = {1,2,3,4,5,6,7,8,9,10};
    int sum = 0,i,myid,init=0,end=0,slice=0,nthreads=0,sum_local;
    func(v,N);
    sum = 0;
    #pragma omp parallel private(i, sum_local, myid, init, end)
    {
        myid = omp_get_thread_num();
         sum_local = 0;
        #pragma omp single
        {
         nthreads = omp_get_num_threads();
         slice = N / nthreads;
        }
        init = myid * slice;
        if(myid == nthreads - 1)
            end = N;
        else 
            end = init + slice;
        for(i = init; i < end; i++)
            sum_local += v[i];
               printf("valor do sum_local %d\n",sum_local);
        #pragma omp atomic
        sum += sum_local;
        printf("valor do sum %d\n",sum);
    }
}
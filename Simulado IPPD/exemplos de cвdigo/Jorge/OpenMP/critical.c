
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
    int sum = 0,i;
    func(v,N);
    int myid,nthreads,sum_local=0;
    #pragma omp parallel private(i, myid)
    {
        myid = omp_get_thread_num();
        #pragma omp single
        {
        nthreads = omp_get_num_threads();
        }
    //for(i = myid; i < N; i += nthreads)
    //sum += v[i];
    //} 
        for(i = myid; i < N; i += nthreads)
            sum_local += v[i];
            printf("valor do sum_local %d\n",sum_local);
        #pragma omp atomic
        sum += sum_local;
        printf("valor do sum %d\n",sum);
    }
}
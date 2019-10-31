#include <stdio.h>
#include <omp.h>
int main() {
#pragma omp parallel
{
int ID = omp_get_thread_num();//retorna o numero de threads
printf(" hello(%d) ", ID);
printf(" world(%d) \n", ID);
}
}
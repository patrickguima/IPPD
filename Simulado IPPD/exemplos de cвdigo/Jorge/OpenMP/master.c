#include <stdio.h>
#include <omp.h>
int main() {
#pragma omp parallel
{
#pragma omp master
printf(" master - this is thread %d\n", omp_get_thread_num());
#pragma omp single
printf(" single - this is thread %d\n", omp_get_thread_num());
}
}
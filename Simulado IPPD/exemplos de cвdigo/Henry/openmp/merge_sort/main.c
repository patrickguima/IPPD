#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "merge_sort.h"
#include "../../utils/gen_random_ints.h"
#include "../../utils/cronometer.h"


int main () {
    int length = 10000;
    int *unsorted_arr = gen_random_ints(length, length);
    struct cronometer c = init_cronometer();
    
    printf("Running with %d threads\n", omp_get_max_threads());

    c = start(c);
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort(unsorted_arr, 0, length);
    }
    c = end(c);
    if(assert_asc_order(unsorted_arr, length))
        printf("Success!\n Sorted %d elements\n\n", length);
    printf("Time elapsed: %lf", diff(c));
    return 0;
}
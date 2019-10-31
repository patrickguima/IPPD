#include <stdlib.h>
#include "gen_random_ints.h"

int *gen_random_ints (unsigned int length, unsigned int max) {
    int *arr = malloc(sizeof(int) * length);
    for (int i=0; i<=length; ++i) {
        arr[i] = rand() % max;              
    }
    return arr;
}

int assert_asc_order(int arr[], int length) {
    for (int i=0; i < length - 1; ++i)
        if (arr[i] > arr[i+1])
            return 0;
    return 1;
}
#include <stdio.h>
#include <stdlib.h>

#include "merge_sort.h"

#define MIN_ARR_LEN 2


void bubble_sort(int [], unsigned int, unsigned int);


void print_int_arr(int arr[], unsigned int start, unsigned int end) {
    for (unsigned int i = start; i<=end; ++i)
        printf(" %d ->", arr[i]);
    printf(" |\n");
}


void merge (
        int array[], 
        unsigned int start, 
        unsigned int middle, 
        unsigned int  end ) {
    
    unsigned int start_ = middle + 1;

    if (array[middle] <= array[start_]) return;

    while (start <= middle && start_ <= end) {
        if (array[start] <= array[start_]) ++start;
        else {
            int value = array[start_];
            unsigned int index = start_;
            while (index != start) {
                array[index] = array[index-1];
                --index;
            }           
            array[start] = value;
            ++start;
            ++middle;
            ++start_; 
        }
    }
}


void merge_sort(
        int array[], 
        unsigned int start,
        unsigned int end ) {

    unsigned int length = end - start;
    unsigned int middle = start + (end - start) / 2;

    if (length <= MIN_ARR_LEN) return bubble_sort(array, start, end);
    
    #pragma omp task
    merge_sort(array, start, middle);
    
    #pragma omp task
    merge_sort(array, middle + 1, end);
    
    #pragma omp taskwait
    merge(array, start, middle, end);
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(int array[], unsigned int start, unsigned int end) {
    int swapped;
    for (int j=start; j<=end; ++j) {
        swapped = 0;
        for (int i=start; i<=end-1; ++i) {
            if (array[i] > array[i+1]) {
                swap(&array[i], &array[i+1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <omp.h>

#include "src/selection-sort.h"

int compare (const void * a, const void * b){
  return (* (int*) a - * (int*) b); /* Do not change it! */
}

int main(int argc, char **argv){
	char *awnser[] = { "bad", "ok" };
	int *vector, *test;
	int i, n, is_ok;
	double elapsed, start, end;

	if(argc != 2){
		fprintf(stderr, "Usage: %s <number of elements>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	n = atoi(argv[1]);

	printf("number of elements: %d\n", n);

	vector = (int *) malloc(n * sizeof(int));
	assert(vector != NULL);

	test = (int *) malloc(n * sizeof(int));
	assert(test != NULL);

	srand(time(NULL));

	for(i = 0; i < n; i++)
		vector[i] = rand();

	memcpy(test, vector, n * sizeof(int));

	qsort (test, n, sizeof(int), compare);

	start = omp_get_wtime();
	selection_sort(vector, n);
	end = omp_get_wtime();

	elapsed = end - start;

	is_ok = (memcmp(vector, test, n * sizeof(int)) == 0);

	printf("sorted values are %s\ntime: %.3f seconds\n", awnser[is_ok], elapsed);

	free(vector);
	free(test);

	return 0;
}
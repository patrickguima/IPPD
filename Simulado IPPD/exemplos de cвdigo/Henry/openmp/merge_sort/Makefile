CC=mpicc
CFLAGS= -Wall -g -O0 -fopenmp

RANDOM_INTS=../../utils/gen_random_ints.c ../../utils/gen_random_ints.h
CRONOMETER=../../utils/cronometer.c ../../utils/cronometer.h
UTILS=${RANDOM_INTS} ${CRONOMETER}

all: main

main: merge_sort.c merge_sort.h main.c
	${CC} ${CFLAGS} -o main main.c merge_sort.c merge_sort.h ${UTILS}
	
clean:
	rm -f main *.o *.out *.gch

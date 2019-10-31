NOTAS DE COMPILAÇÃO 

Linux e OS X com gcc or Intel icc:

gcc -fopenmp foo.c #GCC

icc -fopenmp foo.c #Intel ICC

export OMP_NUM_THREADS=4  bash

./a.out

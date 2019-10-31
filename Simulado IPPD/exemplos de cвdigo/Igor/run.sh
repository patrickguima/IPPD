cd cmake
cmake ..
make
mpirun ../bin/ippd ../bin/vector_add_kernel.cl
cd ..
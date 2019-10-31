#ifndef OPENCL_HPP
#define OPENCL_HPP

#define OPENCL_INIT(platform_id, device_id, context, command_queue, ret) \
    cl_context context; \
    cl_command_queue command_queue; \
    cl_platform_id platform_id = NULL; \
    cl_device_id device_id = NULL;    \
    cl_uint ret_num_devices; \
    cl_uint ret_num_platforms; \
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms); \
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, \
            &device_id, &ret_num_devices); \
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret); \
    command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

#define OPENMP_INIT /* omp_set_num_threads(4); */

#define MPI_INIT(world_size, rank) \
    MPI_Init(NULL, NULL); \
    int world_size; \
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); \
    int rank; \
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#define END(context, command_queue) \
    MPI_Barrier(MPI_COMM_WORLD); \
    MPI_Finalize(); \
    clFlush(command_queue); \
    clFinish(command_queue); \
    clReleaseCommandQueue(command_queue); \
    clReleaseContext(context);

#define INIT(world_size, rank, platform_id, device_id, context, command_queue, ret) \
    OPENCL_INIT(platform_id, device_id, context, command_queue, ret) \
    OPENMP_INIT \
    MPI_INIT(world_size, rank)

#endif // ! OPENCL_HPP
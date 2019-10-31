//g++ -std=c++0x -o matMult matMult.c -lOpenCL

/*
   Simple Matrix Multiplication
   C[i,j] = A[i,k] * B[k,j]
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)

int main(void) {
    // Create the two input matrix
    int i,j;
    const float wA = 25, wB = 25;
    const float hA = 25, hB = 25;
    const float wC = 25, hC = 25;

    float **A = (float**)malloc(hA*sizeof(float));
    for(i = 0; i < hA; i++) {
        A[i] = (float *)malloc(wA*sizeof(float));
        for(j = 0; j < wA; j++){
            A[i][j] = j;
        }
    }

    float **B = (float**)malloc(hB*sizeof(float));
    for(i = 0; i < hB; i++) {
        B[i] = (float *)malloc(wB*sizeof(float));
        for(j = 0; j < wB; j++){
            B[i][j] = j;
        }
    }
    
    float **C = (float**)malloc(hA*sizeof(float));
     for(i = 0; i < hA; i++) {
        C[i] = (float *)malloc(wB*sizeof(float));
    }

    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("simpleMultiply.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ALL, 1, 
            &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector 
    //We assume that A,B,C are float arrays which 
    //have been declared and initialized 
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            wA*hA * sizeof(float), NULL, &ret);

    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
             wB*hB * sizeof(float), NULL, &ret);

    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
             hA*wB * sizeof(float), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
             wA*hA * sizeof(float), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
             wB*hB * sizeof(float), B, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "simpleMultiply", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&c_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_int), (void *)&wA);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_int), (void *)&hA);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_int), (void *)&wB);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_int), (void *)&hB);
    ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&b_mem_obj);
    
    // Set local and global workgroup sizes
    //We assume the matrix dimensions are divisible by 16
    size_t localws[2] = {16,16};
    size_t globalws[2] = {wC,hC};
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, 
            globalws, localws, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    //int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
            wC*hC * sizeof(float), C, 0, NULL, NULL);

   //iterate over the rows of Matrix A
    for(int i = 0; i < hA; i++){
        //iterate over the colums of Matrix B
        for(int j = 0; j < wB; j++){
            C[i][j] = 0;
            ///Multiply and accumulate the values in the 
            //current row of A and column of B
            for(int k = 0; k < wA; k++){
                C[i][j] += A[i][k]*B[k][j];
                printf("%.2f = %.2f*%.2f\n", C[i][j],A[i][k],B[k][j]);

            }
        }
    }

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(A);
    free(B);
    free(C);
    return 0;
        
}

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#ifdef __APPLE__
#define RICH_BOY
#include <OpenCL/opencl.h>
#include <unistd.h>
#else
#include <CL/cl.h>
#endif

#include "err_code.h"

//pick up device type from compiler command line or from
//the default type
#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

//------------------------------------------------------------------------------
//
// kernel:mx_redc
// Purpose: Compute reduction of a given matrix
// input: in matrix input data of length count
// output: out matrix output data of length count/4
//
const char *KernelSource = "\n" \
                            "__kernel void mx_redc(                                                 \n" \
                            "   __global int* in,                                                   \n" \
                            "   __global int* out,                                                  \n" \
                            "   const unsigned int count)                                           \n" \
                            "{                                                                      \n" \
                            "   int id = get_global_id(0);                                          \n" \
                            "   int r_pos = id * 4;                                                 \n" \
                            "   if((r_pos+3) <= count) {                                            \n" \
                            "       out[id] = (in[r_pos] + in[r_pos+1] + in[r_pos+2] + in[r_pos+3])/4;               \n" \
                            "   } else {                                                            \n" \
                            "       out[0] = 999;                                                   \n" \
                            "   }                                                                   \n" \
                            "}                                                                      \n" \
                            "\n";

//------------------------------------------------------------------------------

void printMx(int *array, unsigned sz);

int main(int argc, char** argv)
{
  argc = argc;
  argv = argv;
  // INITIALIZATION
  // =========================================
  int err;
  unsigned m = 0;

  printf("\nEnter m:");
  scanf("%u", &m);

  unsigned SIZE = m * m;

  unsigned counter = 0;
  int * h_in = (int*) calloc(SIZE, sizeof(int));
  int * st_h_in = h_in;
  int * h_out = NULL;
  assert(h_in);
  for (unsigned i=0 ; i<SIZE ; ++i) {
      h_in[i] = ++counter; // initial values
  }
  printMx(h_in, m);

  // OpenCL INITIALIZATION
  // =========================================
  cl_device_id     device_id;     // compute device id
  cl_context       context;       // compute context
  cl_command_queue commands;      // compute command queue
  cl_program       program;       // compute program
  cl_kernel        ko_mx_redc;    // compute kernel

  cl_mem d_in;                    // device memory used for the input matrix
  cl_mem d_out;                   // device memory used for the output matrix


  // Set up platform and GPU device
  // =========================================

  cl_uint numPlatforms;
  // Find number of platforms
  err = clGetPlatformIDs(0, NULL, &numPlatforms);
  checkError(err, "Finding platforms");
  if (numPlatforms == 0)
  {
    printf("Found 0 platforms!\n");
    return EXIT_FAILURE;
  }

  // Get all platforms
  cl_platform_id *Platform = (cl_platform_id *)calloc(numPlatforms, sizeof(cl_platform_id));
  assert(Platform);
  err = clGetPlatformIDs(numPlatforms, Platform, NULL);
  checkError(err, "Getting platforms");

  // Secure a GPU
  for (unsigned i = 0; i < numPlatforms; i++)
  {
    err = clGetDeviceIDs(Platform[i], DEVICE, 1, &device_id, NULL);
    if (err == CL_SUCCESS)
      break;
  }

  if (device_id == NULL)
    checkError(err, "Finding a device");

  // Create a compute context
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  checkError(err, "Creating context");

  // Create a command queue
//  commands = clCreateCommandQueue(context, device_id, 0, &err);
  commands = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
  checkError(err, "Creating command queue");

  // Create the compute program from the source buffer
  program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
  checkError(err, "Creating program");

  // Build the program
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];

    printf("Error: Failed to build program executable!\n%s\n", err_code(err));
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
    return EXIT_FAILURE;
  }

  // Create the compute kernel from the program
  ko_mx_redc = clCreateKernel(program, "mx_redc", &err);
  checkError(err, "Creating kernel");

  unsigned in_sz = SIZE, in_dim = m;
  unsigned out_sz = SIZE/4, out_dim = m/2;

  do {
    d_in = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * SIZE, h_in, &err);
    checkError(err, "Creating buffer d_in");

    d_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, out_sz, NULL, &err);
    checkError(err, "Creating buffer d_out");

    // preparing reduction
    err  = clSetKernelArg(ko_mx_redc, 0, sizeof(cl_mem), &d_in);
    err |= clSetKernelArg(ko_mx_redc, 1, sizeof(cl_mem), &d_out);
    err |= clSetKernelArg(ko_mx_redc, 2, sizeof(unsigned int), &in_sz);
    checkError(err, "Setting kernel arguments");

    // executing reduction
    // creates one thread for each position in the output array
    err = clEnqueueNDRangeKernel(commands, ko_mx_redc, 1, NULL, (size_t *)&out_sz, NULL, 0, NULL, NULL);
    checkError(err, "Enqueueing kernel");

    // Wait for the commands to complete before checking
    printf("Wainting...");
    fflush(stdout);
    err = clFinish(commands);
    checkError(err, "Waiting for kernel to finish");

    // reserve space for results
    if ( h_out != NULL )
      free(h_out); // clean partial results
    h_out = (int *) calloc(out_sz, sizeof(int));

    // Read back the results from the compute device
    err = clEnqueueReadBuffer( commands, d_out, CL_TRUE, 0, sizeof(int) * out_sz, h_out, 0, NULL, NULL );
    if (err != CL_SUCCESS) {
      printf("Error: Failed to read output arrays!\n%s\n", err_code(err));
      exit(1);
    }

    printMx(h_out, out_dim);

    // Next step
    in_sz = out_sz;
    in_dim = out_dim;

    out_sz = out_sz/4;
    out_dim = out_dim/2;

    clReleaseMemObject(d_in);
    clReleaseMemObject(d_out);

    h_in = h_out;
  } while (in_sz >= 4);

  // summarise results
  // cleanup then shutdown
  clReleaseMemObject(d_in);
  clReleaseMemObject(d_out);
  clReleaseProgram(program);
  clReleaseKernel(ko_mx_redc);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);

  free(st_h_in);
  free(h_out);

  return 0;
}

void printMx(int *array, unsigned row_sz) {
  unsigned sz = row_sz * row_sz;

  for (unsigned i=0 ; i<sz ; ++i) {
    if ((i%row_sz) == 0)
      printf("\n");
    printf("M[%u] = %d   \t", i, array[i]);
    fflush(stdout);
  }
  fflush(stdout);
}



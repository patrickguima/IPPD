#ifndef DOC__HPP
#define DOC__HPP

/*

MPI
    // Send non blocking
        MPI_Send(
            void* data,
            int count,
            MPI_Datatype datatype,
            int destination,
            int tag,
            MPI_Comm communicator)
    // Send blocking
        MPI_Ssend(...)
    // Receive (Block)
        MPI_Recv(
            void* data,
            int count,
            MPI_Datatype datatype,
            int source,
            int tag,
            MPI_Comm communicator,
            MPI_Status* status)
    // Example
        if (world_rank == 0) {
            number = -1;
            MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        } else if (world_rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
            printf("Process 1 received number %d from process 0\n",
                number);
        }
    // Example Ring
        int token;
        if (world_rank != 0) {
            MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d received token %d from process %d\n",
                world_rank, token, world_rank - 1);
        } else {
            // Set the token's value if you are process 0
            token = -1;
        }
        MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size,
                0, MPI_COMM_WORLD);
        // Now process 0 can receive from the last process.
        if (world_rank == 0) {
            MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d received token %d from process %d\n",
                world_rank, token, world_size - 1);
        }
    // Example Ping Pong
        while (true)
        {
            if (!rank)
            {
                if (haveBall) 
                {
                    std::cout << "Ping\n";
                    haveBall = false;
                    // Pass the ball
                    MPI_Ssend(&haveBall, 1, MPI_CXX_BOOL, 1, 0, MPI_COMM_WORLD);
                }
                else 
                {
                    // Receive the ball
                    MPI_Recv(&haveBall, 1, MPI_CXX_BOOL, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    haveBall = true;
                    std::cout << "Pong\n";
                }
            }
            else
            {
                // Wait for the ball
                MPI_Recv(&haveBall, 1, MPI_CXX_BOOL, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // Pass the ball
                MPI_Ssend(&haveBall, 1, MPI_CXX_BOOL, 0, 0, MPI_COMM_WORLD);
            }
        }
    // Status
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
        int number_amount;
        if (world_rank == 0) {
            // Pick a random amount of integers to send to process one
            srand(time(NULL));
            number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

            // Send the amount of integers to process one
            MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("0 sent %d numbers to 1\n", number_amount);
        } else if (world_rank == 1) {
            MPI_Status status;
            // Receive at most MAX_NUMBERS from process zero
            MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD,
                    &status);

            // After receiving the message, check the status to determine
            // how many numbers were actually received
            MPI_Get_count(&status, MPI_INT, &number_amount);

            // Print off the amount of numbers, and also print additional
            // information in the status object
            printf("1 received %d numbers from 0. Message source = %d, "
                "tag = %d\n",
                number_amount, status.MPI_SOURCE, status.MPI_TAG);
        }
    // Get Count
        MPI_Get_count(
        MPI_Status* status,
        MPI_Datatype datatype,
        int* count)
    // Probe
        MPI_Probe(
        int source,
        int tag,
        MPI_Comm comm,
        MPI_Status* status)
        // Example
            int number_amount;
            if (world_rank == 0) {
                const int MAX_NUMBERS = 100;
                int numbers[MAX_NUMBERS];
                // Pick a random amount of integers to send to process one
                srand(time(NULL));
                number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

                // Send the random amount of integers to process one
                MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
                printf("0 sent %d numbers to 1\n", number_amount);
            } else if (world_rank == 1) {
                MPI_Status status;
                // Probe for an incoming message from process zero
                MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

                // When probe returns, the status object has the size and other
                // attributes of the incoming message. Get the message size
                MPI_Get_count(&status, MPI_INT, &number_amount);

                // Allocate a buffer to hold the incoming numbers
                int* number_buf = (int*)malloc(sizeof(int) * number_amount);

                // Now receive the message with the allocated buffer
                MPI_Recv(number_buf, number_amount, MPI_INT, 0, 0,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("1 dynamically received %d numbers from 0.\n",
                    number_amount);
                free(number_buf);
            }
    // Scatter
        MPI_Scatter(
        void* send_data,
        int send_count,
        MPI_Datatype send_datatype,
        void* recv_data,
        int recv_count,
        MPI_Datatype recv_datatype,
        int root,
        MPI_Comm communicator)
    // Gather
        MPI_Gather(
        void* send_data,
        int send_count,
        MPI_Datatype send_datatype,
        void* recv_data,
        int recv_count,
        MPI_Datatype recv_datatype,
        int root,
        MPI_Comm communicator)
    // Allgather
        MPI_Allgather(
        void* send_data,
        int send_count,
        MPI_Datatype send_datatype,
        void* recv_data,
        int recv_count,
        MPI_Datatype recv_datatype,
        MPI_Comm communicator)
    // Example Gather and Scatter - Averange
        if (world_rank == 0) {
        rand_nums = create_rand_nums(elements_per_proc * world_size);
        }
        // Create a buffer that will hold a subset of the random numbers
        float *sub_rand_nums = malloc(sizeof(float) * elements_per_proc);
        // Scatter the random numbers to all processes
        MPI_Scatter(rand_nums, elements_per_proc, MPI_FLOAT, sub_rand_nums,
                    elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
        // Compute the average of your subset
        float sub_avg = compute_avg(sub_rand_nums, elements_per_proc);
        // Gather all partial averages down to the root process
        float *sub_avgs = NULL;
        if (world_rank == 0) {
        sub_avgs = malloc(sizeof(float) * world_size);
        }
        MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0,
                MPI_COMM_WORLD);
        // Compute the total average of all numbers.
        if (world_rank == 0) {
        float avg = compute_avg(sub_avgs, world_size);
        }
    // Example of Allgather - Averange
        // Gather all partial averages down to all the processes
        float *sub_avgs = (float *)malloc(sizeof(float) * world_size);
        MPI_Allgather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT,
                    MPI_COMM_WORLD);

        // Compute the total average of all numbers.
        float avg = compute_avg(sub_avgs, world_size);
    // Barrier
        MPI_Barrier(MPI_Comm communicator)  
    // Broadcast
        MPI_Bcast(
        void* data,
        int count,
        MPI_Datatype datatype,
        int root,
        MPI_Comm communicator)
    // Reduce
        MPI_Reduce(
        void* send_data,
        void* recv_data,
        int count,
        MPI_Datatype datatype,
        MPI_Op op,
        int root,
        MPI_Comm communicator)
    // Example Reduce - Average 
        float *rand_nums = NULL;
        rand_nums = create_rand_nums(num_elements_per_proc);
        // Sum the numbers locally
        float local_sum = 0;
        int i;
        for (i = 0; i < num_elements_per_proc; i++) {
        local_sum += rand_nums[i];
        }
        // Print the random numbers on each process
        printf("Local sum for process %d - %f, avg = %f\n",
            world_rank, local_sum, local_sum / num_elements_per_proc);
        // Reduce all of the local sums into the global sum
        float global_sum;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
                MPI_COMM_WORLD);
        // Print the result
        if (world_rank == 0) {
        printf("Total sum = %f, avg = %f\n", global_sum,
                global_sum / (world_size * num_elements_per_proc));
        }


OpenMP
    // set X threads
        omp_set_num_threads(X); 
    // Get thread number
        omp_get_thread_num();
    // Get number of threads
        omp_get_num_threads();
    // Equaly distribute a task
        #pragma omp parallel
        #pragma omp parallel num_threads(X)
        // Private indicates that each thread has
        // its own copy of variable
            #pragma omp parallel private(data, id, total) num_threads(6)
        // Firt private inialize private variables
            #pragma omp parallel firstprivate(data, id, total) num_threads(6)
        // Different identifiers
            #pragma omp parallel shared(a,b,c,d,n,m) private(i,j)
        // With reduction
            #pragman omp parallel reduction(*: x)
            x += y * z;
    // Critical session
        #pragma omp critical
    // Parallel for loop
        #pragma omp parallel for num_threads(2) schedule(static) shared(vec,begin,middle,end)
        #pragma omp parallel for num_threads(2) schedule(static)
        // With reduction
            #pragma omp parallel for reduction(+:sum)
            for (i = 0; i < num_steps; ++i) {
                int x = (i+0.5) * step;
                sum += 4.0 / (1.0 + x*x);
            }
        // Without reduction, + - / * | & || && ^
            #pragma omp parallel
            {
                for (i = id; i < num_steps; i+=lnthreads) {
                    x = (i+0.5) * step;
                    sum += 4.0 / (1.0 + x*x);
                }
                #pragma omp atomic
                pi += sum * step;
            }
        // Schedulers
            // Default - Distribui igualmente as tarefas entre threads
                #pragma omp parallel for schedule(static) num_threads(THREADS)
            // Dynamic - Distribui tarefas para quem terminar
                #pragma omp parallel for schedule(dynamic) num_threads(THREADS)
            // Dynamic with chunk - Reduce overhead of dynamic of attaching a task
                #pragma omp parallel for schedule(dynamic) num_threads(THREADS)
            // Guided - In run time configure size of chunk in dynamic
                #pragma omp parallel for schedule(guided) num_threads(THREADS)
    // Sections
        #pragma omp parallel sections
        {
            #pragma omp section
            { 
                printf ("id = %d, \n", omp_get_thread_num());
            }

            #pragma omp section
            { 
                printf ("id = %d, \n", omp_get_thread_num());
            }
        }
    // Barrier  
        #pragma omp barrier
    // Atomic - critical arithmetic operations
        #pragma omp atomic

OpenCL
    // Create a buffer
        cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
                LIST_SIZE * sizeof(int), NULL, &ret);
    // Write main memory to GPUs memory
        ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
    
    // Kernel example
        __kernel void vector_add(__global const int *A, __global const int *B, __global int *C) {   
            // Get the index of the current element to be processed
            int i = get_global_id(0);
            // Do the operation
            C[i] = A[i] + B[i];
        }
*/

#endif // ! DOC__HPP
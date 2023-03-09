// MPI_test_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <mpi.h>

using namespace std;

#define N 5

#define SHOW 

//#define FIRST
//#define SECOND

#ifdef FIRST

int main()
{

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);


    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
        processor_name, world_rank, world_size);

    /*
     * N > size
     * N = size
     * N < size
     */
    ///*
    if (world_rank == 0)
    {
        int* a = new int [N * N];
        int* b = new int [N * N];
        int* c = new int [N * N];

        auto start_time = std::chrono::steady_clock::now();

        for (int i = 0; i < N * N; i++)
            a[i] = i;

        for (int i = 0; i < N * N; i++)
            b[i] = i;

#ifdef SHOW
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", a[i * N + j]);
            printf("\n");
        }
        printf("\n");

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", b[j * N + i]);
            printf("\n");
        }
        printf("\n");
#endif

        int rank = 1;
        for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (rank <= world_size)
                rank = 1;
            int _b = 1;
            MPI_Send(&_b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
            MPI_Send(a + (N * i), N, MPI_INT, rank, rank, MPI_COMM_WORLD);
            MPI_Send(b + (N * j), N, MPI_INT, rank, rank, MPI_COMM_WORLD);

            MPI_Status status;
            int temp = 0;
            MPI_Recv(&temp, 1, MPI_INT, rank, rank, MPI_COMM_WORLD, &status);
            c[i * N + j] = temp;
            rank++;
        }

        auto end_time = std::chrono::steady_clock::now();

#ifdef SHOW
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", c[j * N + i]);
            printf("\n");
        }
#endif

        auto elapsed_ns = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        printf("Time: %d \n", elapsed_ns.count());

        //for (int i = 0; i < N; i++)
        //  printf("%d", a[i]);

        rank = 1;
        for (int i = 1; i < world_size; i++)
        {
            int b = 0;
            MPI_Send(&b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
            rank++;
        }
        
    }
    else
    {
        int* a = new int [N];
        int* b = new int [N];
        int* c = new int [N];

        MPI_Status status;

        //MPI_Recv(a, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAGN MPI_COMM_WORLD, &status);
        int s = 0;
        while (true)
        {
            MPI_Recv(&s, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
            if (!s)
                break;
            MPI_Recv(a, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
            MPI_Recv(b, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

            int sum = 0;
            for (int i = 0; i < N; i++)
                sum += a[i] * b[i];

            MPI_Send(&sum, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD);

            /*
            printf("%s: ", processor_name);
            for (int i = 0; i < N; i++)
                printf("%d", a[i]);
            printf("\n");
            //*/
        }

    }
    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}

#elif SECOND


int main()
{

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);


    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
        processor_name, world_rank, world_size);

    /*
     * N > size
     * N = size
     * N < size
     */

    if (world_rank == 0)
    {
        int* a = new int [N * N];
        int* b = new int [N * N];
        int* c = new int [N * N];

        auto start_time = std::chrono::steady_clock::now();

        for (int i = 0; i < N * N; i++)
            a[i] = i;

        for (int i = 0; i < N * N; i++)
            b[i] = i;

#ifdef SHOW
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", a[i * N + j]);
            printf("\n");
        }
        printf("\n");

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", b[j * N + i]);
            printf("\n");
        }
        printf("\n");
#endif

        int rank = 1;
        for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (rank <= world_size)
                rank = 1;
            int _b = 1;
            MPI_Request req[4];
            MPI_Send(&_b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
            MPI_Isend(a + (N * i), N, MPI_INT, rank, rank, MPI_COMM_WORLD, &req[0]);
            MPI_Isend(b + (N * j), N, MPI_INT, rank, rank, MPI_COMM_WORLD, &req[1]);

            MPI_Waitall(2, req, MPI_STATUSES_IGNORE);

            MPI_Status status;
            int temp = 0;
            MPI_Recv(&temp, 1, MPI_INT, rank, rank, MPI_COMM_WORLD, &status);
            c[i * N + j] = temp;

            rank++;
        }

        auto end_time = std::chrono::steady_clock::now();

#ifdef SHOW
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", c[j * N + i]);
            printf("\n");
        }
#endif

        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        printf("Time: %d \n", elapsed_ns.count());

        //for (int i = 0; i < N; i++)
        //  printf("%d", a[i]);

        rank = 1;
        for (int i = 1; i < world_size; i++)
        {
            int b = 0;
            MPI_Send(&b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
            rank++;
        }
        
    }
    else
    {
        int* a = new int [N];
        int* b = new int [N];
        int* c = new int [N];

        MPI_Status status;

        //MPI_Recv(a, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAGN MPI_COMM_WORLD, &status);
        int s = 0;
        while (true)
        {
            MPI_Request req[2];
            MPI_Recv(&s, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
            if (!s)
                break;
            MPI_Irecv(a, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &req[0]);
            MPI_Irecv(b, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &req[1]);

            MPI_Waitall(2, req, MPI_STATUSES_IGNORE);

            int sum = 0;
            for (int i = 0; i < N; i++)
                sum += a[i] * b[i];

            MPI_Rsend(&sum, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD);


            /*
            printf("%s: ", processor_name);
            for (int i = 0; i < N; i++)
                printf("%d", a[i]);
            printf("\n");
            */
        }

    }

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}



#else
//THIRD


int main()
{

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);


    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
        processor_name, world_rank, world_size);

    /*
     * N > size
     * N = size
     * N < size
     */
    ///*
    if (world_rank == 0)
    {
        int* a = new int [N * N];
        int* b = new int [N * N];
        int* c = new int [N * N];

        auto start_time = std::chrono::steady_clock::now();

        for (int i = 0; i < N * N; i++)
            a[i] = i;

        for (int i = 0; i < N * N; i++)
            b[i] = i;

#ifdef SHOW
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", a[i * N + j]);
            printf("\n");
        }
        printf("\n");

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", b[j * N + i]);
            printf("\n");
        }
        printf("\n");
#endif

        int rank = 1;
        for (int i = 0; i < N; i++)
        {
            //MPI_Send(a + (N * i), N, MPI_INT, rank, rank, MPI_COMM_WORLD);
            MPI_Bcast(a + (N * i), N, MPI_INT, 0, MPI_COMM_WORLD);

            for (int j = 0; j < N; j++)
            {
                if (rank <= world_size)
                {
                    MPI_Bcast(a + (N * i), N, MPI_INT, 0, MPI_COMM_WORLD);
                    rank = 1;
                }
                int _b = 1;
                MPI_Send(&_b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
                MPI_Send(b + (N * j), N, MPI_INT, rank, rank, MPI_COMM_WORLD);

                MPI_Status status;
                int temp = 0;
                MPI_Recv(&temp, 1, MPI_INT, rank, rank, MPI_COMM_WORLD, &status);
                c[i * N + j] = temp;
                rank++;
            }
        }

        auto end_time = std::chrono::steady_clock::now();

#ifdef SHOW
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d \t", c[j * N + i]);
            printf("\n");
        }
#endif

        auto elapsed_ns = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        printf("Time: %d \n", elapsed_ns.count());

        //for (int i = 0; i < N; i++)
        //  printf("%d", a[i]);

        rank = 1;
        for (int i = 1; i < world_size; i++)
        {
            int b = 0;
            MPI_Send(&b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
            rank++;
        }
        
    }
    else
    {
        int* a = new int [N];
        int* b = new int [N];
        int* c = new int [N];

        MPI_Status status;

        //MPI_Recv(a, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAGN MPI_COMM_WORLD, &status);
        int s = 0;
        for (int j = 0; j < world_size - 1; j++)
        {
            MPI_Recv(a, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

            while (true)
            {

                MPI_Recv(&s, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
                if (!s)
                    break;
                MPI_Recv(b, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

                int sum = 0;
                for (int i = 0; i < N; i++)
                    sum += a[i] * b[i];

                MPI_Send(&sum, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD);

                /*
                printf("%s: ", processor_name);
                for (int i = 0; i < N; i++)
                    printf("%d", a[i]);
                printf("\n");
                //*/
            }
        }

    }
    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}

#endif


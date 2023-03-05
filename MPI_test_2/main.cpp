// MPI_test_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <mpi.h>

using namespace std;

#define N 26

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
        int* a = new int [N];
        int* b = new int [N * N];

        for (int i = 0; i < N; i++)
            a[i] = i;

        int rank = 1;
        for (int i = 0; i < N; i++)
        {
            if (rank <= world_size)
                rank = 1;
            int b = 1;
            MPI_Send(&b, 1, MPI_INT, rank, rank, MPI_COMM_WORLD);
            MPI_Send(a, N, MPI_INT, rank, rank, MPI_COMM_WORLD);
            rank++;
        }

            for (int i = 0; i < N; i++)
                printf("%d", a[i]);

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

        MPI_Status status;

        //MPI_Recv(a, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAGN MPI_COMM_WORLD, &status);
        int s = 0;
        while (true)
        {
            MPI_Recv(&s, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);
            if (!s)
                break;
            MPI_Recv(a, N, MPI_INT, 0, world_rank, MPI_COMM_WORLD, &status);

            printf("%s: ", processor_name);
            for (int i = 0; i < N; i++)
                printf("%d", a[i]);
            printf("\n");
        }

    }

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}

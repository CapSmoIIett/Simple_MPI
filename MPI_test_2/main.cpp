// MPI_test_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <mpi.h>

using namespace std;

#define SIZE_ROW 3000 
#define SIZE_COL 3000 

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

    if (world_rank == 0)
    {
        int* a = new int [SIZE_ROW * SIZE_COL];
        int* b = new int [SIZE_ROW * SIZE_COL];

        for (int i = 0; i < SIZE_ROW; i++)
            MPI_Send(a, SIZE_ROW, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD);
        
    }
    else
    {
        int* a = new int [SIZE_COL];
        int* b = new int [SIZE_COL];

        MPI_Recv(a, SIZE_ROW, MPI_INT, MPI_ANY_SOURCE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
        processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}

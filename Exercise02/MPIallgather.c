/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Allgather
*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Variables
    int rank, size;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int gathered_data[size];  // Holds gathered data from all processes

    // For each process, gather data (process ranks) from all processes
    MPI_Allgather(&rank, 1, MPI_INT, gathered_data, 1, MPI_INT, MPI_COMM_WORLD);

    // Display the gathered data for all processes by each process
    printf("Data Gathered by Process %d: via All-Gather\n[ ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", gathered_data[i]);
    }
    printf("]\n");

    MPI_Finalize();
    return 0;
}

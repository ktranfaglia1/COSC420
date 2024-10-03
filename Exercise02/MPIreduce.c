/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Reduce
*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // variables
    int rank, size, result;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Reduce the data (process ranks), meaning recieve data, then sum all data and send it to the master process
    MPI_Reduce(&rank, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Master Process
    if (rank == 0) {
        printf("Sum Reduced by Master Process: %d via Reduce\n", result);  // Display the sum of process ranks
    }

    MPI_Finalize();
    return 0;
}

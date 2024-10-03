/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Allreduce
*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Variables
    int rank, size, result;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Reduce the data (process ranks) for all, meaning collect data, then sum data and distribute the result to all processes
    MPI_Allreduce(&rank, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Sum Reduced by process %d: %d via All-Reduce\n", rank, result);  // For each process, display the sum of process ranks 

    MPI_Finalize();
    return 0;
}
/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Scan
*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Variables
    int rank, result;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Scan the data (process rank) to do a partial reduce, meaning do a partial sum on the data and send it to the corresponding process
    MPI_Scan(&rank, &result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Partial Sum Reduced by Process %d via Scan: %d\n", rank, result);  // For each process, display the partial sum of process ranks 

    MPI_Finalize();
    return 0;
}

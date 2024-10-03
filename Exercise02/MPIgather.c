/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Gather
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

    MPI_Gather(&rank, 1, MPI_INT, gathered_data, 1, MPI_INT, 0, MPI_COMM_WORLD);  // Gather all the sent process ranks in the Master process

    // Master Process
    if (rank == 0) {
        // Display the gathered data
        printf("Data Gathered by Master Process: via Gather\n[ ");
        for (int i = 0; i < size; i++) {
            printf("%d ", gathered_data[i]);
        }
        printf("]\n");
    }

    MPI_Finalize();
    return 0;
}

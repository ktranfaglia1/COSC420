/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Reduce_scatter
*/
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Variables
    int rank, size, recv_data;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[size];  // Holds data from all processes

    // Each process sets its data array
    for (int i = 0; i < size; i++) {
        data[i] = rank;
    }

    // Reduction followed by scatter
    int recv_counts[] = {1, 1, 1, 1};  // One item per process
    MPI_Reduce_scatter(data, &recv_data, recv_counts, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d received reduced scattered data = %d\n", rank, recv_data);

    MPI_Finalize();
    return 0;
}

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
    int recv_counts[size];  // Store an item per process

    // Each process sets a data array
    for (int i = 0; i < size; i++) {
        data[i] = rank * (i + 1);  // Store data for each process
        recv_counts[i] = 1;  // Initialize array
    }

    // Reduction followed by scatter, meaning recieve all data (process ranks), then sum the data and send to all processes
    MPI_Reduce_scatter(data, &recv_data, recv_counts, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("Process %d Received Data: %d via Reduced-Scatter\n", rank, recv_data);  // Display the scattered sum (reduction) of each process

    MPI_Finalize();
    return 0;
}

/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Alltoall
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

    int send_data[size], recv_data[size];  // Holds data from all processes

    // Generate unique data for each process to send to other processes
    for (int i = 0; i < size; i++) {
        send_data[i] = rank * i + 1;
    }

    MPI_Alltoall(send_data, 1, MPI_INT, recv_data, 1, MPI_INT, MPI_COMM_WORLD);  // Communicate all data to all processes

    // Display all data from all processes by each process
    printf("Data recieved by Process %d via All-to-All:\n[ ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", recv_data[i]);
    }
    printf("]\n");

    MPI_Finalize();
    return 0;
}

/* Kyle Tranfaglia
*  COSC420 - Exercise02
*  Last updated 10/02/24
*  This program is a basic demonstration of MPI_Bcast
*/
#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // Variables
    int rank;
    char message[6];  // Holds 5 character message + null terminator

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master Process
    if (rank == 0) {
        strcpy(message, "Hello");  // Set a message to be broadcasted
    }
    
    // Broadcast the 6 char message from master process to all processes
    MPI_Bcast(message, 6, MPI_CHAR, 0, MPI_COMM_WORLD); 

    printf("Process %d received message: %s via broadcast\n", rank, message);

    MPI_Finalize();
    return 0;
}
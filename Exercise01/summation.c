/* Kyle Tranfaglia
*  COSC420 - Exercise01
*  Last updated 09/25/24
*  This program takes in a command line argument (N) and does a summation from 1 to N in parallel using MPI
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);  // Initialize the MPI environment

     // Check that a command line argument was entered
    if (argc != 2) {
        printf("Must provide a command line argument N (positive integer).\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Convert command line argument to an integer and check value
    long n = atol(argv[1]);
    if (n < 1) {
        printf("Please enter a positive integer.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Variables
    int num_comm = 0, rank = 0;
    long long recieved_sum = 0, total_sum = 0, start = 0, stop = 0, range = 0;
    double start_time = 0, end_time = 0, elapsed_time = 0;
    MPI_Request request;
    MPI_Status status;

    // Get the number of processes & their ranks
    MPI_Comm_size(MPI_COMM_WORLD, &num_comm);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    start_time = MPI_Wtime();  // Start the timer

    // Note: The previous method used would occasionally result in an incorrect output; the following appears to handle the edge cases
    // Calculate the segment each process should handle
    range = n / num_comm;  // Basic range for each process (floor division)
    start = rank * range + 1;

    // Check if last process and give remaining numbers in range (prevents incorrect division and possible overlapping cases)
    if (rank == num_comm - 1) {
        stop = n;
    } 
    else {
        stop = (rank + 1) * range;
    }

    // Perform the partial summation for a process
    for (long i = start; i <= stop; i++) {
        total_sum += i;
    }

    // Check if master process
    if (rank == 0) {
        // Master process collects partial sums from other processes
        for (int i = 1; i < num_comm; i++) {
            MPI_Irecv(&recieved_sum, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);  // Wait for the non-blocking receive to complete
            total_sum += recieved_sum;  // Accumulate the received sum
        }

        // Stop the timer for the master process & calculate the total elapsed time
        end_time = MPI_Wtime();
        elapsed_time = end_time - start_time;

        // Print the total sum and elapsed time
        printf("Summation from 1 to %ld: %lld\n", n, total_sum);
        printf("Time taken: %f seconds\n", elapsed_time);
    } 
    else {
        // Worker processes send their partial sums to the master
        MPI_Isend(&total_sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);  // Wait for the send to complete
    }

    MPI_Finalize();  // Finalize the MPI environment

    return 0;
}
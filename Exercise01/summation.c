#include <stdio.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv) {

    // Check that a command line argument was entered
    if (argc != 2) {
        printf("Must provide a command line argument N (positive integer).\n");
        return 1;
    }

    // Convert command line argument to an integer
    long n = atol(argv[1]);
    if (n < 1) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    int my_rank, send_data, recieve_data;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Variables for timing and summation
    clock_t start, end;
    long sum = 0;

    start = clock();  // Start timing

    end = clock();  // End timing

    // Calculate the time taken in seconds
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    MPI_Finalize()

    // Output the result
    printf("Sum from 1 to %ld = %ld\n", n, sum);
    printf("Time to compute: %f seconds\n", cpu_time_used);
}
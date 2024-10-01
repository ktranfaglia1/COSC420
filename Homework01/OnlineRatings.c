/* Kyle Tranfaglia
*  COSC420 - Homework01
*  Last Updated: 09/30/24
*  This is an MPI program to compute the average ratings for each of the products and sort them in decending order given a m and n value in command line
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_M 55 // Maximum number of products <m>
#define MAX_N 1000000  // Maximum number of reviews per product <n>
#define MIN_M 1 // Minimum number of products <m>
#define MIN_N 5  // Minimum number of reviews per product <n>

int main(int argc, char **argv) {

    // Variables
    MPI_Status status;
    int rank, num_comm, m, n;
    double start_time, end_time, elapsed_time;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_comm);

    // Command-line input validation with master process only
    if (rank == 0) {
        // Check command-line arguments - provide descriptive program requirements upon invalid argument
        if (argc != 3) {
            printf("Usage: %s <m> <n>\n", argv[0]);
            printf("<m> is the number of products (%d-%d)\n", MIN_M, MAX_M);
            printf("<n> is the number of reviews per product (%d-%d)\n", MIN_N, MAX_N);
            MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to incorrect number of command-line arguments
        }

        // Store command line arguments as integers
        m = atoi(argv[1]);
        n = atoi(argv[2]);

        // Validate m and n
        if (m < 1 || m > MAX_M || n < MIN_N || n > 1000000) {
            printf("Error: m must be in range [%d,%d] and n must be in range [%d,%d]\n", MIN_M, MAX_M, MIN_N, MAX_N);
            MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to argument range error
        }

        // Check for sufficient number of processes
        if (num_comm < 2) {
            printf("Error: At least 2 MPI processes are required (1 master and at least 1 worker).\n");
            MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to insufficient MPI processes (1 master and at least 1 worker)
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);  // Ensure all processes wait for the master to finish checking command-line input before proceeding

    start_time = MPI_Wtime();  // Start timing

    // Check if master process
    if (rank == 0) {
        srand(time(NULL));

        // Allocate memory for n random reviews for all m products as a flattened array such that every n elements represents a product x from m
        int *all_reviews = (int *)malloc(m * n * sizeof(int));

        // Error check memory allocation for reviews array
        if (all_reviews == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for all_reviews.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

         // Generate and store n random review scores between 0 and 5 for all m products
        for (int i = 0; i < m * n; i++) {
            all_reviews[i] = rand() % 6;
        }

        // Send data to worker process(es)
        for (int i = 1; i <= m; i++) {
            MPI_Send(&all_reviews[(i - 1) * n], n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        // Allocate memory for m products and receive average ratings of n reviews from worker process(es)
        double *averages = (double *)malloc(m * sizeof(double));

        // Error check memory allocation for averages array
        if (averages == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for averages.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        for (int i = 1; i < num_comm; i++) {
            printf("Master %d waiting to receive average from worker %d\n", rank, i);
            MPI_Recv(&averages[i - 1], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
            printf("Master received average: %.2f from worker %d\n", averages[i - 1], i);
        }
        
        // Sort averages in descending order from highest to lowest and display
        printf("Sorted Product Ratings:\n");
        for (int i = 0; i < m; i++) {
            printf("Product %d: %.1f\n", i + 1, averages[i]);
        }

        // Stop the timer for the master process & calculate the total elapsed time
        end_time = MPI_Wtime();
        elapsed_time = end_time - start_time;

        // Print the total sum and elapsed time
        printf("Time taken: %4.1f seconds\n", elapsed_time);

        // Deallocate memory
        free(all_reviews);
        free(averages);
    }
    // Worker processes 
    else {
        // Allocate memory for n random reviews and receive the array of reviews from master
        int *product_reviews = (int *)malloc(n * sizeof(int));

        // Error check memory allocation for averages array
        if (product_reviews == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for product_reviews.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Worker %d ready to receive reviews from master\n", rank);
        MPI_Recv(product_reviews, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        // Compute the average review rating for the given product
        double average = 0.0;
        for (int i = 0; i < n; i++) {
            average += product_reviews[i];
        }
        average /= n;
        printf("Worker %d sending average: %.2f\n", rank, average);
        MPI_Send(&average, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);  // Send average review rating back to master

        free(product_reviews);  // Deallocate memory
    }

    MPI_Finalize();  // Finalize MPI

    return 0;
}
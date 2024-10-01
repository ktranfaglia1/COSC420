/* Kyle Tranfaglia
*  COSC420 - Homework01
*  Last Updated: 10/01/24
*  This is a Debugging version of an MPI program to compute the average ratings for each 
*  of the products and sort them in decending order given an m and n value in command line
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_M 55  // Maximum number of products <m>
#define MAX_N 1000000  // Maximum number of reviews per product <n>
#define MIN_M 1  // Minimum number of products <m>
#define MIN_N 5  // Minimum number of reviews per product <n>

// Comparison function for qsort (descending order)
int compare(const void *a, const void *b) {
    double arg1 = *(const double*)a;
    double arg2 = *(const double*)b;
    return (arg1 < arg2) - (arg1 > arg2);
}

int main(int argc, char **argv) {

    // Variables
    MPI_Status status;
    int rank, num_comm;
    double start_time, end_time, elapsed_time;

    // Store command line arguments as integers
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

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

        // Validate m and n
        if (m < 1 || m > MAX_M || n < MIN_N || n > 1000000) {
            printf("Error: m must be in range [%d,%d] and n must be in range [%d,%d]\n", MIN_M, MAX_M, MIN_N, MAX_N);
            MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to argument range error
        }

        // Check for sufficient number of processes
        if (num_comm < 2 || num_comm != (m + 1)) {
            printf("Error: At least 2 MPI processes are required and there must be m - 1 processes.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to insufficient MPI processes (1 master and at least 1 worker)
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);  // Ensure all processes wait for the master to finish checking command-line input before proceeding

    start_time = MPI_Wtime();  // Start timing

    // Check if master process
    if (rank == 0) {
        srand(time(NULL));

        // Allocate memory for n random reviews for all m products as a flattened array such that every n elements represents a product x from m
        int **all_reviews = (int **)malloc(m * sizeof(int*));

        // Generate and store n random review scores between 1 and 5 for all m products
        for (int i = 0; i < m; i++) {
            all_reviews[i] = (int *)malloc(n * sizeof(int));
            for (int j = 0; j < n; j++) {
                all_reviews[i][j] = (rand() % 5) + 1; 
            }
        }

        // Print reviews data being sent
        for (int i = 0; i < m; i++) {
            printf("Master prepared reviews for product %d: [ ", i + 1);
            for (int j = 0; j < n; j++) {
                printf("%d ", all_reviews[i][j]);
            }
            printf("]\n");
        }

        // Send data to worker process(es)
        for (int i = 1; i < num_comm; i++) {
            printf("Master %d sending reviews to worker %d\n", rank, i);
            MPI_Send(all_reviews[i-1], n, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Master sent reviews to worker %d\n", i);
        }
        
        // Allocate memory for m products and receive average ratings of n reviews from worker process(es)
        double *averages = (double *)malloc(m * sizeof(double));
        
        for (int i = 1; i < num_comm; i++) {
            printf("Master %d waiting to receive average from worker %d\n and I am you're father!", rank, i);
            MPI_Recv(&averages[i - 1], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
            printf("Master received average: %.2f from worker %d\n", averages[i - 1], i);
        }

        qsort(averages, m, sizeof(double), compare);  // Sort the averages in descending order using qsort (compare function is above)
        
        // Display sorted averages
        printf("Sorted Product Ratings:\n");
        for (int i = 0; i < m; i++) {
            printf("Product %d: %.1f\n", i + 1, averages[i]);
        }

        // Stop the timer for the master process & calculate the total elapsed time
        end_time = MPI_Wtime();
        elapsed_time = end_time - start_time;

        // Print the elapsed time
        printf("Time taken: %4.1f seconds\n", elapsed_time);

        // Deallocate memory
        for (int i = 0; i < m; i++) {
            free(all_reviews[i]);
        }
        free(all_reviews);
        free(averages);
    }
    // Worker processes 
    else if (rank <= m) {
        // Allocate memory for n random reviews and receive the array of reviews from master
        int *product_reviews = (int *)malloc(n * sizeof(int));
        printf("Worker %d ready to receive reviews from master\n", rank);
        MPI_Recv(product_reviews, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Worker %d received reviews from master\n", rank);

        // Display received data
        printf("[");
        for (int i = 0; i < n; i++) {
            printf("%d ", product_reviews[i]);
        }
        printf(" ]\n");

        // Compute the average review rating for the given product
        double average = 0.0;
        for (int i = 0; i < n; i++) {
            average += product_reviews[i];
        }
        average /= n;
        printf("Worker %d sending average: %.2f\n", rank, average);
        MPI_Send(&average, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);  // Send average review rating back to master
        printf("Worker %d sent average: %.2f\n", rank, average);
        free(product_reviews);  // Deallocate memory
    }

    MPI_Finalize();  // Finalize MPI

    return 0;
}
/* Kyle Tranfaglia
*  COSC420
*  Last Updated: 09/30/24
*  This is an MPI program to compute the average ratings for each of the products and sort them in decending order given a m and n value in command line
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_M 55 // Maximum number of products
#define MIN_N 5  // Minimum number of reviews per product

int main(int argc, char **argv) {
    int rank, num_comm;
    MPI_Status status;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_comm);

    // Check command line arguments
    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <m> <n>\n", argv[0]);
            printf("<m> is the number of products (1-%d)\n", MAX_M);
            printf("<n> is the number of reviews per product (%d-1000000)\n", MIN_N);
        }
        MPI_Finalize();
        return 1;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    // Validate m and n
    if (m < 1 || m > MAX_M || n < MIN_N || n > 1000000) {
        if (rank == 0) {
            printf("Error: m must be in range [1,%d] and n must be in range [%d,1000000]\n", MAX_M, MIN_N);
        }
        MPI_Finalize();
        return 1;
    }

    double start_time, end_time;
    start_time = MPI_Wtime(); // Start timing

    if (rank == 0) {
        // Master process
        srand(time(NULL));

        // Generate random reviews for each product
        int *scores = (int *)malloc(m * n * sizeof(int));
        for (int i = 0; i < m * n; i++) {
            scores[i] = rand() % 6; // Random score between 0 and 5
        }

        // Send data to worker processes
        for (int i = 1; i < num_comm; i++) {
            MPI_Send(&scores[(i-1) * n], n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Receive average ratings from worker processes
        double *averages = (double *)malloc(m * sizeof(double));
        for (int i = 1; i < num_comm; i++) {
            MPI_Recv(&averages[i-1], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
        }

        // Sort averages from highest to lowest and print
        printf("Product Ratings (from highest to lowest):\n");
        for (int i = 1; i < m; i++) {
            printf("Product %d: %.1f\n", i, averages[i-1]);
        }

        free(scores);
        free(averages);
    } else {
        // Worker processes
        int *product_reviews = (int *)malloc(n * sizeof(int));

        // Receive reviews array from master
        MPI_Recv(product_reviews, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        // Compute average rating
        double average = 0.0;
        for (int i = 0; i < n; i++) {
            average += product_reviews[i];
        }
        average /= n;

        // Send average back to master
        MPI_Send(&average, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);

        free(product_reviews);
    }

    end_time = MPI_Wtime(); // End timing

    // Print time taken by master process
    if (rank == 0) {
        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}

/* Kyle Tranfaglia
*  COSC420 - Homework01
*  Last Updated: 10/01/24
*  This is an MPI program to compute the average ratings for each of the products 
*  and sort them in decending order given an m and n value in command line
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX_M 55  // Maximum number of products <m>
#define MAX_N 1000000  // Maximum number of reviews per product <n>
#define MIN_M 1  // Minimum number of products <m>
#define MIN_N 5  // Minimum number of reviews per product <n>

// Struct to hold average rating and the corresponding product number/rank
typedef struct {
    double average;
    int product;
} ProductRating;

// Comparison function for qsort (descending order)
int compare(const void *a, const void *b) {
    double diff = ((ProductRating*)b)->average - ((ProductRating*)a)->average;  // Cast void pointers to product struct and get difference between product averages
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
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

    start_time = MPI_Wtime();  // Start timer

    // Check if master process
    if (rank == 0) {
        srand(time(NULL));

        // Allocate memory for m products, each to have n reviews
        int **all_reviews = (int **)malloc(m * sizeof(int*));

        // Generate and store n random review scores between 1 and 5 for all m products
        for (int i = 0; i < m; i++) {
            all_reviews[i] = (int *)malloc(n * sizeof(int));
            for (int j = 0; j < n; j++) {
                all_reviews[i][j] = (rand() % 5) + 1; 
            }
        }

        // Send data to worker process(es)
        for (int i = 1; i < num_comm; i++) {
            MPI_Send(all_reviews[i-1], n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        ProductRating *ratings = (ProductRating *)malloc(m * sizeof(ProductRating));  // Allocate memory for m products using product object

        // Receive average ratings of n reviews and n worker ranks from worker process(es)
        for (int i = 1; i < num_comm; i++) {
            double result[2];  // stores both average and rank from workers
            MPI_Recv(result, 2, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
            ratings[i - 1].average = result[0];  // Store average rating
            ratings[i - 1].product = (int)result[1];  // Store corresponding product number
        }

        qsort(ratings, m, sizeof(ProductRating), compare);  // Sort the averages in descending order using qsort (compare function is above)
        
        // Display sorted averages
        printf("Sorted Product Ratings:\n");
        for (int i = 0; i < m; i++) {
            printf("Product %d: %.1f\n", ratings[i].product, ratings[i].average);
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
        free(ratings);
    }
    // Worker processes 
    else if (rank <= m) {
        // Allocate memory for n random reviews and receive the array of reviews from master
        int *product_reviews = (int *)malloc(n * sizeof(int));
        MPI_Recv(product_reviews, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        // Compute the average review rating for the given product
        double average = 0.0;
        for (int i = 0; i < n; i++) {
            average += product_reviews[i];
        }
        average /= n;

        // Prepare to send both average and rank (product number) back to the master
        double result[2];
        result[0] = average;
        result[1] = (double)rank;

        MPI_Send(result, 2, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);  // Send average review rating back to master
        free(product_reviews);  // Deallocate memory
    }

    MPI_Finalize();  // Finalize MPI

    return 0;
}
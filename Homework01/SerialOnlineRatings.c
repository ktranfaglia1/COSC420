/* Kyle Tranfaglia
*  COSC420 - Homework01
*  Last Updated: 10/01/24
*  This is a serial program to compute the average ratings for each of the 
*  products and sort them in decending order given an m and n value in command line
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_M 55  // Maximum number of products
#define MAX_N 1000000  // Maximum number of reviews per product
#define MIN_M 1  // Minimum number of products
#define MIN_N 5  // Minimum number of reviews per product

// Comparison function for qsort (descending order)
int compare(const void *a, const void *b) {
    double arg1 = *(const double*)a;
    double arg2 = *(const double*)b;
    return (arg1 < arg2) - (arg1 > arg2);
}

int main(int argc, char **argv) {

    // Variables
    double start_time, end_time, elapsed_time;

    // Command-line input validation
    if (argc != 3) {
        printf("Usage: %s <m> <n>\n", argv[0]);
        printf("<m> is the number of products (%d-%d)\n", MIN_M, MAX_M);
        printf("<n> is the number of reviews per product (%d-%d)\n", MIN_N, MAX_N);
        return 1;
    }

    // Store command-line arguments as integers
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    // Validate command-line arguments m and n
    if (m < MIN_M || m > MAX_M || n < MIN_N || n > MAX_N) {
        printf("Error: m must be in range [%d,%d] and n must be in range [%d,%d]\n", MIN_M, MAX_M, MIN_N, MAX_N);
        return 1;
    }

    start_time = clock();  // Start timing

    srand(time(NULL));

    // Allocate memory for m products, each with n reviews
    int **all_reviews = (int **)malloc(m * sizeof(int *));
    double *averages = (double *)malloc(m * sizeof(double));

    // Generate n random reviews for each product and compute the average
    for (int i = 0; i < m; i++) {
        all_reviews[i] = (int *)malloc(n * sizeof(int));
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            all_reviews[i][j] = (rand() % 5) + 1; // Random review score between 1 and 5
            sum += all_reviews[i][j];
        }
        averages[i] = sum / n;
        // printf("Product %d average: %.2f\n", i + 1, averages[i]);
    }

    // Sort the averages in descending order using qsort
    qsort(averages, m, sizeof(double), compare);

    // Display sorted averages
    printf("\nSorted Product Ratings:\n");
    for (int i = 0; i < m; i++) {
        printf("Product %d: %.1f\n", i + 1, averages[i]);
    }

    // Stop the timer and calculate elapsed time
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Print the elapsed time
    printf("\nTime taken: %.4f seconds\n", elapsed_time);

    // Deallocate memory
    for (int i = 0; i < m; i++) {
        free(all_reviews[i]);
    }
    free(all_reviews);
    free(averages);

    return 0;
}
/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 1
*  Last updated 11/05/24
*  This program serially computes x × A = b matrix-vector multiplication
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // Check for correct number of command-line arguments
    if (argc != 2) {
        printf("Usage: %s <n> where n is an integer n > 0\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);  // Convert the second argument (n) to an integer
    // Input validate that n is greater than 0
    if (n <= 0) {
        printf("Invalid input. Must follow n >= 0\n");
        return 1;
    }

    srand(time(NULL));  // Seed the random number generator with the current time
    
    // Dynamically allocate memory for the vectors and matrix
    double *x = (double*)malloc(n * sizeof(double));
    double **A = (double**)malloc(n * sizeof(double*));
    // Allocate memory for each row of matrix A
    for (int i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
    }
    double *b = (double*)malloc(n * sizeof(double));

    // Initialization of the vector x with values in range [0, 1]
    for (int i = 0; i < n; i++) {
        x[i] = (double)rand() / RAND_MAX;
    }

    // Initialization of the matrix A with values in range [0, 1]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = (double)rand() / RAND_MAX;
        }
    }

    clock_t start = clock();  // Start measuring the time

    // Matrix-vector multiplication to compute b = x * A
    for (int i = 0; i < n; i++) {
        b[i] = 0.0;
        for (int j = 0; j < n; j++) {
            b[i] += x[j] * A[j][i];  // Matrix-vector multiplication
        }
    }

    clock_t end = clock();  // Stop measuring the time
    
    // Print the resulting vector b with 4 digits after the decimal point for each element
    printf("Vector b:\n[ ");
       for (int i = 0; i < n; i++) {
        printf("%.4f ", b[i]);
    }
    printf("]\nExecution time: %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);  // Print the execution time
    
    // Free dynamically allocated memory
    free(x);
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    
    return 0;
}
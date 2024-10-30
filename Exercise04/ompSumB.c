/* Kyle Tranfaglia
*  COSC420 - Exercise04 - Part 2
*  Last updated 10/29/24
*  This program uses directive #pragma omp parallel for and #pragma omp critical inside a for loop to compute summation
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    // Check for two command line arguments
    if (argc != 3) {
        printf("Usage: %s <N> <T>\n", argv[0]);
        printf("<N> is the number to sum (N >= 2000)");
        printf("<T> is the number of threads to use (T >= 1)");
        return 1;
    }

    // Store command-line arguments as integers
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);

    // Validate N and T
    if (N < 2000) {
        printf("Error: N should be 2000 or more.\n");
        return 1;
    }
    else if (T < 1) {
        printf("Error: T should be a positive integer.\n");
        return 1;
    }

    omp_set_num_threads(T);   // Set the number of threads

    // Variables
    unsigned long long sum = 0;
    double start_time, end_time;

    start_time = omp_get_wtime();  // Start the timer

    // Parallel summation with critical section
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        #pragma omp critical 
            sum += i;
    }

    end_time = omp_get_wtime();  // End the timer

    // Print the result and runtime
    printf("Summation result: %llu\n", sum);
    printf("Runtime: %.8f seconds\n", end_time - start_time);

    return 0;
}
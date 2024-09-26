/* Kyle Tranfaglia
*  COSC420 - Exercise01
*  Last updated 09/25/24
*  This program takes in a command line argument (N) and does a summation from 1 to N with a closed form formula
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    
    // Check that a command line argument was entered
    if (argc != 2) {
        printf("Must provide a command line argument N (positive integer).\n");
        return 1;
    }

    // Convert command line argument to an integer and check value
    long n = atol(argv[1]);
    if (n < 1) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    // Variables for timing and summation
    clock_t start, end;
    long long sum = 0;

    start = clock();  // Start timing

    // Use the closed-form formula to compute the sum
    sum = n * (n + 1) / 2;

    end = clock(); // End timing

    // Calculate the time taken in seconds
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Output the result
    printf("Sum from 1 to %ld = %lld\n", n, sum);
    printf("Time to compute: %f seconds\n", cpu_time_used);

    return 0;
}
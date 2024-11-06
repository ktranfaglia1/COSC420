/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp parallel for with some clauses
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Variables
    int size = 10, sum = 0;
    int a[size], b[size];

    // Initialize arrays
    for (int i = 0; i < size; i++) {
        a[i] = i;
        b[i] = i * size;
    }

    // Parallel for loop to sum array indexes with shared variables
    #pragma omp parallel for shared(a, b) reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += a[i] + b[i];
    }

    printf("Sum: %d\n", sum);
    return 0;
}
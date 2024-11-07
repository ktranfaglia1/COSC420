/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/06/24
*  This program showcases the directive #pragma omp for using all clauses
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Variables
    int size = 5, sum = 0, num1 = 0, num2 = 0, i;
    int a[size], b[size], c[size];

    // Initialize arrays a and b
    for (int i = 0; i < size; i++) {
        a[i] = i;
        b[i] = i * i;
    }

    // Parallel region with OpenMP for loop using all clauses
    #pragma omp parallel default(none) shared(a, b, c, sum, size, num2) private(i) firstprivate(num1)
    {
        #pragma omp for ordered lastprivate(num2) reduction(+:sum)
        for (i = 0; i < size; i++) {
            num1 = i;  // First private ensures each thread gets the initial value of num1
            sum += a[i] + b[i];  // Compute sum using reduction

            // Perform ordered operations
            #pragma omp ordered
            {
                // Sum a and b and store in array c
                c[i] = a[i] + b[i];
                printf("Thread %d | i: %d | c[i]: %d\n", omp_get_thread_num(), i, c[i]);
            }
            num2 = i;  // Lastprivate ensures num2 gets the value of i from the last loop iteration
        }

        // Print results within each thread to demonstrate private, firstprivate, lastprivate behavior
        #pragma omp critical
        {
            printf("Thread %d | First value: %d | Last value: %d\n", omp_get_thread_num(), num1, num2);
        }
    }

    // Display results
    printf("Sum: %d\n", sum);
    printf("Array c: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", c[i]);
    }

    return 0;
}
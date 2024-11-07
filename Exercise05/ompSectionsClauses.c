/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/06/24
*  This program showcases the directive #pragma omp sections using all clauses
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Variables
    int a = 10, b = 5, sum = 0, num1 = 1, num2 = 0;

    // Parallel region with OpenMP sections
    #pragma omp parallel default(none) shared(a, b, num2) firstprivate(num1) reduction(+:sum)
    {
        // Using sections to divide work among threads
        #pragma omp sections lastprivate(num2) nowait
        {
            // Section 1
            #pragma omp section
            {
                int local_sum = a + b + num1;
                printf("Section 1 | Thread: %d | Result: %d\n", omp_get_thread_num(), local_sum);
                sum += local_sum;
                num2 = 1;
            }

            // Section 2
            #pragma omp section
            {
                int local_diff = a - b + num1;
                printf("Section 2 | Thread: %d | Result: %d\n", omp_get_thread_num(), local_diff);
                sum += local_diff;
                num2 = 2;
            }

            // Section 3
            #pragma omp section
            {
                int local_prod = a * b + num1;
                printf("Section 3 | Thread: %d | Result: %d\n", omp_get_thread_num(), local_prod);
                sum += local_prod;
                num2 = 3;
            }

            // Section 4
            #pragma omp section
            {
                int local_div = (a / b) + num1;
                printf("Section 4 | Thread: %d | Result: %d\n", omp_get_thread_num(), local_div);
                sum += local_div;
                num2 = 4;
            }
        }
        // Critical section
        #pragma omp critical
        {
            printf("Thread %d | Last value of num2: %d\n", omp_get_thread_num(), num2);
        }
    }

    // Display the reduction result
    printf("Final sum: %d\n", sum);

    return 0;
}
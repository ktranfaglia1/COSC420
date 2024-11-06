/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp critical
*/
#include <stdio.h>
#include <omp.h>

int main() {
    int i = 0;

    // Start a parallel region with multiple threads and uses the 'critical' directive to ensure only one thread at a time in the critical section
    #pragma omp parallel
    {
        #pragma omp critical
        {
            printf("i: %d | Thread: %d\n", ++i, omp_get_thread_num());
        }
    }

    return 0;
}
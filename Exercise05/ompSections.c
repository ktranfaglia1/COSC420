/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp parallel sections and #pragma omp section
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Sspecify that different sections of code should be executed in parallel by different threads
    #pragma omp parallel sections
    {
        // To be executed by one of the available threads
        #pragma omp section
        {
            printf("Section: 1 | Thread: %d\n", omp_get_thread_num());
        }
        // To be executed by one of the other available threads
        #pragma omp section
        {
            printf("Section: 2 | Thread: %d\n", omp_get_thread_num());
        }
        // To be executed by another different available thread
        #pragma omp section
        {
            printf("Section: 3 | Thread: %d\n", omp_get_thread_num());
        }
    }

    return 0;
}
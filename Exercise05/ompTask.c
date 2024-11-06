/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp task and #pragma omp single
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Create a parallel region
    #pragma omp parallel
    {
        // Only one thread will execute the section
        #pragma omp single
        {
            // Single thread executes
            for (int i = 0; i < 10; i++) {
                // Creates a task for each iteration of the loop and it will be executed by threads in the parallel region
                #pragma omp task
                printf("Task: %d | Thread: %d\n", i, omp_get_thread_num());
            }
        }
    }

    return 0;
}
/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp single nowait
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Create a parallel region
    #pragma omp parallel
    {
        // Only one thread will execute the section and no synchronization is required after this section because of the "nowait" clause
        #pragma omp single nowait
        {
            printf("Single section | Thread: %d\n", omp_get_thread_num());
        }

        printf("Post single section | Thread: %d\n", omp_get_thread_num());
    }

    return 0;
}
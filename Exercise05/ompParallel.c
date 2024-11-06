/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp parallel
*/
#include <stdio.h>
#include <omp.h>

int main() {
    // Create multiple threads
    #pragma omp parallel
    {
        printf("Thread: %d\n", omp_get_thread_num());
    }
    return 0;
}
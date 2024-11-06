/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp parallel for collapse(x)
*/
#include <stdio.h>
#include <omp.h>

int main() {

    // Create a parallel region and distribute the iterations of the two nested loops
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            printf("Thread: %d | Iteration: [%d, %d]\n", omp_get_thread_num(), i, j);
        }
    }

    return 0;
}
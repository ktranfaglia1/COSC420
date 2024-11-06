/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp parallel for schedule(static, x)
*/
#include <stdio.h>
#include <omp.h>

int main() {

    // Parallelize the for loop such that iterations will be divided into chunks of size 2 and each thread will get a chunk of consecutive iterations.
    #pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < 10; i++) {
        printf("Thread: %d | Iteration %d\n", omp_get_thread_num(), i);
    }
    
    return 0;
}
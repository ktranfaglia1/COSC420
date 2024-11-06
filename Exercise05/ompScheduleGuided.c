/* Kyle Tranfaglia
*  COSC420 - Exercise05 - Part 2
*  Last updated 11/05/24
*  This program showcases the directive #pragma omp parallel for schedule(guided, x)
*/
#include <stdio.h>
#include <omp.h>

int main() {

    // Parallelize the for loop such that iterations will be assigned to threads in a "guided" fashion, with chunks of size 2
    // Initially, each thread is given a larger chunk, and as the threads finish their work, the size of the chunks decreases
    #pragma omp parallel for schedule(guided, 2)
    for (int i = 0; i < 10; i++) {
        printf("Thread: %d | Iteration %d\n", omp_get_thread_num(), i);
    }
    
    return 0;
}
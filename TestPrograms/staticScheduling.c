#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    #pragma omp parallel for schedule(static,1)
    for (int i = 0; i < 10; i++) {
        int id = omp_get_thread_num();
        printf("Thread: %d | Value: %d\n", id, i);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 999

int main() {
    int a[N], b[N], c[N];
    
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    omp_set_num_threads(5);

    double start_time = omp_get_wtime();
    
    #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            c[i] = b[i] + a[i];
        }

    double end_time = omp_get_wtime();

    // for (int i = 0; i < N; i++) {
    //     printf("c[%d] = %d\n", i, c[i]);
    // }

    printf("\nRuntime: %.8f seconds\n", end_time - start_time);

    return 0;
}

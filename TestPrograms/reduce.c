#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int my_rank, num_procs;
    int a = 1, c = 2;
    int b = 0, d = 0;  // b and d will store the reduction results on P0

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Ensure we have at least 3 processes for this example
    if (num_procs < 3) {
        if (my_rank == 0) {
            printf("This program requires at least 3 processes.\n");
        }
        MPI_Finalize();
        return -1;
    }

    // Step 1: Perform the first MPI_Reduce at time 1
    if (my_rank == 0 || my_rank == 2) {
        // P0 and P2 reduce a -> b
        MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    } else if (my_rank == 1) {
        // P1 reduces c -> d
        MPI_Reduce(&c, &d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    // Step 2: Perform the second MPI_Reduce at time 2
    if (my_rank == 0 || my_rank == 2) {
        // P0 and P2 reduce c -> d
        MPI_Reduce(&c, &d, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    } else if (my_rank == 1) {
        // P1 reduces a -> b
        MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    // Only P0 prints the results
    if (my_rank == 0) {
        printf("Final value of b (after reducing a): %d\n", b);
        printf("Final value of d (after reducing c): %d\n", d);
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}

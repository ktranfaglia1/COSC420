/* Kyle Tranfaglia
*  COSC420 - Homework02
*  Last Updated: 10/22/24
*  This is an MPI program to compute the approximate area under a curve using trapezoidal rule
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

long double f(long double x) {
    return x * x;
}

long double trapezoidal_rule(long double a, long double b, int n, long double delta_x) {
    long double estimate = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < n; i++) {
        long double x = a + i * delta_x;
        estimate += f(x);
    }
    estimate *= delta_x;
    return estimate;
}

int main(int argc, char** argv) {
    int rank, size, n;
    long double a, b, delta_x, local_a, local_b, local_integral, total_integral;
    double start_time, end_time, local_time, min_time, max_time, avg_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0 && argc != 4) {
        printf("Usage: %s <a> <b> <n>\n", argv[0]);
        printf("<a> is the integral lower bound\n");
        printf("<b> is the integral upper bound\n");
        printf("<n> is the number of subintervals for partitioning the approximation\n");
        MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to incorrect number of command-line arguments
    }
    MPI_Barrier(MPI_COMM_WORLD);

    a = strtold(argv[1], NULL);
    b = strtold(argv[2], NULL);
    n = atoi(argv[3]);

    // Compute step size (delta_x) and local intervals
    delta_x = (b - a) / n;
    int local_n = n / size;  // Number of trapezoids per process
    local_a = a + rank * local_n * delta_x;
    local_b = local_a + local_n * delta_x;

    // Measure local computation time
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    local_integral = trapezoidal_rule(local_a, local_b, local_n, delta_x);
    end_time = MPI_Wtime();
    local_time = end_time - start_time;

    // Gather integrals to the master process
    if (rank == 0) {
        total_integral = local_integral;
        for (int i = 1; i < size; i++) {
            long double temp;
            MPI_Recv(&temp, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += temp;
        }
    } else {
        MPI_Send(&local_integral, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    // Gather runtimes for statistic
    MPI_Reduce(&local_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    avg_time /= size;

    // Print results on the master process
    if (rank == 0) {
        printf("With n = %d trapezoids, our estimate of the integral from %.2Lf to %.2Lf = %.10Le\n", n, a, b, total_integral);
        printf("Among %d processes, the runtime statistics are:\n", size);
        printf("Min: %.8fs Max: %.8fs Avg: %.8fs\n", min_time, max_time, avg_time);
    }

    MPI_Finalize();
    return 0;
}

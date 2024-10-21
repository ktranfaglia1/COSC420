/* Kyle Tranfaglia
*  COSC420 - Homework02
*  Last Updated: 10/21/24
*  This is an MPI program to compute the approximate area under a curve using trapezoidal rule
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

// Evaluate the integral function f(x) = x^2
long double f(long double x) {
    return x * x;
}

// Apply the trapezoidal rule to approximate the integral f(x) over the range [a, b]
long double trapezoidal_rule(long double a, long double b, int n, long double delta_x) {
    // Initial estimate using the values of f(a) and f(b), then divide by two as the endpoints contribute half the weight
    long double estimate = (f(a) + f(b)) / 2.0;

    // Loop through interior points of the trapezoidal subinterval and sum up the function values f(x) at each point
    for (int i = 1; i < n; i++) {
        long double x = (i * delta_x) + a;  // Calculate the x (coordinate) for the ith trapezoid
        estimate += f(x);  // Update the estimate but adding the function value at x
    }

    return estimate * delta_x;  // Scale by the width of each trapezoid to get the total estimated (sub) integral
}

int main(int argc, char** argv) {
    // Variables
    int rank, size, n, subinterval_n;
    long double a, b, delta_x, subinterval_a, subinterval_b, subinterval_width, subinterval_integral, total_integral;
    double start_time, end_time, subinterval_time, min_time, max_time, sum_time, total_start_time, total_end_time, total_elapsed_time;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check number of command-line arguments with master process only - provide descriptive program requirements upon invalid argument
    if (rank == 0) {
        if (argc != 4) {
            printf("Usage: %s <a> <b> <n>\n", argv[0]);
            printf("<a> is the integral lower bound\n");
            printf("<b> is the integral upper bound\n");
            printf("<n> is the number of subintervals for partitioning the approximation\n");
            MPI_Abort(MPI_COMM_WORLD, 1);  // Abort all processes due to incorrect number of command-line arguments
        }
        else {
            total_start_time = MPI_Wtime();  // Start the total runtime timer
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);  // Ensure all processes wait for the master to finish checking command-line input before proceeding

    // Store command line arguments as long doubles (a, b) and an integer (n)
    a = strtold(argv[1], NULL);
    b = strtold(argv[2], NULL);
    n = atoi(argv[3]);

    // Computations for the integral subinterval
    delta_x = (b - a) / n;  // Step size | width of each trapezoid 
    subinterval_n = n / size;  // Number of trapezoids per process
    subinterval_width = subinterval_n * delta_x;  // Shift distance | total width of the subinterval
    subinterval_a = a + rank * subinterval_width;  // Starting point of the subinterval (shifted by rank)
    subinterval_b = subinterval_a + subinterval_width;  // Ending point of the subinterval (starting point plus shift distance)

    // Measure local computation time
    MPI_Barrier(MPI_COMM_WORLD);  // Synchronize all processes before starting the timer
    start_time = MPI_Wtime();  // Start the timer for the process to measure execution time
    subinterval_integral = trapezoidal_rule(subinterval_a, subinterval_b, subinterval_n, delta_x);  // Estimate the integral subinterval using trapezoidal rule
    end_time = MPI_Wtime();  // Stop the timer for the process after the calculation
    subinterval_time = end_time - start_time;  // Calculate the elapsed time in seconds

    // In the master process, recieve all integrals from other processes
    if (rank == 0) {
        total_integral = subinterval_integral;  // Add master process subinterval result to total

        // Communicate (recieve) from all other processes
        for (int i = 1; i < size; i++) {
            long double temp;
            MPI_Recv(&temp, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Receive the integral from process | MPI_STATUS_IGNORE reduces overhead (status not needed)
            total_integral += temp;  // Accumulate the estimated integral
        }
    } 
    else {
        MPI_Send(&subinterval_integral, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);  // If not the master process, send subinterval result to the master process
    }

    // Gather runtimes for statistic (min, max, and sum times) | reduce the subinterval_time from each process to master
    MPI_Reduce(&subinterval_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&subinterval_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&subinterval_time, &sum_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
 
    // calculate total runtime and print results on the master process
    if (rank == 0) {
        total_end_time = MPI_Wtime();  // Stop the total runtime timer
        total_elapsed_time = total_end_time - total_start_time;  // Calculate total elapsed time

        printf("With n = %d trapezoids, our estimate of the integral from %.2Lf to %.2Lf = %.10Le\n", n, a, b, total_integral);
        printf("Among %d processes, the runtime statistics are:\n", size); 
        printf("Min: %.8fs Max: %.8fs Avg: %.8fs\n", min_time, max_time, sum_time / size);  // Average runtime calculated by MPI_Sum result / number of processes
        printf("Total runtime: %.8f seconds\n", total_elapsed_time);
    }

    MPI_Finalize();  // Finalize MPI
    return 0;
}

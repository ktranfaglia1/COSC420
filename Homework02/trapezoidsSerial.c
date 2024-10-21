/* Kyle Tranfaglia
*  COSC420 - Homework02
*  Last Updated: 10/21/24
*  This is a serial program to compute the approximate area under a curve using trapezoidal rule
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Evaluate the integral function f(x) = x^2
long double f(long double x) {
    return x * x;
}

// Apply the trapezoidal rule to approximate the integral f(x) over the range [a, b]
long double trapezoidal_rule(long double a, long double b, int n, long double delta_x) {
    // Initial estimate using the values of f(a) and f(b), then divide by two as the endpoints contribute half the weight
    long double estimate = (f(a) + f(b)) / 2.0;

    // Loop through interior points and sum up the function values f(x) at each point
    for (int i = 1; i < n; i++) {
        long double x = a + i * delta_x;  // Calculate the x (coordinate) for the ith trapezoid
        estimate += f(x);  // Update the estimate by adding the function value at x
    }

    return estimate * delta_x;  // Scale by the width of each trapezoid to get the total estimated integral
}

int main(int argc, char** argv) {
    // Variables
    int n;
    long double a, b, delta_x, total_integral;
    clock_t start_time, end_time;
    double elapsed_time;

    // Check command-line arguments
    if (argc != 4) {
        printf("Usage: %s <a> <b> <n>\n", argv[0]);
        printf("<a> is the integral lower bound\n");
        printf("<b> is the integral upper bound\n");
        printf("<n> is the number of subintervals for partitioning the approximation\n");
        return 1;  // Exit due to incorrect number of command-line arguments
    }

    start_time = clock();  // Start timer before calculation

    // Store command line arguments as long doubles (a, b) and an integer (n)
    a = strtold(argv[1], NULL);
    b = strtold(argv[2], NULL);
    n = atoi(argv[3]);

    delta_x = (b - a) / n;  // Calculate step size | width of each trapezoid
    total_integral = trapezoidal_rule(a, b, n, delta_x);  // Estimate the integral subinterval using trapezoidal rule

    end_time = clock();  // Stop timer after calculation
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // Compute elapsed time in seconds

    // Print the result
    printf("With n = %d trapezoids, our estimate of the integral from %.2Lf to %.2Lf = %.10Le\n", n, a, b, total_integral);
    printf("Total runtime: %.8f seconds\n", elapsed_time);

    return 0;
}

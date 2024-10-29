/* Kyle Tranfaglia
*  COSC420 - Exercise04 - Part 1
*  Last updated 10/29/24
*  This program uses directive #pragma omp parallel for, then uses a loop to compute a summation
*/
// write an openmp program using c that takes two command line arguments: N, T. where N is the number to sum over (1 to N) and T is the number of threads. N should only accept integers 2000 or more. Use omp_get_wtime to measure runtime for the program. The program should use the directive #pragma omp parallel for, then use a for loop to do the summation. The summation value and the runtime should print to the console.
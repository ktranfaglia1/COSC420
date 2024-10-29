/* Kyle Tranfaglia
*  COSC420 - Exercise04 - Part 3
*  Last updated 10/29/24
*  This program uses directive #pragma omp parallel for reduction(+:sum), then uses a loop to compute a summation
*/
// Finally, lets make a third program uses the directive #pragma omp parallel for reduction(+:sum) but without the directive #pragma omp critical
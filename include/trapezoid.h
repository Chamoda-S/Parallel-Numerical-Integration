#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Evaluate one of several built-in test functions
 * func_id: 0 -> sin(x)
 *          1 -> cos(x)
 *          2 -> exp(x)
 *          3 -> x*x
 */
double func_eval(int func_id, double x);

/* Serial trapezoidal integration of selected function on [a,b] with n subintervals */
double trapezoid_serial(double a, double b, long n, int func_id);

/* OpenMP-parallel trapezoidal integration (requires -fopenmp) */
double trapezoid_openmp(double a, double b, long n, int func_id);

/* Portable wall-clock time (uses omp_get_wtime when available) */
double wtime(void);

#ifdef __cplusplus
}
#endif

#endif /* TRAPEZOID_H */

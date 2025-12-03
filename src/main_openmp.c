#include<stdio.h>
#include<stdlib.h>
#ifdef _OPENMP
#include<omp.h>
#endif
#include "../include/trapezoid.h"

int main(int argc, char **argv) {
    double a = 0.0, b = 1.0;
    long n = 1000000;
    int func_id = 0;
    int threads = 0;
    if (argc > 1) a = atof(argv[1]);
    if (argc > 2) b = atof(argv[2]);
    if (argc > 3) n = atol(argv[3]);
    if (argc > 4) func_id = atoi(argv[4]);
    if (argc > 5) threads = atoi(argv[5]);

#ifdef _OPENMP
    if (threads > 0) omp_set_num_threads(threads);
    int used = omp_get_max_threads();
#else
    int used = 1;
#endif

    double t0 = wtime();
    double result = trapezoid_openmp(a, b, n, func_id);
    double t1 = wtime();

    printf("OpenMP Trapezoid: func=%d [%.6f, %.6f] n=%ld threads=%d -> result=%.12f time=%.6f s\n",
           func_id, a, b, n, used, result, t1 - t0);
    return 0;
}

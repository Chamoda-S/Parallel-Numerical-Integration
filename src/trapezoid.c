#include "../include/trapezoid.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

double func_eval(int func_id, double x) {
    switch (func_id) {
        case 0: return sin(x);
        case 1: return cos(x);
        case 2: return exp(x);
        case 3: return x * x;
        default: return sin(x);
    }
}

double trapezoid_serial(double a, double b, long n, int func_id) {
    if (n <= 0) return 0.0;
    double h = (b - a) / (double)n;
    double sum = 0.5 * (func_eval(func_id, a) + func_eval(func_id, b));
    for (long i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += func_eval(func_id, x);
    }
    return sum * h;
}

double trapezoid_openmp(double a, double b, long n, int func_id) {
    if (n <= 0) return 0.0;
    double h = (b - a) / (double)n;
    double sum = 0.0;
#ifdef _OPENMP
#pragma omp parallel
    {
        double local_sum = 0.0;
#pragma omp for nowait
        for (long i = 1; i < n; ++i) {
            double x = a + i * h;
            local_sum += func_eval(func_id, x);
        }
#pragma omp atomic
        sum += local_sum;
    }
#else
    for (long i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += func_eval(func_id, x);
    }
#endif
    sum += 0.5 * (func_eval(func_id, a) + func_eval(func_id, b));
    return sum * h;
}

double wtime(void) {
#ifdef _OPENMP
    return omp_get_wtime();
#else
    return (double)clock() / (double)CLOCKS_PER_SEC;
#endif
}

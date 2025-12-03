#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include"../include/trapezoid.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 1.0;
    long n = 1000000;
    int func_id = 0;
    if (argc > 1) a = atof(argv[1]);
    if (argc > 2) b = atof(argv[2]);
    if (argc > 3) n = atol(argv[3]);
    if (argc > 4) func_id = atoi(argv[4]);

    double h = (b - a) / (double)n;
    long base = n / size;
    int rem = n % size;
    long local_n = base + (rank < rem ? 1 : 0);
    long start_index = rank * base + (rank < rem ? rank : rem);
    double local_a = a + start_index * h;
    double local_b = local_a + local_n * h;

    double t0 = MPI_Wtime();
    double local_result = trapezoid_serial(local_a, local_b, local_n, func_id);
    double total = 0.0;
    MPI_Reduce(&local_result, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    if (rank == 0) {
        printf("MPI Trapezoid: func=%d [%.6f, %.6f] n=%ld ranks=%d -> result=%.12f time=%.6f s\n",
               func_id, a, b, n, size, total, t1 - t0);
    }

    MPI_Finalize();
    return 0;
}

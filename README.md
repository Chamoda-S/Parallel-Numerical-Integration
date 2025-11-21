# Parallel Numerical Integration — Trapezoidal Rule

This project demonstrates serial, OpenMP and MPI implementations of the trapezoidal rule for numerical integration.

Build (PowerShell):

```powershell
# Build all targets
make
# or build specific target
make serial
make openmp
make mpi
```

Run examples:

Serial:

```powershell
.
bin/serial 0 1 1000000 0
# args: a b n func_id
```

OpenMP:

```powershell
# Optionally set threads via last arg or OMP_NUM_THREADS
.
bin/openmp 0 1 10000000 0 8
```

MPI (example with mpirun):

```powershell
mpirun -np 4 bin/mpi 0 1 10000000 0
```

Functions (func_id):

- 0: sin(x)
- 1: cos(x)
- 2: exp(x)
- 3: x*x

Notes:

- `trapezoid_serial` and `trapezoid_openmp` are in `src/trapezoid.c` and exposed via `include/trapezoid.h`.
- The OpenMP build requires a compiler with OpenMP support (GCC/Clang with `-fopenmp`).
- The MPI build requires an MPI compiler wrapper (`mpicc`) and runtime (`mpirun`/`mpiexec`).

Next steps you might want me to do:

- Add unit tests to validate accuracy vs analytic integrals.
- Add a small Python script to run benchmarks and plot speedup.
- Implement a CUDA version (if you want GPU comparison).

Benchmarks and tests
--------------------

- A simple benchmark harness is available at `bench/benchmark.py`. It runs the `bin/serial`, `bin/openmp` and `bin/mpi` binaries (if present), measures execution time over several repeats and writes `bench/results.csv`.
- Basic accuracy tests are in `tests/test_accuracy.py` and use the `serial` binary to validate numeric output against analytic integrals. Run them with `pytest` or directly via Python.

Examples (after building):

```powershell
python bench/benchmark.py --n 2000000 --repeats 3
python -m pytest tests/test_accuracy.py
```

# Parallel-Numerical-Integration
A comparative implementation of the Trapezoidal Rule in serial, OpenMP, MPI and CUDA. Demonstrates how numerical integration can be accelerated through parallelism across multicore CPUs, distributed systems and GPUs. Includes performance analysis, code samples and execution instructions.

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cuda.h>

// Device-side function selector
__device__ double device_func_eval(int func_id, double x) {
    switch (func_id) {
        case 0: return sin(x);
        case 1: return cos(x);
        case 2: return exp(x);
        case 3: return x * x;
        default: return sin(x);
    }
}

// Kernel: each thread computes a grid-stride sum over interior points and atomically adds
__global__ void trapezoid_kernel(double a, double h, long n, int func_id, double *d_sum) {
    unsigned long idx = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned long stride = blockDim.x * gridDim.x;
    double local = 0.0;
    for (unsigned long i = 1 + idx; i < (unsigned long)n; i += stride) {
        double x = a + (double)i * h;
        local += device_func_eval(func_id, x);
    }
    if (local != 0.0) {
#if __CUDA_ARCH__ >= 600
        atomicAdd(d_sum, local);
#else
        // older architectures may not support double atomicAdd; use atomicAdd on long long as fallback (less accurate)
        atomicAdd(d_sum, local);
#endif
    }
}

int main(int argc, char **argv) {
    double a = 0.0, b = 1.0;
    long n = 10000000; // default large
    int func_id = 0;
    if (argc > 1) a = atof(argv[1]);
    if (argc > 2) b = atof(argv[2]);
    if (argc > 3) n = atol(argv[3]);
    if (argc > 4) func_id = atoi(argv[4]);

    double h = (b - a) / (double)n;

    // choose launch parameters
    int threads = 256;
    int blocks = 256;

    double *d_sum;
    cudaError_t err = cudaMalloc((void**)&d_sum, sizeof(double));
    if (err != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed: %s\n", cudaGetErrorString(err));
        return 1;
    }
    cudaMemset(d_sum, 0, sizeof(double));

    // timing
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    trapezoid_kernel<<<blocks, threads>>>(a, h, n, func_id, d_sum);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "Kernel launch failed: %s\n", cudaGetErrorString(err));
        cudaFree(d_sum);
        return 1;
    }
    cudaDeviceSynchronize();

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float ms = 0.0f;
    cudaEventElapsedTime(&ms, start, stop);

    double gpu_sum = 0.0;
    cudaMemcpy(&gpu_sum, d_sum, sizeof(double), cudaMemcpyDeviceToHost);

    double fa, fb;
    switch (func_id) {
        case 0: fa = sin(a); fb = sin(b); break;
        case 1: fa = cos(a); fb = cos(b); break;
        case 2: fa = exp(a); fb = exp(b); break;
        case 3: fa = a*a; fb = b*b; break;
        default: fa = sin(a); fb = sin(b); break;
    }
    double sum = gpu_sum + 0.5 * (fa + fb);
    double result = sum * h;

    printf("CUDA Trapezoid: func=%d [%.6f, %.6f] n=%ld blocks=%d threads=%d -> result=%.12f time=%.6f ms\n",
           func_id, a, b, n, blocks, threads, result, (double)ms);

    cudaFree(d_sum);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}

#!/bin/bash
set -e
# Colab helper: compile and run the CUDA demo
echo "Checking nvcc..."
nvcc --version || { echo "nvcc not found"; exit 1; }

# clone repo if not present
REPO=Parallel-Numerical-Integration
if [ ! -d "$REPO" ]; then
  git clone https://github.com/Chamoda-S/Parallel-Numerical-Integration.git
fi
cd $REPO

mkdir -p bin
echo "Compiling CUDA demo..."
nvcc -O2 src/trapezoid_cuda.cu -Iinclude -o bin/cuda

echo "Running CUDA demo (small example)..."
./bin/cuda 0 1 10000000 0

echo "Done. Output above."

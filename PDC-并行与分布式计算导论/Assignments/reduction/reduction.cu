#include "reduction.cuh"

// naive reduction kernel
// This kernel is barely optimized and serves as a baseline
__global__ void reductionNaive(int *output, const int *input,
                               const int length) {
    int i = blockIdx.x * NAIVE_BLOCK_DIM;
    int partialSum = 0;
    
    for (int j = 0; j < NAIVE_BLOCK_DIM; j++) {
        if (i + j < length) {
            partialSum += input[i + j];
        }
    }
    atomicAdd(output, partialSum);
}

// Your optimized kernel implementations go here
// __global__ void reductionOptimized(int *output, const int *input, const int length) {}
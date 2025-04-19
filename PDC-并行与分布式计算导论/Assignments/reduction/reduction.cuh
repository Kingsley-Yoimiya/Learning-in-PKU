#ifndef REDUCTION_CUH
#define REDUCTION_CUH

#include <assert.h>
#include <stdio.h>

const int NUM_REPS = 100;   // Number of repetitions for timing
const int VEC_LEN = 1024 * 1024 * 32;  // 32M elements
const int MEM_SIZE = VEC_LEN * sizeof(int);
const int NAIVE_BLOCK_DIM = 8;

inline cudaError_t checkCuda(cudaError_t result) {
#if defined(DEBUG) || defined(_DEBUG)
    if (result != cudaSuccess) {
        fprintf(stderr, "CUDA Runtime Error: %s\n", cudaGetErrorString(result));
        assert(result == cudaSuccess);
    }
#endif
    return result;
}

// Initialize the input data and get the correct result
inline void initData(int *h_input, int vec_len, int *gold) {
    *gold = 0;
    for (int i = 0; i < vec_len; i++) {
        h_input[i] = rand() % 100;
        *gold += h_input[i];
    }
}

// Check errors and print the duration
inline void postProcess(const int *ref, const int *res, float duration) {
    if (res[0] != ref[0]) {
        printf("ref: %d, res: %d", ref[0], res[0]);
        printf("%25s\n", "*** FAILED ***");
    } else {
        printf("%25f\n", duration);
    }
}

__global__ void reductionNaive(int *output, const int *input, const int length);

// Your kernel declarations go here
// __global__ void reductionOptimized(int *output, const int *input, const int length);

#endif  // REDUCTION_CUH
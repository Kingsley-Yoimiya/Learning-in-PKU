#include <stdio.h>
#include <stdlib.h>
#include "reduction.cuh"

/*
 * Main program for benchmarking different CUDA reduction implementations
 * Compares performance of reduction kernels with baseline GPU kernel
 */

int main(int argc, char **argv) {
    // Allocate and initialize host memory
    int *h_input = (int *)malloc(MEM_SIZE);
    int gold, h_output = 0;  // gold stores CPU reference result
    initData(h_input, VEC_LEN, &gold);  // Initialize input data

    // Allocate device memory
    int *d_input, *d_output;
    checkCuda(cudaMalloc(&d_input, MEM_SIZE));  // Allocate input array on device
    checkCuda(cudaMalloc(&d_output, sizeof(int)));  // Allocate output scalar on device
    checkCuda(cudaMemcpy(d_input, h_input, MEM_SIZE, cudaMemcpyHostToDevice));  // Copy data to device

    // Create CUDA events for timing measurements
    cudaEvent_t startEvent, stopEvent;
    checkCuda(cudaEventCreate(&startEvent));
    checkCuda(cudaEventCreate(&stopEvent));
    float duration;  // Will store elapsed time in milliseconds

    // Print benchmark header
    printf("================================================================\n");
    printf("%30s%30s\n", "Routine", "Reduction time (ms)");

    /* Benchmark naive reduction kernel */
    printf("%30s", "Baseline reduction");
    // Configure kernel launch parameters:
    // - Each block has 1 thread
    // - Each thread processes NAIVE_BLOCK_DIM elements
    // - Total blocks = VEC_LEN / NAIVE_BLOCK_DIM
    dim3 dimBlockNaive(1);
    dim3 dimGridNaive(VEC_LEN / NAIVE_BLOCK_DIM);
    
    // Warmup run to avoid startup timing artifacts
    checkCuda(cudaMemset(d_output, 0, sizeof(int)));  // Clear output
    reductionNaive<<<dimGridNaive, dimBlockNaive>>>(d_output, d_input, VEC_LEN);

    // Time the kernel execution
    checkCuda(cudaEventRecord(startEvent, 0));  // Start timer
    for (int i = 0; i < NUM_REPS; i++) {
        checkCuda(cudaMemset(d_output, 0, sizeof(int)));  // Reset output
        reductionNaive<<<dimGridNaive, dimBlockNaive>>>(d_output, d_input, VEC_LEN);
    }
    checkCuda(cudaEventRecord(stopEvent, 0));  // Stop timer
    checkCuda(cudaEventSynchronize(stopEvent));  // Wait for stop event
    checkCuda(cudaEventElapsedTime(&duration, startEvent, stopEvent));  // Get duration
    
    // Copy result back to host and verify
    checkCuda(cudaMemcpy(&h_output, d_output, sizeof(int), cudaMemcpyDeviceToHost));
    postProcess(&gold, &h_output, duration / NUM_REPS);  // Compare with CPU result

    // Your optimized kernel calls go here

    printf("================================================================\n");

    // Clean up allocated resources
    free(h_input);
    checkCuda(cudaFree(d_input));
    checkCuda(cudaFree(d_output));
    checkCuda(cudaEventDestroy(startEvent));
    checkCuda(cudaEventDestroy(stopEvent));
}

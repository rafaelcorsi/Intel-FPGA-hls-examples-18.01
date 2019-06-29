#include "HLS/hls.h"
#include <stdio.h>  // For printf
#include <stdlib.h> // For rand()

#define T 1024   // Number of test samples
#define N 64     // Depth of the moving average filter

// This is the moving_avg_sum component that will be implemented in RTL.
component int moving_avg_sum(int a) {
  static int array[N];
  static int sum = 0;

  sum -= array[N-1];

  // This is the recommended code structure for shifter register implementations
#pragma unroll
  for (int i = N - 1; i > 0; --i) {
    array[i] = array[i-1];
  }
  array[0] = a;

  sum += a;

  // Note that if N is known to be a compile-time constant and a power of 2, the
  // compiler can optimize the divide into a bit-shift operation
  return (sum / N);
}

// This is the golden moving_avg_sum that will be run on the x86 testbench to
// verify the correctness of the component
int moving_avg_sum_golden(int a) {
  static int array[N];
  static int curr_index = 0;
  int sum = 0;

  array[curr_index] = a;

  for (int i = 0; i < N; ++i) {
    sum += array[i];
  }

  curr_index = (curr_index + 1) % N;

  return (sum / N);
}

int main() {
  int results[T];
  int results_ref[T];
  srand(0);

  // Enqueue T component calls
  for(int i = 0; i < T; ++i) {
    int x = rand() % 256;
    ihc_hls_enqueue(&results[i], moving_avg_sum, x);
    results_ref[i] = moving_avg_sum_golden(x);
  }

  // Run all enqueued calls to the component
  ihc_hls_component_run_all(moving_avg_sum);

  // Read and verify the results from the component
  bool passed = true;
  for (int i = 0; i < T; ++i) {
    if (results[i] != results_ref[i]) {
      passed = false;
      printf("At index %4d, %d != %d\n", i, results[i], results_ref[i]);
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}

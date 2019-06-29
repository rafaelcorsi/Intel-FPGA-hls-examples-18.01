#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"

component void vector_add(int* a,
                          int* b,
                          int* c,
                          int N) {
  for (int i = 0; i < N; ++i) {
    c[i] = a[i] + b[i];
  }
}

#define TEST_SIZE 1000
#define SEED 4

int main(void) {

  int A[TEST_SIZE];
  int B[TEST_SIZE];
  int C[TEST_SIZE];

  // prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE; ++i) {
    A[i] = rand();
    B[i] = rand();
  }

  // Run the component
  vector_add(A, B, C, TEST_SIZE);

  // Check the output
  bool passed = true;
  for (int i = 0; i < TEST_SIZE; ++i) {
    bool data_okay = (C[i] == (A[i] + B[i]));

    passed &= data_okay;
    if (!data_okay) {
      printf("ERROR: C[%d] = %d != %d\n", i, C[i], (A[i] + B[i]));
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}

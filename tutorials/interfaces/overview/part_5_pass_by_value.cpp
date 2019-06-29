#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"

struct int_v8 {
  // Passing 8 elements at a time to the component for processing
  int data[8];
};

component int_v8 vector_add(int_v8 a,
                            int_v8 b) {
  int_v8 c; 
  #pragma unroll 8
  for (int i = 0; i < 8; ++i) {
    c.data[i] = a.data[i] + b.data[i];
  }

  return c;
}

#define TEST_SIZE 1024
#define SEED 4

int main(void) {

  int_v8 A[TEST_SIZE/8];
  int_v8 B[TEST_SIZE/8];
  int_v8 C[TEST_SIZE/8];

  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE/8; ++i) {
    for(int j = 0; j < 8; ++j) {
      A[i].data[j] = rand();
      B[i].data[j] = rand();
    }
    ihc_hls_enqueue(&C[i], vector_add, A[i], B[i]);
  }
  ihc_hls_component_run_all(vector_add);

  // Check the output
  bool passed = true;
  for (int i = 0; i < TEST_SIZE/8; i+=8) {
    for (int j = 0; j < 8; ++j) {
      bool data_okay = (C[i].data[j] == (A[i].data[j] + B[i].data[j]));

      passed &= data_okay;
      if (!data_okay) {
        printf("ERROR: C[%d] = %d != %d\n", i+j, C[i].data[j], (A[i].data[j] + B[i].data[j]));
      }
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}

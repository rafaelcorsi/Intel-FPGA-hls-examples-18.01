#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"

struct int_v8 {
  // Passing 8 elements at a time to the component for processing
  int data[8];
};

component void vector_add(ihc::stream_in<int_v8>&  a,
                          ihc::stream_in<int_v8>&  b,
                          ihc::stream_out<int_v8>& c,
                          int N) {
  for (int j = 0; j < (N/8); ++j) {
    int_v8 av = a.read(); 
    int_v8 bv = b.read(); 
    int_v8 cv; 
    #pragma unroll 8
    for (int i = 0; i < 8; ++i) {
      cv.data[i] = av.data[i] + bv.data[i];
    }
    c.write(cv);
  }
}

#define TEST_SIZE 1024
#define SEED 4

int main(void) {

  int A[TEST_SIZE];
  int B[TEST_SIZE];

  // Create the streams
  ihc::stream_in<int_v8>  str_A;
  ihc::stream_in<int_v8>  str_B;
  ihc::stream_out<int_v8> str_C;

  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE; ++i) {
    A[i] = rand();
    B[i] = rand();
  }

  for (int i = 0; i < TEST_SIZE; i+=8) {
    int_v8 a;
    int_v8 b;
    for(int j = 0; j < 8; ++j) {
      a.data[j] = A[i+j];
      b.data[j] = B[i+j];
    }
    str_A.write(a);
    str_B.write(b);
  }

  // Run the component
  vector_add(str_A, str_B, str_C, TEST_SIZE);

  // Check the output
  bool passed = true;
  for (int i = 0; i < TEST_SIZE; i+=8) {
    int_v8 c = str_C.read();
    for (int j = 0; j < 8; ++j) {
      bool data_okay = (c.data[j] == (A[i+j] + B[i+j]));

      passed &= data_okay;
      if (!data_okay) {
        printf("ERROR: C[%d] = %d != %d\n", i+j, c.data[j], (A[i+j] + B[i+j]));
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

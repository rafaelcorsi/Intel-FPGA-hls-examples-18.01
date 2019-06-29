#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"

component void vector_add(ihc::mm_master<int, ihc::aspace<1>, ihc::dwidth<8*8*sizeof(int)>, ihc::align<8*sizeof(int)> >& a,
                          ihc::mm_master<int, ihc::aspace<2>, ihc::dwidth<8*8*sizeof(int)>, ihc::align<8*sizeof(int)> >& b,
                          ihc::mm_master<int, ihc::aspace<3>, ihc::dwidth<8*8*sizeof(int)>, ihc::align<8*sizeof(int)> >& c,
                          int N) {
  #pragma unroll 8
  for (int i = 0; i < N; ++i) {
    c[i] = a[i] + b[i];
  }
}

#define TEST_SIZE 1024
#define SEED 4

int main(void) {

  int A[TEST_SIZE];
  int B[TEST_SIZE];
  int C[TEST_SIZE];

  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE; ++i) {
    A[i] = rand();
    B[i] = rand();
  }

  // Create the mm_masters
  ihc::mm_master<int, ihc::aspace<1>, ihc::dwidth<8*8*sizeof(int)>, ihc::align<8*sizeof(int)> > mm_A(A, TEST_SIZE*sizeof(int));
  ihc::mm_master<int, ihc::aspace<2>, ihc::dwidth<8*8*sizeof(int)>, ihc::align<8*sizeof(int)> > mm_B(B, TEST_SIZE*sizeof(int));
  ihc::mm_master<int, ihc::aspace<3>, ihc::dwidth<8*8*sizeof(int)>, ihc::align<8*sizeof(int)> > mm_C(C, TEST_SIZE*sizeof(int));

  // Run the component
  vector_add(mm_A, mm_B, mm_C, TEST_SIZE);

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

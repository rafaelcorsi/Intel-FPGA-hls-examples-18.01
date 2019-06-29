#include "HLS/hls.h"
#include <stdio.h>
#include <stdlib.h>

component unsigned short mult_and_threshold ( unsigned char a,
                                              unsigned char b,
                                              hls_stable_argument unsigned short max_value) {
  unsigned short c = (unsigned short) a * (unsigned short) b;

  // threshold the value
  if (c > max_value) {
    c = max_value;
  }

  return c;
}

#define TEST_SIZE 100
#define SEED 98

#define THRESH1 200
#define THRESH2 124

int main(void) {
  unsigned char A[TEST_SIZE];
  unsigned char B[TEST_SIZE];
  unsigned short ret_values[TEST_SIZE];

  srand(SEED);

  // run the first half of the data through the component with one threshold
  int i = 0;
  for (i = 0; i < TEST_SIZE/2; ++i) {
    ihc_hls_enqueue(&ret_values[i], mult_and_threshold, A[i], B[i], THRESH1);
  }
  ihc_hls_component_run_all(mult_and_threshold);

  // run the second half of the data through the component with a different threshold
  for (; i < TEST_SIZE; ++i) {
    ihc_hls_enqueue(&ret_values[i], mult_and_threshold, A[i], B[i], THRESH2);
  }
  ihc_hls_component_run_all(mult_and_threshold);

  bool passed = true;
  for (int i = 0; i < TEST_SIZE; ++i) {
    // setup expected value
    unsigned short exp = (unsigned short) A[i] * (unsigned short) B[i];
    if ( (i < (TEST_SIZE/2)) && (exp > THRESH1) ) {
      exp = THRESH1;
    }
    else if ( (i >= (TEST_SIZE/2)) && (exp > THRESH2) ) {
      exp = THRESH2;
    }

    // check return value
    bool data_okay = (exp == ret_values[i]);
    passed &= data_okay;
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}

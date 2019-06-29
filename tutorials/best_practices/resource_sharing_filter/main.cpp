#include "HLS/hls.h"
#include "HLS/math.h"

#include <stdio.h> //printf
#include <stdlib.h> //srand, rand

// Number of taps
#define NUM_TAPS 32
// Number of test cases
#define TEST_CASES 128

#define TEST_SEED 3

// HLS component to test
// implemented in filter.cpp
component float firfilt (float x);
component float firfilt_shared (float x);

// CPU implementation
void golden_fir_filter(float* test_inputs, float* expected, int num_inputs) {

  static const float coeffs[NUM_TAPS] = {
    -0.003852168201892195f,
    -0.013081856594749709f,
    0.010010260080449414f,
    0.031291002556661535f,
    0.016086283040442007f,
    -0.038895648712939476f,
    -0.030625680181133576f,
    0.006840427139813927f,
    0.007537995674188784f,
    -0.011211077287440443f,
    0.06365912824643992f,
    0.10016861585467923f,
    -0.05201892745373349f,
    -0.2194681135600675f,
    -0.0767203890034225f,
    0.21884627232526455f,
    0.21884627232526455f,
    -0.0767203890034225f,
    -0.2194681135600675f,
    -0.05201892745373349f,
    0.10016861585467923f,
    0.06365912824643992f,
    -0.011211077287440443f,
    0.007537995674188777f,
    0.006840427139813927f,
    -0.030625680181133576f,
    -0.038895648712939476f,
    0.016086283040442007f,
    0.031291002556661535f,
    0.010010260080449421f,
    -0.013081856594749709f,
    -0.003852168201892195f
  };

  for (int i = 0; i < num_inputs; ++i) {
    expected[i] = 0.0f;

    int c = 0;
    // Accumulate over all taps
    for (int t = i; (t > (i - NUM_TAPS)) && (t >= 0); --t) {
      expected[i] += test_inputs[t] * coeffs[c++];
    }
  }

}

int main() {
  float test_inputs[TEST_CASES];
  float expected[TEST_CASES];
  float out[TEST_CASES];

  srand(TEST_SEED);

  // Generate test inputs
  for (int i = 0; i < TEST_CASES; ++i) {
    // generate inputs between 0.0 and 20.0
    float x = (float) rand() / (((float) RAND_MAX) / 20.0f);
    test_inputs[i] = x;
  }

  // Generate expected outputs from golden models
  golden_fir_filter(test_inputs, expected, TEST_CASES);

  // Test the non-shared version of the fir_filter
  for (int i = 0; i < TEST_CASES; ++i) {
    // Enqueue the input to the component for execution in the simulator, but don't run the component right away
    //    When the component is simulated, its pipeline will be completely filled by the input data
    //    The output data is available for the testbench to read from the out stream
    // Emulation runs this immediately
    ihc_hls_enqueue(out+i, firfilt, test_inputs[i]);
  }
  ihc_hls_component_run_all(firfilt);
  // Run filter component and check outputs
  int fail_count = 0;
  for (int i = 0; i < TEST_CASES; ++i) {
    // retrieve the next output from the component
    float y = out[i];

    // verify the correctness of the component's output
    if (fabs(y - expected[i]) > 0.0001) {
      printf("ERROR: Expected %f, got %f\n", expected[i], y);
      fail_count++;
    }
  }

  // print final test results
  if (fail_count == 0) {
    printf("PASSED (%d tests run)\n", TEST_CASES);
  } else {
    printf("FAILED (%d incorrect results)\n",fail_count);
  }

  // Test the shared version of the firfilt
  for (int i = 0; i < TEST_CASES; ++i) {
    ihc_hls_enqueue(out+i, firfilt_shared, test_inputs[i]);
  }
  ihc_hls_component_run_all(firfilt_shared);

  // Run filter component and check outputs
  fail_count = 0;
  for (int i = 0; i < TEST_CASES; ++i) {
    // retrieve the next output from the component
    float y = out[i];

    // verify the correctness of the component's output
    if (fabs(y - expected[i]) > 0.0001) {
      printf("ERROR: Expected %f, got %f\n", expected[i], y);
      fail_count++;
    }
  }

  // print final test results
  if (fail_count == 0) {
    printf("PASSED (%d tests run)\n", TEST_CASES);
  } else {
    printf("FAILED (%d incorrect results)\n",fail_count);
  }

  return 0;
}


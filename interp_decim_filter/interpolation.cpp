#include "HLS/hls.h"
#include "HLS/math.h"
#include "coefs.h"

// Test function for the interpolation filter:
//   This function is intended to run a simple interpolation filter on the CPU
//   and generate the golden result which can then be compared against the
//   output from the component.
void interpolation_test (float *in_vals, float *out_vals, int N) {
  // Keep a copy of the current delay_line
  float delay_line[INTERP * NUM_SUB_TAPS] = {0};
  // The delay line here is used as a circular buffer so keep track of the
  // current position.
  int d_pos = 0;

  // Iterate over the number of inputs that are provided to the function
  for (int i = 0; i < N; i++) {
    // Shift in the new input into the delay line
    delay_line[d_pos] = in_vals[i];

    for (int j = 0; j < INTERP; j++) {
      float accum = 0;

      // Compute the result of the FIR filter and accumulate in accum
      for (int k = 0; k < INTERP * NUM_SUB_TAPS; k++) {
        int buf_pos = (d_pos + 1 + k) % (INTERP * NUM_SUB_TAPS);
        accum += delay_line[buf_pos] * coefs_interpolation[k];
      }

      // Store the result back to the output buffer and increment the delay
      // line position
      out_vals[i * INTERP + j] = accum;
      d_pos = (d_pos + 1) % (INTERP * NUM_SUB_TAPS);

      // Make sure the intermediate values in the delay line are 0
      delay_line[d_pos] = 0;
    }
  }
}

// By default, the compiler optimizes for the best throughput/area tradeoff. In
// this particular case, to reduce area utilization, the compiler optimized the
// design to have a maximum concurrency of 1. This attribute
// (hls_max_concurrency) can be used to configure this. A value of 1 (or N)
// tells the compiler that only one (or N) iteration(s) will be in flight at a
// time. A value of 0 tells the compiler to provide the maximum possible
// throughput.
hls_max_concurrency(0)
component void interpolation (ihc::stream_in<float>  &input_stream,
                              ihc::stream_out<float> &output_stream)
{
  // Use a delay line for the input data
  hls_register static float delay_line[NUM_SUB_TAPS];

  // Read the input data from the input stream
  float data = input_stream.read();

  // Shift the delay line - this implements a shift register in hardware
  #pragma unroll
  for (int i = 0; i < NUM_SUB_TAPS - 1 ; i++) {
    delay_line[i] = delay_line[i+1];
  }
  // Store the new data into the shift register
  delay_line[NUM_SUB_TAPS - 1] = data;

  // Perform the poly-phase filtering
  for (int i = INTERP - 1; i >= 0; i--) {
    float accum = 0.0f;
    #pragma unroll
    for (int j = 0; j < NUM_SUB_TAPS ; j++) {
      accum += coefs_interpolation[i + j * INTERP] * delay_line[j];
    }

    // Write each of the results back to the output stream
    output_stream.write(accum);
  }
}

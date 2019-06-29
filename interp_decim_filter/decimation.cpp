#include "HLS/hls.h"
#include "HLS/math.h"
#include "coefs.h"

// Test function for the decimation filter:
//   This function is intended to run a simple interpolation filter on the CPU
//   and generate the golden result which can then be compared against the
//   output from the component.
void decimation_test (float *in_vals, float *out_vals, int N) {
  // Keep a copy of the current delay line
  float delay_line[DECIM * NUM_SUB_TAPS] = {0};
  // The delay line here is used as a circular buffer so keep track of the
  // current position.
  int d_pos = DECIM * NUM_SUB_TAPS - 1;

  // Iterate over the number of outputs that have to be produced
  for (int i = 0; i < N; i++) {
    float accum = 0;

    // Shift in new data values into the delay line
    for (int j = 0; j < DECIM; j++) {
      delay_line[d_pos] = in_vals[i * DECIM + j];
      d_pos = (d_pos + 1) % (DECIM * NUM_SUB_TAPS);
    }

    // Compute the result of the FIR filter and accumulate in accum
    for (int j = 0; j < DECIM * NUM_SUB_TAPS; j++) {
      int d_pos2 = (d_pos + j) % (DECIM * NUM_SUB_TAPS);
      accum += delay_line[d_pos2] * coefs_decimation[j];
    }

    // Store the result back to the output buffer
    out_vals[i] = accum;
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
component void decimation (ihc::stream_in<float>  &input_stream,
                           ihc::stream_out<float> &output_stream)
{
  // Use a delay line for each of the polyphase filter
  hls_register static float delay_line[DECIM][NUM_SUB_TAPS];
  float accum = 0.0f;

  for (int i = 0; i < DECIM; i++) {
    // Use a temporary partial delay line to break the loop-carried dependency
    hls_register float delay_line_t[NUM_SUB_TAPS];

    // Shift delay lines
    #pragma unroll
    for (int j = 0; j < NUM_SUB_TAPS - 1; j++) {
      delay_line_t[j] = delay_line[i][j+1];
    }
    // Read in new data value from the input stream
    delay_line_t[NUM_SUB_TAPS - 1] = input_stream.read();

    // Copy the temporary delay line back to the delay_line
    #pragma unroll
    for (int j = 0; j < NUM_SUB_TAPS; j++) {
      delay_line[i][j] = delay_line_t[j];
    }

    // Perform the polyphase filtering
    #pragma unroll
    for (int j = 0; j < NUM_SUB_TAPS; j++) {
      accum += delay_line_t[j] * coefs_decimation[i + j * DECIM];
    }
  }

  // Write the result back to the output stream
  output_stream.write(accum);
}

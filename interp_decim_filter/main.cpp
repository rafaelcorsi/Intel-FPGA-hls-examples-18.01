#include "HLS/hls.h"
#include "HLS/math.h"             // for sinf()
#include "coefs.h"
#include <stdio.h>

int main() {
  bool decimation_passed          = true;
  bool interpolation_passed       = true;
  const float threshold           = 0.000001f;
  const unsigned int interp_count = NUM_CASES * INTERP;
  const unsigned int decim_count  = NUM_CASES * INTERP / DECIM;

  float input_array[NUM_CASES];

  // Generate a sine wave to test the interpolation and decimation
  for (int i = 0; i < NUM_CASES ; i++) {
    input_array[i] = sinf(((float) i) * 8 * 3.14159f / NUM_CASES);
  }

  // Temporary arrays to hold the input as well as the intermediate results
  float interpolation_golden_ref[interp_count] = {0};
  float res2[decim_count]                      = {0};
  float decimation_golden_ref[decim_count]     = {0};

  // Test the interpolation
  {
    ihc::stream_in<float>   interpolation_input_stream;
    ihc::stream_out<float> interpolation_output_stream;

    // Use the interpolation_test() to generate the golden output. This function
    // does not use polyphase filtering
    interpolation_test (input_array, interpolation_golden_ref, NUM_CASES);

    // The component uses a streaming input so populate the input stream with
    // real data.
    for (int i = 0; i < NUM_CASES; i++) {
      interpolation_input_stream.write(input_array[i]);
    }

    // Enqueue the component once for each input
    for (int i = 0; i < NUM_CASES; i++) {
      ihc_hls_enqueue_noret(interpolation, interpolation_input_stream, interpolation_output_stream);
    }
    // Run all enqueued calls to the component as quickly as the component can
    // accept new invocations
    ihc_hls_component_run_all(interpolation);

    // Read back from the component stream and compare with the golden result
    for (int i = 0; i < interp_count; i++) {
      float interp_result = interpolation_output_stream.read();
      // Compare the component output with the golden reference
      if (fabs(interp_result - interpolation_golden_ref[i]) > threshold) {
        interpolation_passed = false;
      }
    }

    if (!interpolation_passed) printf("FAIL: interpolation\n");
  }

  // Test the decimation using the output from the interpolation filter
  {
    ihc::stream_in<float> decimation_input_stream;
    ihc::stream_out<float> decimation_output_stream;

    // Use the decimation_test() to generate the golden output. This function
    // also does not use polyphase filtering.
    decimation_test (interpolation_golden_ref, decimation_golden_ref, decim_count);

    // The decimation component uses a streaming input so populate the input
    // stream with the correct data.
    for (int i = 0; i < interp_count; i++) {
      decimation_input_stream.write(interpolation_golden_ref[i]);
    }

    // Enqueue the component once per each output generated
    for (int i = 0; i < decim_count; i++) {
      ihc_hls_enqueue_noret(decimation, decimation_input_stream, decimation_output_stream);
    }
    // Run all enqueued calls to the component as quickly as the component can
    // accept new invocations
    ihc_hls_component_run_all(decimation);

    // Read back the data from the output stream and compare with the golden result
    for (int i = 0; i < decim_count; i++) {
      float decim_result = decimation_output_stream.read();
      // Compare the component output (decim_result) with decimation_golden_ref
      if (fabs(decim_result - decimation_golden_ref[i]) > threshold) {
        decimation_passed = false;
      }
    }
    if (!decimation_passed) printf("FAIL: decimation\n");
  }

  if (decimation_passed & interpolation_passed) printf("PASSED\n");

  return 0;
}

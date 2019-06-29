#include "HLS/hls.h"

#include <stdio.h>  //printf
#include <stdlib.h> //srand, rand

// Number of taps
#define NUM_TAPS 32

component float firfilt (float x) {
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

  // The FIFO is sized based on the number of taps. Note that in
  // this case, the capacity of the FIFO is larger than required. The compiler
  // will optimize the extra storage away.
  static float taps[NUM_TAPS];

  float y = 0.0f;

  // shift in input data
  #pragma unroll
  for(int i = (NUM_TAPS - 1); i > 0; --i) {
    taps[i] = taps[i-1];
  }
  taps[0] = x;

  // accumulate over all taps
  #pragma unroll
  for(int i = (NUM_TAPS - 1); i >= 0; --i) {
    y += taps[i] * coeffs[i];
  }

  return y;
}


component float firfilt_shared (float x) {
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
    -0.003852168201892195
  };

  // The FIFO is sized based on the number of taps. Note that in this case, the
  // capacity of the FIFO is larger than required. The compiler will optimize
  // the extra storage away.
  static float taps[NUM_TAPS];
  static int index = 0;

  float y = 0.0f;

  // shift in input data
  taps[index] = x;

  // accumulate over all taps
  #pragma unroll 1
  for(int i = (NUM_TAPS - 1); i >= 0; --i) {
    if(++index >= (NUM_TAPS)) index = 0;
    y += taps[index] * coeffs[i];
  }

  if(++index >= (NUM_TAPS)) index = 0;

  return y;
}


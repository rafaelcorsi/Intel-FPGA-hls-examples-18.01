// See README for a description of this tutorial
#include <stdio.h>
#include "HLS/hls.h"

extern "C" {
  int scale_factor = 2;
  const int const_scale_factor = 2;
};

component int scale_value(int x) {
  #ifdef USE_CONST
  // A const global value cannot be modified, so the compiler can replace
  // const_scale_factor with the value 2 and optimize
  return x * const_scale_factor;
  #else
  // A non-const global is owned by the testbench (not the component).  So the
  // compiler will build an AVMM interface to external memory and expect the
  // user to pass in a pointer to where scale_factor is stored.
  return x * scale_factor;
  #endif
}

int main() {
  #ifdef USE_CONST
  printf("Use const qualified global value...\n");
  #else
  printf("Use mutable global value...\n");
  #endif
  printf("scale_value(%d) = %d\n", 1, scale_value(1));
  printf("scale_value(%d) = %d\n", 4, scale_value(4));
  return 0;
}

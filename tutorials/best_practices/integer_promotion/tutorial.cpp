// See README for a description of this tutorial
#include "HLS/hls.h"
#include <stdio.h>

component int add_width_cast(unsigned char a, unsigned char b) {
  // Operands explicitely promoted to int, so operation will be performed with
  // 32-bit arithmetic and produce a 32-bit result.
  int sum = (int)a + (int)b;
  return sum;
}

component int add_width(unsigned char a, unsigned char b) {
  // Operands are of type unsigned char, so by default the operation will be
  // performed with 8-bit arithmetic in i++.  Compilers applying integer
  // promotion rules will promote this operation to happen in 32-bit
  // arithmetic.  Differences in overflow behavior will be observed, with i++
  // truncating at 8-bits and most others preserving the overflow bits.
  int sum = a + b;
  return sum;
}

int main() {
  unsigned char a = 255, b = 1;
  printf("add_width_cast: %hhu + %hhu = %d\n", a, b, add_width_cast(a, b));
  printf("add_width:      %hhu + %hhu = %d\n", a, b, add_width(a, b));
  // This addition is done in the testbench, but follows the same rules as
  // components in i++.
  int sum = a + b;
  printf("add_testbench:  %hhu + %hhu = %d\n", a, b, sum);
  return 0;
}

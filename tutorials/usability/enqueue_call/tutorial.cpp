#include "HLS/hls.h"
#include <stdio.h>   // For printf

component int dut(int a, int b) {
  return a*b;
}
int main (void) {
  int x1, x2, x3;

  x1 = dut(1, 2);
  x2 = dut(3, 4);
  x3 = dut(5, 6);
  printf("x1 = %d, x2 = %d, x3 = %d\n", x1, x2, x3);

  ihc_hls_enqueue(&x1, &dut,  7,  8);
  ihc_hls_enqueue(&x2, &dut,  9, 10);
  ihc_hls_enqueue(&x3, &dut, 11, 12);
  ihc_hls_component_run_all(dut);

  printf("x1 = %d, x2 = %d, x3 = %d\n", x1, x2, x3);

  return 0;
}

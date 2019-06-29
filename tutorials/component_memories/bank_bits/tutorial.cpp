// See README for a description of this tutorial
#include "HLS/hls.h"

component int bank_arb_consecutive_multidim (int raddr, int waddr, int wdata, int upperdim) {

  #if defined BANKBITS_GOOD
  int a[2][4][128] __attribute__((bank_bits(8,7),bankwidth(4)));
  #elif defined BANKBITS_BAD
  int a[2][4][128] __attribute__((bank_bits(4,3),bankwidth(4)));
  #else
  int a[2][4][128];
  #endif

  #pragma unroll
  for (int i = 0; i < 4; i++) {
    a[upperdim][i][(waddr & 0x7f)] = wdata + i;
  }

  int rdata = 0;

  #pragma unroll
  for (int i = 0; i < 4; i++) {
    rdata += a[upperdim][i][(raddr & 0x7f)];
  }

  return rdata;
}

/****************************************************************************
These designs expect the memories a and b to be merged side by side. That is,

 M20K                 M20K                             M20K
+------+             +------+                    +------+------+
|      |             |      |                    |      |      |
|      |             |      |                    |      |      |
|   a  | 256-words   |   b  | 256-words     =>   |   a  |   b  | 256-words
|      |             |      |                    |      |      |
|      |             |      |                    |      |      |
+------+             +------+                    +------+------+
16-bits              16-bits                         32-bits

Resources:
2 loads (16-bits)                               1 load (32-bits)
2 stores (16-bits)                          =>  1 store (32-bits)
2 M20Ks                                         1 M20K

****************************************************************************/
#include "HLS/hls.h"

component short width_manual (int raddr, int waddr, short wdata) {

  #ifdef MERGE
  // Manually force the compiler to merge the memories width-wise
  hls_merge("mem_name", "width") short a[256];
  hls_merge("mem_name", "width") short b[256];
  #else
  short a[256];
  short b[256];
  #endif

  short rdata = 0;

  // Lock step write (could have constant offset)
  a[waddr] = wdata;
  b[waddr] = wdata;

  // Lock step read (could have constant offset)
  rdata += a[raddr];
  rdata += b[raddr];

  return rdata;
}

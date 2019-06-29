/****************************************************************************
This design shows how merge attributes can be used to force the compiler to
merge memories a and b in a stacked manner.
That is,

 M20K                 M20K                        M20K
+------+             +------+                    +------+
|      |             |      |                    |      |
|      |             |      |                    |      |
|   a  | 128-words   |   b  | 128-words     =>   |   a  |
|      |             |      |                    |      |
|      |             |      |                    |      |
+------+             +------+                    +------+ 256-words
32-bits              32-bits                     |      |
                                                 |      |
                                                 |   b  |
                                                 |      |
                                                 |      |
                                                 +------+
                                                 32-bits
Resources:
2 loads (32-bits)                               1 load (32-bits)
2 stores (32-bits)                          =>  1 store (32-bits)
2 M20Ks                                         1 M20K


****************************************************************************/
#include "HLS/hls.h"

component int depth_manual (bool use_a, int raddr, int raddr2, int waddr, int waddr2, int wdata) {

  #ifdef MERGE
  // Manually force the compiler to merge the memories depth-wise
  hls_merge("mem_name", "depth") int a[128];
  hls_merge("mem_name", "depth") int b[128];
  #else
  int a[128];
  int b[128];
  #endif

  int rdata;

  // mutually exclusive write
  if (use_a) {
    a[waddr] = wdata;
  } else {
    b[waddr2] = wdata;
  }

  // mutually exclusive read
  if (use_a) {
    rdata = a[raddr];
  } else {
    rdata = b[raddr2];
  }

  return rdata;

}

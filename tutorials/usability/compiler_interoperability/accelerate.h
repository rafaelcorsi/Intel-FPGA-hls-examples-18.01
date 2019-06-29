// See README for a description of this tutorial
#include "HLS/hls.h"

#ifdef  __cplusplus
// component names are always extern "C", never C++.
// In Windows all calls between modules compiled with different compilers have to be extern "C"
extern "C" {
#endif

void my_fma(ihc::stream_in<int> &a, ihc::stream_in<int> &b, ihc::stream_in<int> &c, ihc::stream_out<int> &res);

#ifdef  __cplusplus
};
#endif


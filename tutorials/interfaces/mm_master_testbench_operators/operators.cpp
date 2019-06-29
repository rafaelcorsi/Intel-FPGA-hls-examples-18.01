#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"

using namespace std;
using namespace ihc;

typedef mm_master<unsigned int, dwidth<32>, awidth<64>, aspace<1>, latency<0>, maxburst<1> > mm_src_t;
typedef mm_master<unsigned int, dwidth<32>, awidth<64>, aspace<3>, latency<0>, maxburst<1> > mm_dst_t;

#define SEED 3
#define NUM_SAMPLES 8
//#define DUMP_BUFFERS

int verify_data(int *src, int *dst, int count, unsigned int x=0) {
  int errors = 0;
  for(int i = 0; i < count; i++) {
    if(dst[i] != (src[i] + x)) {
      errors++;
      printf("mismatch at index %d: 0x%llx != 0x%llx\n", i, (unsigned long long)dst[i], (unsigned long long)src[i] + x);
    }
  }
  return errors;
}

template<typename T> void dump_buffers(const char* title, T *ref, T *dst, unsigned size) {
#ifdef DUMP_BUFFERS
  // Title
  printf("%s:\n", title);
  if(size == 0) return;
  // Reference
  printf("  REF:");
  for(unsigned i=0; i<size-1; ++i) {
    printf(" 0x%04x,", ref[i]);
    if(i % 8 == 7) printf("\n      ");
  }
  printf(" 0x%04x\n", ref[size-1]);
  // Destination
  printf("  DST:");
  for(unsigned i=0; i<size-1; ++i) {
    printf(" 0x%04x,", dst[i]);
    if(i % 8 == 7) printf("\n      ");
  }
  printf(" 0x%04x\n", dst[size-1]);
#endif
}

component void add_x(mm_src_t &src, mm_dst_t &dst, unsigned int x) {
  *dst = *src + x;
}

int main () {
  int x = 0;
  int errors = 0;
  int src_mem[NUM_SAMPLES];
  int dst_mem[NUM_SAMPLES];

  // initialize data
  srand(SEED);
  for (int i=0; i<NUM_SAMPLES; ++i) {
    src_mem[i] = rand() % 2048;
    dst_mem[i] = -1;
  }
  dump_buffers("Initial", src_mem, dst_mem, NUM_SAMPLES);


  /*
    1)
    This code creates a new mm_master interface for each iteration of the loop.
  */
  x = 0x10;
  for (int i=0; i<NUM_SAMPLES; ++i) {
    mm_src_t src_mm_i(&src_mem[i], 1*sizeof(int));
    mm_dst_t dst_mm_i(&dst_mem[i], 1*sizeof(int));
    ihc_hls_enqueue_noret(&add_x,  src_mm_i,  dst_mm_i, x);
  }
  ihc_hls_component_run_all(add_x);
  dump_buffers("New mm_master for each index", src_mem, dst_mem, NUM_SAMPLES);
  errors += verify_data(src_mem, dst_mem, NUM_SAMPLES, x);


  /*
    2)
    This code demonstartes use of the getInterfaceAtIndex() function which can
    be used to index into a mm_master object.  This can be useful when iterating
    over an array and invoking a component on different indices of the array.
  */
  x = 0x20;
  mm_src_t src_mm(src_mem, NUM_SAMPLES*sizeof(int));
  mm_dst_t dst_mm(dst_mem, NUM_SAMPLES*sizeof(int));
  for (int i=0; i<NUM_SAMPLES; ++i) {
    ihc_hls_enqueue_noret(&add_x,  src_mm.getInterfaceAtIndex(i),  dst_mm.getInterfaceAtIndex(i), x);
  }
  ihc_hls_component_run_all(add_x);
  dump_buffers("Use getInterfaceAtIndex()", src_mem, dst_mem, NUM_SAMPLES);
  errors += verify_data(src_mem, dst_mem, NUM_SAMPLES, x);


  if (errors) {
    printf("FAILED with %d errors\n", errors);
  } else {
    printf("PASSED\n");
  }
}

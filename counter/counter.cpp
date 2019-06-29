#include "HLS/hls.h"
#include <stdio.h>

using namespace ihc;

component unsigned int count() {
  static unsigned int cnt = 0;
  return cnt++;
}

int main() {
  const int SIZE = 100;
  unsigned int result[SIZE];
  for(unsigned int i=0; i<SIZE; ++i) {
    ihc_hls_enqueue(&result[i], &count);
  }

  ihc_hls_component_run_all(count);

  bool pass = true;
  for(unsigned int i=0; i < SIZE; ++i) {
    if (result[i] != i) {
      printf("ERROR: Expected %u, found %u\n", i, result[i]);
      pass = false;
    }
  }

  if (pass) {
    printf("PASSED\n");
  }
  else {
    printf("FAILED\n");
  }

  return 0;

}


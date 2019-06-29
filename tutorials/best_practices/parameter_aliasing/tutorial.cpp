// See README for a description of this tutorial
#include "HLS/hls.h"
#include <stdio.h>

component void maybe_aliased(int seed, int *x, int *y) {
  *x=seed + 1;
  *y=seed * 3;
}

component void non_aliased(int seed, int * restrict x, int * restrict y) {
  *x=seed + 1;
  *y=seed * 3;
}

int main() {
  printf("The maybe aliased call is always correct to use.\n");
  int argument1;
  int argument2;
  maybe_aliased(2,&argument1,&argument2);
  printf("maybe_aliased(2) => %d,%d expecting 3,6\n", argument1, argument2);

  printf("But if we know they will never refer to the same storage, the compiler can do better.\n");
  int argument3;
  int argument4;
  non_aliased(2,&argument3,&argument4);
  printf("non_aliased(2) => %d,%d expecting 3,6\n", argument3, argument4);

  return 0;
}

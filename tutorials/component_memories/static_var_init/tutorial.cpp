// See README for a description of this tutorial
#include "HLS/hls.h"
#include <stdio.h>  // For printf
#include <stdlib.h> // For rand()

#define ARR_SIZE 64
#define TEST_SIZE 1024

component int counter_powerup(int x){
  hls_init_on_powerup static int counter_arr[ARR_SIZE];
  return counter_arr[x]++;
}

component int counter_reset(int x){
  hls_init_on_reset static int counter_arr[ARR_SIZE];
  return counter_arr[x]++;
}

int counter(int x){
  static int counter_arr[ARR_SIZE];
  return counter_arr[x]++;
}

int main(){
  bool pass = true;
  for(int i = 0; i < TEST_SIZE; i++){
    int x = rand() % ARR_SIZE;
    int r1 = counter_powerup(x);
    int r2 = counter_reset(x);
    int r3 = counter(x);
    if(r1 != r3) pass = false;
    if(r1 != r2) pass = false;
  }

  // reset the simulation testbench (this has no effect on the x86 components)
  if ( ihc_hls_sim_reset() ) {
    // if this is testing the FPGA components in simulation,
    // check that the reset reinitialized the statics
    for(int i = 0; i < ARR_SIZE; i++) {
      int count = counter_reset(i);
      pass &= (count == 0);
    }
  }

  if(pass){
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }
  return 0;
}

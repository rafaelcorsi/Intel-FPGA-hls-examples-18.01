#include "HLS/hls.h"
#include <stdio.h>
#include <stdlib.h>

#define cpu_swap_endianess(_x) (((_x << 24) & 0xff000000) | ((_x << 8) & 0x00ff0000) | ((_x >> 8) & 0x0000ff00) | ((_x >> 24) & 0x000000ff))

#define BUFFER_SIZE 4096
#define SEED 64

// Take an integer array and swap between
// big and little endianness at each element
component void swp_int_end (int* x,
                            int N ) {

  char* xchar = (char*) x;

  // swap
  for (int i = 0; i < N; ++i) {
    char tmp[4];

    tmp[0] = xchar[(4*i) + 3];
    tmp[1] = xchar[(4*i) + 2];
    tmp[2] = xchar[(4*i) + 1];
    tmp[3] = xchar[(4*i)    ];

    xchar[(4*i)    ] = tmp[0];
    xchar[(4*i) + 1] = tmp[1];
    xchar[(4*i) + 2] = tmp[2];
    xchar[(4*i) + 3] = tmp[3];
  }

}

int main(void) {
  int buffer[BUFFER_SIZE];
  int expected[BUFFER_SIZE];

  // fill test buffer
  srand(SEED);
  for(int i = 0; i < BUFFER_SIZE; ++i) {
    buffer[i] = rand();
    expected[i] = cpu_swap_endianess(buffer[i]);
  }

  // Run the component on the buffer data
  swp_int_end(buffer, BUFFER_SIZE);

  // check the results
  bool passed = true;
  for (int i = 0; i < BUFFER_SIZE; ++i) {
    bool data_okay = (buffer[i] == expected[i]);
    passed &= data_okay;

    if (!data_okay) {
      printf("ERROR: i = %d, 0x%08x != 0x%08x\n", i, buffer[i], expected[i]);
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}


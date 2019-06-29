#include "HLS/hls.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 32

typedef ihc::stream_in<int> my_operand;
typedef ihc::stream_out<int> my_result;

component void transpose_and_fold(my_operand &data_in, my_result &res)
{
  int i;
  int j;
  int in_buf[SIZE][SIZE];
  int tmp_buf[SIZE][SIZE];
  for (i = 0; i < SIZE * SIZE; i++) {
    in_buf[i / SIZE][i % SIZE] = data_in.read();
    tmp_buf[i / SIZE][i % SIZE] = 0;
  }

  #ifdef USE_IVDEP
  #pragma ivdep safelen(SIZE)
  #endif
  for (j = 0; j < SIZE * SIZE * SIZE; j++) {
  #pragma unroll
    for (i = 0; i < SIZE; i++) {
      tmp_buf[j % SIZE][i] += in_buf[i][j % SIZE];
    }
  }
  for (i = 0; i < SIZE * SIZE; i++) {
    res.write(tmp_buf[i / SIZE][i % SIZE]);
  }
}

int main() {
  my_operand data;
  my_result result;
  int indata_arr[SIZE][SIZE];
  int tmp_arr[SIZE][SIZE];
  int solution[SIZE * SIZE];

  srand(7);
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      data.write(indata_arr[i][j] = rand() % 8);
      tmp_arr[j][i] = 0;
    }
  }

  // compute result in component
  transpose_and_fold(data, result);

  // compute testbench reference
  for (int j = 0; j < SIZE * SIZE * SIZE; j++) {
    for (int i = 0; i < SIZE; i++) {
      tmp_arr[j % SIZE][i] += indata_arr[i][j % SIZE];
    }
  }
  for (int i = 0; i < SIZE * SIZE; i++) {
    solution[i] = tmp_arr[i / SIZE][i % SIZE];
  }

  // error check
  bool failed = false;
  for (int i = 0; i < SIZE * SIZE; i++) {
    int result_read = result.read();
    if (result_read != solution[i]) {
      printf("error at [%d]: %d != %d\n", i, result_read, solution[i]);
      failed = true;
    }
  }
  printf("%s\n", failed ? "FAILED" : "PASSED");
}

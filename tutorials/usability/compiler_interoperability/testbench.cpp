// See README for a description of this tutorial
#include <stdio.h>
#include "HLS/hls.h"
#include "accelerate.h"

int main() {
	ihc::stream_in<int> param1, param2, param3;
	ihc::stream_out<int> result;
	param1.write(5);
	param2.write(6);
	param3.write(7);
	my_fma(param1, param2, param3, result);
	printf("fma(%d,%d,%d) = %d\n", 5,6,7, result.read());
  return 0;
}

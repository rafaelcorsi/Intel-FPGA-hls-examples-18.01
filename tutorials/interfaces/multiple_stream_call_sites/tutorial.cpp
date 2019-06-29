#include "HLS/hls.h"
#include "stdio.h"

typedef ihc::stream_in<int, ihc::usesPackets<true> > int_in_t;

// Input in a stream of int
// return sum from start of packet to end of packet
component int reduce_sum(int_in_t & int_in){
  int val;
  int sum = 0;
  bool start_of_packet = false;
  bool end_of_packet = false;

  while(!start_of_packet){
    val = int_in.read(start_of_packet, end_of_packet);
  }
  sum += val;

  while(!end_of_packet){
    val = int_in.read(start_of_packet, end_of_packet);
    sum += val;
  }
  return sum;
}

int main(){
  int_in_t int_in;
  int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int start_point = 2;
  int end_point = 7;
  int sum = 0;
  bool pass = true;
  for(int i = 0; i <= 10; i++){
    int val = arr[i];
    int_in.write(val, i == start_point, i == end_point);
    if(i >= start_point && i <= end_point){
      sum += val;
    }
  }
  int result = reduce_sum(int_in);
  if(result != sum){
    pass = false;
  }

  if(pass){
    printf("PASSED\n");
  } else{
    printf("ERROR\n");
  }
  return 0;
}

#include "HLS/hls.h"
#include <stdio.h>
#include <stdlib.h>

component
void threshold(ihc::stream_in<unsigned char,  ihc::usesPackets<true>, ihc::usesValid<false> >&  in_pixel, 
               ihc::stream_out<unsigned char, ihc::usesPackets<true>, ihc::usesReady<false> >& out_pixel
              ) {
  bool start_of_packet = false;
  bool end_of_packet   = false;

  while (!end_of_packet) {
    // read in next pixel
    unsigned char pixel = in_pixel.read(start_of_packet, end_of_packet);

    //threshold
    if (pixel > (unsigned char) 200) pixel = 200;

    // write out result
    out_pixel.write(pixel, start_of_packet, end_of_packet);
  }
}

int main(void) {

  // image streams
  ihc::stream_in<unsigned char,  ihc::usesPackets<true>, ihc::usesValid<false> >  in_pixel;
  ihc::stream_out<unsigned char, ihc::usesPackets<true>, ihc::usesReady<false> > out_pixel;

  // test image dimensions
  unsigned int width  = 16;
  unsigned int height = 16;

  // randomize pixel data
  srand(3);
  for(int i = 0; i < (width * height); ++i) {
    bool start_of_packet = (i == 0);
    bool end_of_packet   = (i == ((width * height) - 1));
    in_pixel.write((unsigned char) rand(), start_of_packet, end_of_packet);
  }

  // call component
  threshold(in_pixel, out_pixel);

  // check that output pixels are below the threshold
  bool passed = true;
  for(int i = 0; i < (width * height); ++i) {
    passed &= (out_pixel.read() <= (unsigned char) 200);
  }

  if (passed) {
    printf("PASSED\n"); 
  } else {
    printf("FAILED\n"); 
  }

  return 0;
}

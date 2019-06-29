
#include "HLS/hls.h"
#include <stdio.h>
#include <stdlib.h>

component
void threshold(unsigned int width, unsigned int height, 
               ihc::stream_in<unsigned char>&  in_pixel, 
               ihc::stream_out<unsigned char>& out_pixel
              ) {
  for(int i = 0; i < (width * height); ++i) {

    // read in next pixel
    unsigned char pixel = in_pixel.read();

    //threshold
    if (pixel > (unsigned char) 200) pixel = 200;

    // write out result
    out_pixel.write(pixel);
  }
}

int main(void) {

  // image streams
  ihc::stream_in<unsigned char>  in_pixel;
  ihc::stream_out<unsigned char> out_pixel;

  // test image dimensions
  unsigned int width  = 16;
  unsigned int height = 16;

  // randomize pixel data
  srand(3);
  for(int i = 0; i < (width * height); ++i) {
    in_pixel.write((unsigned char) rand());
  }

  // call component
  threshold(width, height, in_pixel, out_pixel);

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

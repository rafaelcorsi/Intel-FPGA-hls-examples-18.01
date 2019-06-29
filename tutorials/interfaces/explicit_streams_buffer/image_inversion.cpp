// The components defined in this tutorial perform grayscale image inversion on
// 8-bit grayscale pixels. They accept the H and W of the image.

#include "HLS/hls.h"
#include <stdio.h>   // For printf
#include <stdlib.h>  // For srand() and rand()

#define IMG_W    32
#define IMG_H    32

// Default stream behavior
component void img_invert(int W,
			  int H,
			  ihc::stream_in<unsigned char>  &a,
			  ihc::stream_out<unsigned char> &b) {
  for (int i = 0; i < W * H; ++i) {
    // Blocking read from the input stream
    unsigned char a1 = a.read();

    // Perform the image inversion
    unsigned char b1 = 255 - a1;

    // Blocking write to the output stream
    b.write(b1);
  }
}

// Streaming behavior with buffers on the input stream
component void img_invert_buff(int W,
		                   int H,
		                   ihc::stream_in<unsigned char, ihc::buffer<64> > &a,
		                   ihc::stream_out<unsigned char> &b) {
  for (int i = 0; i < W * H; ++i) {
    // Blocking read from the input stream
    unsigned char a1 = a.read();

    // Perform the image inversion
    unsigned char b1 = 255 - a1;

    // Blocking write to the output stream
    b.write(b1);
  }
}


int main (void) {
  unsigned char input_image[IMG_W][IMG_H];
  unsigned char output_image_ref[IMG_W][IMG_H];
  bool pass = true;

  ihc::stream_in<unsigned char> input_stream;
  ihc::stream_in<unsigned char, ihc::buffer<64> > input_stream_buf;
  ihc::stream_out<unsigned char> output_stream1;
  ihc::stream_out<unsigned char> output_stream2;

  srand(0);
  // Populate the input image
  for (int i = 0; i < IMG_W; ++i) {
    for (int j = 0; j < IMG_H; ++j) {
      input_image[i][j] = rand() % 256;
    }
  }

  // Compute the testbench reference
  for (int i = 0; i < IMG_W; ++i) {
    for (int j = 0; j < IMG_H; ++j) {
      output_image_ref[i][j] = 255 - input_image[i][j];
    }
  }

  //
  // This section runs the default stream version of the img_invert
  //

  // Populate the input stream
  for (int i = 0; i < IMG_W; ++i) {
    for (int j = 0; j < IMG_H; ++j) {
      input_stream.write(input_image[i][j]);
    }
  }

  img_invert(IMG_W, IMG_H, input_stream, output_stream1);

  // Read from the output stream and compare against the reference image
  // Populate the input stream
  for (int i = 0; i < IMG_W; ++i) {
    for (int j = 0; j < IMG_H; ++j) {
      input_stream.write(input_image[i][j]);
      unsigned char pixel = output_stream1.read();
      if (pixel != output_image_ref[i][j]) {
        pass = false;
        printf("At index [%d][%d], %hhu != %hhu\n", i, j, pixel, output_image_ref[i][j]);
      }
    }
  }

  //
  // This section runs the buffered version of the img_invert_buff
  //

  // Populate the input stream again for the second call
  for (int i = 0; i < IMG_W; ++i) {
    for (int j = 0; j < IMG_H; ++j) {
      input_stream_buf.write(input_image[i][j]);
    }
  }

  img_invert_buff(IMG_W, IMG_H, input_stream_buf, output_stream2);

  // Read from the output stream and compare against the reference image
  // Populate the input stream
  for (int i = 0; i < IMG_W; ++i) {
    for (int j = 0; j < IMG_H; ++j) {
      input_stream.write(input_image[i][j]);
      unsigned char pixel = output_stream2.read();
      if (pixel != output_image_ref[i][j]) {
        pass = false;
        printf("At index [%d][%d], %hhu != %hhu\n", i, j, pixel, output_image_ref[i][j]);
      }
    }
  }

  if (pass) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}


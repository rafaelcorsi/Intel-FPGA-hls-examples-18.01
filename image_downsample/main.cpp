// main.cpp
#include "HLS/hls.h"
#include "resize.h"
#include "bmp_tools.h"

#include <string>
#include <stdio.h> //printf
#include <stdlib.h> // malloc, free

int main (void) {
  // image files
  std::string input_bmp_filename    = "test.bmp";
  std::string output_bmp_filename   = "downsampled.bmp";
  std::string expected_bmp_filename = "expected.bmp";

  // downsampling parameters
  unsigned int ratio = 128;

  // load image
  unsigned int* in_img = 0;;
  int rows, cols;
  read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);

  // run downsampling component
  input_image_stream in_img_s;
  output_image_stream out_img_s;
  for (int i = 0; i < (rows*cols); ++i) {
    in_img_s.write(in_img[i]);
  }
  resize(ratio, rows, cols, in_img_s, out_img_s);

  // save output image
  int out_rows = rows / (SCALE_GRAN / ratio);
  int out_cols = cols / (SCALE_GRAN / ratio);
  unsigned int* out_img = (unsigned int*) malloc(out_rows * out_cols * sizeof(unsigned int));
  for (int i = 0; i < (out_rows*out_cols); ++i) {
    out_img[i] = out_img_s.read();
  }
  write_bmp(output_bmp_filename.c_str(), out_img, out_rows, out_cols);

  // load expected output and compare
  unsigned int* exp_img = 0;;
  int exp_rows, exp_cols;
  read_bmp(expected_bmp_filename.c_str(), &exp_img, exp_rows, exp_cols);

  // check dimensions
  bool passed = (out_rows == exp_rows) && (out_cols == exp_cols);
  if (!passed) {
    printf("ERROR: dimensions (%d, %d) != (%d, %d)\n", out_cols, out_rows, exp_cols, exp_rows);
  }

  // compare image data
  if (passed) {
    for (int i = 0; i < (out_rows*out_cols); ++i) {
      passed &= (out_img[i] == exp_img[i]);
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  // free memory
  free(in_img);
  free(out_img);
  free(exp_img);

  return 0;
}

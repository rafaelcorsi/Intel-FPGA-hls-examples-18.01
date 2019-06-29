// resize.cpp
#include "HLS/hls.h"
#include "resize.h"

struct uchar4 {
  unsigned char d[4];
};

struct uint4 {
  unsigned int d[4];
};

// The maximum number of blocks supported per row. It must cover the border around the image.
#define MAXBX 91     
// The number of cells per dimension in one block
#define BLOCK_SIZE 2 
// The number of pixels per cell
#define CELL_SIZE 8  
// The number of bins per cell
#define NBINS 9      
#define MAXCOLS (MAXBX * CELL_SIZE)
#define BLOCK_HIST (NBINS * BLOCK_SIZE * BLOCK_SIZE)


/* Conversion Helper Functions */
uint4 convert_uint4(uchar4 x) {
  uint4 y;
  y.d[0] = (unsigned int) x.d[0];
  y.d[1] = (unsigned int) x.d[1];
  y.d[2] = (unsigned int) x.d[2];
  y.d[3] = (unsigned int) x.d[3];
  return y;
}

uchar4 convert_uchar4(uint4 x) {
  uchar4 y;
  y.d[0] = (unsigned char) x.d[0];
  y.d[1] = (unsigned char) x.d[1];
  y.d[2] = (unsigned char) x.d[2];
  y.d[3] = (unsigned char) x.d[3];
  return y;
}

unsigned int convert_uint(uchar4 x) {
  return ( (((unsigned int) x.d[3]) << 24) | (((unsigned int) x.d[2]) << 16)  | (((unsigned int) x.d[1]) << 8) | (((unsigned int) x.d[0]) << 0));
}

/*!
 * @brief Input an image of ROWS x COLS, and output is a reduced scale image by a factor SCALE_GRAN / ratio
 * @note It is the equivalent of gpu::HOGInvoker::resize() of OpenCV library.
 * @param[in] ratio          : Describes the scaling factor
 * @param[in] rows           : The number of rows in the original image
 * @param[in] cols           : The number of columns in the original image
 * @param[in] original_image : The original input image
 */
component void resize(unsigned ratio, int rows, int cols, input_image_stream& original_image, output_image_stream& resized_image) {
  unsigned int out_pointer = 0;
  uchar4 buf[MAXCOLS + 2]; // Must buffer one row and 2 pixels from the original image to be able to downscale using bilinear interpolation
  unsigned int accumColumn, accumRow = SCALE_GRAN - ratio;
  int col = 0, row = 0;
  bool write_x, write_y;

  // Downsample the image using vector operation. It processes row by row.
  for (int i = 0; i < rows * MAXCOLS + MAXCOLS + 1; i++) {
    if (row < rows && col < cols) {
      unsigned int predata=original_image.read();
      buf[MAXCOLS + 1].d[0]= predata&0xff;
      buf[MAXCOLS + 1].d[1]= (predata>>8)&0xff;
      buf[MAXCOLS + 1].d[2]= (predata>>16)&0xff;
      buf[MAXCOLS + 1].d[3]=0;
    }

    // Use integer logic to compute the fraction and determine when to
    // write and how to interpolate
    // write this row / column only if overflowing ratio + SCALE_GRAN
    if (col == 0) {
      accumColumn = SCALE_GRAN - ratio;
      write_y = false;
      accumRow += ratio;
      if (accumRow >= SCALE_GRAN) {
        accumRow -= SCALE_GRAN;
        write_y = true;
      }
    }
    accumColumn += ratio;
    write_x = false;
    if (accumColumn >= SCALE_GRAN) {
      accumColumn -= SCALE_GRAN;
      write_x = true;
    }
    /* The actual bilinear interpolation core. */
    if (write_x && write_y && (i >= MAXCOLS + 1) && col > 0 && (col - 1) < cols) {
      uint4 pix0 = convert_uint4(buf[0]);
      uint4 pix1 = convert_uint4(buf[1]);
      uint4 pix2 = convert_uint4(buf[MAXCOLS]);
      uint4 pix3 = convert_uint4(buf[MAXCOLS + 1]);
      /* The following operations process all components in a pixel together. */
      unsigned int rc=(accumRow&0xff)*(accumColumn&0xff);
      uint4 tmp_pixel;
      #pragma unroll
      for (int d = 0; d < 4; ++d) {
        tmp_pixel.d[d] =(pix3.d[d]*SCALE_GRAN*SCALE_GRAN +
            ((pix1.d[d]-pix3.d[d])&0x3ffffff)*(accumRow&0x3ffffff)*SCALE_GRAN +
            ((pix2.d[d]-pix3.d[d])&0x3ffffff)*(accumColumn&0x3ffffff)*SCALE_GRAN +
            ((pix0.d[d]+pix3.d[d]-pix2.d[d]-pix1.d[d])&0x3ffffff)*(rc&0x3ffffff))/SCALE_GRAN/SCALE_GRAN;
      }
      uchar4 pixel=convert_uchar4(tmp_pixel);
      resized_image.write(convert_uint(pixel));
    }
    /* Shift the buffer in preparation for the next iteration. */
    #pragma unroll
    for (int i = 0; i < MAXCOLS + 1; i++) {
      buf[i].d[0] = buf[i + 1].d[0];
      buf[i].d[1] = buf[i + 1].d[1];
      buf[i].d[2] = buf[i + 1].d[2];
      buf[i].d[3] = buf[i + 1].d[3];
    }
    col = col == MAXCOLS - 1 ? 0 : col + 1;
    row = col == 0 ? row + 1 : row;
  }
}

#ifndef YUV2RGB_H_
#define YUV2RGB_H_

#include "HLS/hls.h"

#define INPUT_FILE   "image_in.bmp"
#define OUTPUT_FILE  "image_out.bmp"

#define IMG_HEIGHT   1080
#define IMG_WIDTH    1920
#define IMG_WIDTH_DIV2 (IMG_WIDTH/2)

typedef unsigned long long sim_time_t;

typedef struct rbg2_s
{
   uint8_t r1, g1, b1, r2, g2, b2;
} rgb2_type;

typedef struct yuv_s
{
   uint8_t y1, y2, u1, v1;
} yuv_type;

component rgb2_type yuv2rgb(yuv_type yuv_data_in);

#endif // YUV2RGB_H_

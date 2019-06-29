#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "yuv2rgb.h"

component rgb2_type yuv2rgb(yuv_type yuv_data_in)
{
   rgb2_type outRgb = {
         0, 0, 0, 0, 0, 0 };
   int16_t YYY, UUU, VVV, YY1, YY2;
   int16_t R, G, B, U1, V1;

   // unpack YUV422 word which is really YYUV
   YY1 = yuv_data_in.y1;
   YY2 = yuv_data_in.y2;
   U1 = yuv_data_in.u1;
   V1 = yuv_data_in.v1;

   /*1----------------------*/
   VVV = V1 - 128;
   UUU = U1 - 128;

   YYY = (YY1 - 16);
   R = ((298 * YYY + 409 * VVV + 128) >> 8);
   G = ((298 * YYY - 100 * UUU - 208 * VVV + 128) >> 8);
   B = ((298 * YYY + 516 * UUU + 128) >> 8);
   if (R < 1) R = 1;
   if (G < 1) G = 1;
   if (B < 1) B = 1;
   if (R > 255) R = 255;
   if (G > 255) G = 255;
   if (B > 255) B = 255;
   outRgb.r1 = R;
   outRgb.g1 = G;
   outRgb.b1 = B;

   /*2-----------------------*/
   YYY = (YY2 - 16);
   R = ((298 * YYY + 409 * VVV + 128) >> 8);
   G = ((298 * YYY - 100 * UUU - 208 * VVV + 128) >> 8);
   B = ((298 * YYY + 516 * UUU + 128) >> 8);
   if (R < 1) R = 1;
   if (G < 1) G = 1;
   if (B < 1) B = 1;
   if (R > 255) R = 255;
   if (G > 255) G = 255;
   if (B > 255) B = 255;
   outRgb.r2 = R;
   outRgb.g2 = G;
   outRgb.b2 = B;

   return outRgb;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#include "yuv2rgb.h"

static const int BMP_HEADER_SIZE = 54;
static FILE* BmpOutFile = NULL;
int readBmpAsYuv(yuv_type *yuv_data) {
   int w, h, idx;
   yuv_type yuvData;
   unsigned char* bmpDataIn = NULL;
   size_t byteCount;
   FILE* bmpInFile = NULL;
   int error = 1;

   do { // false loop
      bmpInFile = fopen(INPUT_FILE, "rb");
      if (bmpInFile == NULL) {
         fprintf(stderr, "Error opening BMP file %s\n", INPUT_FILE);
         break;
      }

      // determine the height and width of the image
      for (int i=0; i<18; i++) fgetc(bmpInFile); 
      fread(&w, sizeof(int), 1, bmpInFile);
      fread(&h, sizeof(int), 1, bmpInFile);
      if (w != IMG_WIDTH || h != IMG_HEIGHT) {
         fprintf(stderr, "Bitmap must be %dW X %dH.\n", IMG_WIDTH, IMG_HEIGHT);
         break;
      }

      unsigned char header[BMP_HEADER_SIZE];
      rewind(bmpInFile);
      if (fread(header, 1, BMP_HEADER_SIZE, bmpInFile) < BMP_HEADER_SIZE) {
         fprintf(stderr, "Error reading BMP header for %s\n", INPUT_FILE);
         break;
      }

      // Now read the pixel data from the file...
      byteCount = w * h * 3;
      bmpDataIn = (unsigned char *)malloc(sizeof(unsigned char) * byteCount);

      if (fread(bmpDataIn, 1, byteCount, bmpInFile) < byteCount) {
         fprintf(stderr, "Could not read the pixel data for %s.\n", INPUT_FILE);
         break;
      }

      for (h = 0; h < IMG_HEIGHT; h++) {
         for (w = 0; w < IMG_WIDTH; w += 2) {
            int Y1, Y2, U1, V1, R, G, B;
            idx = (h * IMG_WIDTH + w) * (3);  // calc RGB location
            B = bmpDataIn[idx++];
            G = bmpDataIn[idx++];
            R = bmpDataIn[idx++];

            // calc YUV data
            Y1 = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
            U1 = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
            V1 = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

            B = bmpDataIn[idx++];
            G = bmpDataIn[idx++];
            R = bmpDataIn[idx++];
            Y2 = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;

            // pack YUV422 into stream format: Y1Y2U1V1
            yuvData.y1 = Y1;
            yuvData.y2 = Y2;
            yuvData.u1 = U1;
            yuvData.v1 = V1;
            yuv_data[h*IMG_WIDTH_DIV2+w/2] = yuvData;
         }
      }

      // open output file
      BmpOutFile = fopen(OUTPUT_FILE, "wb");
      printf("Opening output file %s\n", OUTPUT_FILE);
      if (BmpOutFile == NULL) {
         fprintf(stderr, "Error opening BMP file %s for writing\n", OUTPUT_FILE);
         break;
      }

      // Write the file header
      if (fwrite(header, 1, BMP_HEADER_SIZE, BmpOutFile) < BMP_HEADER_SIZE) {
         fprintf(stderr, "Error writing BMP header for %s\n", OUTPUT_FILE);
         break;
      }
      error = 0;
   } while (0);  // false loop

   if (bmpDataIn != NULL) {
      free(bmpDataIn);
   }
   if (bmpInFile != NULL) {
      fclose(bmpInFile);
   }

   if (error) {
      if (BmpOutFile != NULL) {
         fclose(BmpOutFile);
      }
      fprintf(stderr, "readBmp:Encountered error\n");
   }
   return error;
}

int main(void) {
   rgb2_type *rgb_data_out = new rgb2_type[IMG_HEIGHT*IMG_WIDTH_DIV2];
   yuv_type *yuv_data_in = new yuv_type[IMG_HEIGHT*IMG_WIDTH_DIV2];

   printf("Reading input data file..."); fflush (stdout);
   if(readBmpAsYuv(yuv_data_in) != 0) {
      delete [] rgb_data_out;
      delete [] yuv_data_in;
      printf("Error reading bitmap - ending application\n");
      return 1;
   }
   printf("Done\n");

   printf("Converting image YUV to RGB data"); fflush(stdout);
   uint32_t w, h;
   uint32_t progress = 0;
   for (h = 0; h < IMG_HEIGHT; h++) {
      // Emit 10 progress '.' characters so we can see something is happening...
      if((h * 10 / IMG_HEIGHT) >= progress) {
         printf("."); fflush(stdout);
         ++progress;
      }
      for (w = 0; w < IMG_WIDTH; w += 2) {
         // Use enqueue to push data in back-to-back without waiting for the
         // function to return.  This simulates the fully pipelined behaviour
         // of yuv2rgb, and also results in a faster simulation time.
         ihc_hls_enqueue(&(rgb_data_out[h*IMG_WIDTH_DIV2 + w/2]), yuv2rgb, yuv_data_in[h*IMG_WIDTH_DIV2 + w/2]);
      }
   }
   printf("Done\n");
   printf("Waiting for conversion to complete..."); fflush(stdout);
   // Wait for all of the enqueued yuv2rgb calls to complete
   ihc_hls_component_run_all(yuv2rgb);
   printf("Done\n");

   printf("Writing image to file..."); fflush(stdout);
   for (h = 0; h < IMG_HEIGHT; h++) {
      for (w = 0; w < IMG_WIDTH; w += 2) {
         rgb2_type rgbData = rgb_data_out[h*IMG_WIDTH_DIV2 + w/2];
         fwrite(&rgbData.b1, 1, 1, BmpOutFile);
         fwrite(&rgbData.g1, 1, 1, BmpOutFile);
         fwrite(&rgbData.r1, 1, 1, BmpOutFile);
         fwrite(&rgbData.b2, 1, 1, BmpOutFile);
         fwrite(&rgbData.g2, 1, 1, BmpOutFile);
         fwrite(&rgbData.r2, 1, 1, BmpOutFile);
      }
   }
   printf("Done\n");

   if (BmpOutFile != NULL) {
      fclose(BmpOutFile);
   }
   delete [] rgb_data_out;
   delete [] yuv_data_in;

   printf("PASSED\n");


   return EXIT_SUCCESS;
}

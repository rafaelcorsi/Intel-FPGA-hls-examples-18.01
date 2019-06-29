#include "HLS/hls.h"
#include "HLS/math.h"
#include <stdio.h>    // printf

#define NUM_ROWS 4
#define NUM_COLS 4
#define NUM_ELEMENTS 16
#define NUM_MATRICES 5

// result = A * B, where A and B are both n x n matrices
void matrix_multiply(float A[NUM_ROWS][NUM_COLS], float B[NUM_ROWS][NUM_COLS], float result[NUM_ROWS][NUM_COLS]) {
  #pragma unroll
  for (int row = 0; row < NUM_ROWS; row++) {
    #pragma unroll 
    for (int col = 0; col < NUM_COLS; col++) {
      float sum = 0.0f;
      #pragma unroll
      for (int idx = 0; idx < NUM_COLS; idx++) {
        float valA = A[row][idx];
        float valB = B[idx][col];
        sum += valA * valB;
      }
      result[row][col] = sum;
    }
  }
}

component void accum(int numMatrices, ihc::stream_in<float> &matrixInput, ihc::stream_out<float> &matrixOutput) {
  hls_register float currMatrix[NUM_ROWS][NUM_COLS];
  hls_register float currResult[NUM_ROWS][NUM_COLS] = {0.0f};
  hls_register float currResultCopy[NUM_ROWS][NUM_COLS] = {0.0f};

  // initialize as identity matrix
  #pragma unroll 
  for (int i = 0; i < NUM_ROWS; i++) {
    currResult[i][i] = 1.0f;
    currResultCopy[i][i] = 1.0f;
  }
  
  for (int count = 0; count < numMatrices; count++) {
    // populate currMatrix
    for (int row = 0; row < NUM_ROWS; row++) {
      for (int col = 0; col < NUM_COLS; col++) {
        currMatrix[row][col] = matrixInput.read();
      }
    }

    // in-place multiplication with deep copying
    // currResult = currMatrix * currResult <--> currResult = currMatrix * currResultCopy
    matrix_multiply(currMatrix, currResultCopy, currResult);
    #pragma unroll
    for (int row = 0; row < NUM_ROWS; row++) {
      #pragma unroll
      for (int col = 0; col < NUM_COLS; col++) {
        currResultCopy[row][col] = currResult[row][col];
      }
    }
  }
  
  // stream out matrixOutput
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      matrixOutput.write(currResult[row][col]);
    }
  }
}

void golden(float* resultBufferPtr1,float* resultBufferPtr2){
  float currMatrix[NUM_ROWS][NUM_COLS] = {0.0f}; 

  for (int i = 0; i < NUM_ROWS; i++) {
    resultBufferPtr1[i*NUM_COLS + i] = 1.0f;
    resultBufferPtr2[i*NUM_COLS + i] = 1.0f;
  }

  for (int count = 0; count < NUM_MATRICES; count++) {
    for (int row = 0; row < NUM_ROWS; row++) {
      for (int col = 0; col < NUM_COLS; col++) {
        currMatrix[row][col] = count*row*col + 1;
      }
    }

    // in-place multiplication with pointer swapping
    // currResult = currMatrix * currResult <--> resultBuffer2 = currMatrix * resultBuffer1
    matrix_multiply(currMatrix, (float(*)[4])resultBufferPtr1, (float(*)[4])resultBufferPtr2);
    
    if(count == NUM_MATRICES - 1) break;
    float* temp = resultBufferPtr1;
    resultBufferPtr1 = resultBufferPtr2;
    resultBufferPtr2 = temp;
  }
}

int main() {
  ihc::stream_in<float> matrixInput;
  ihc::stream_out<float> matrixOutput;

  // specify the matrices to be multiplied
  for (int count = 0; count < NUM_MATRICES; count++) {
    for(int row = 0; row < NUM_ROWS; row++) {
      for (int col = 0; col < NUM_COLS; col++) {
        matrixInput.write(count*row*col + 1);
      }
    }
  }

  printf("Calling matrix multiplication with deep copying and HLS registers\n");
  accum(NUM_MATRICES, matrixInput, matrixOutput);


  // correctness check
  float resultBuffer1[NUM_ROWS][NUM_COLS] = {0.0f}; 
  float resultBuffer2[NUM_ROWS][NUM_COLS] = {0.0f}; 
  float* resultBufferPtr1 = &resultBuffer1[0][0];
  float* resultBufferPtr2 = &resultBuffer2[0][0];
  golden(resultBufferPtr1, resultBufferPtr2);

  bool passed = true;
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS; col++) {
      if(matrixOutput.read() != resultBufferPtr2[row*NUM_COLS + col]) {
        passed = false;
      }
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}

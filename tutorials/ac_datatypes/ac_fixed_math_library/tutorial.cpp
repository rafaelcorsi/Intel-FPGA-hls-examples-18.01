#include "HLS/hls.h"
#include "HLS/ac_fixed.h"
#include "HLS/ac_fixed_math.h"
#include "stdio.h"

typedef ac_fixed<10, 3, true> fixed_10_3_t;
typedef ac_fixed<9, 2, true> fixed_9_2_t;

component 
float
test_float(float x){
  float sin_ret = sinf(x);
  float cos_ret = cosf(x);
  return sqrtf(sin_ret * sin_ret + cos_ret * cos_ret);
}

// Please refer to ac_fixed_math.h header file for fixed point math functions'
// type deduction rule. In this case, following those rules:
// I, W, S are input type template parameter (ac_fixed<W, I, S>)
// rI, rW, rS are output type template parameter (ac_fixed<rW, rI, rS>)
//* Function Name              Type Propagation Rule
//* sqrt_fixed                 rI = I, rW = W, rS = S
//* sin_fixed                  For signed (S == true), rI == 2, rW =  W - I + 2; 
//*                            For unsigned (S == false), I == 1, rW =  W - I + 1
//* cos_fixed                  For signed (S == true), rI == 2, rW =  W - I + 2; 
//*                            For unsigned (S == false), I == 1, rW =  W - I + 1
component 
fixed_9_2_t 
test_fixed(fixed_10_3_t x){
  fixed_9_2_t sin_ret = sin_fixed(x);
  fixed_9_2_t cos_ret = cos_fixed(x);
  return sqrt_fixed(sin_ret * sin_ret + cos_ret * cos_ret);
}

const int SIZE = 5;

int main(){
  float inputs[SIZE] = {
    -0.807991899423 ,
    -2.09982907558 ,
    -0.742066235466 ,
    -2.33217071676 ,
    1.14324158042
  };

  // quantum: the minimum positive value this type can represent
  fixed_9_2_t quantum = 0.0078125;

  // for fixed point, the error should be less than 1 quantum of data type (1 / 2^(W - I))
  static const fixed_9_2_t epsilon_fixed_9_2 = quantum;
  static const float epsilon_float = 1.0/(1.0 * float(1 << 20));

  printf("MAX DIFF for ac_fixed<10, 3, true>:  %.8f\n", epsilon_fixed_9_2.to_double());
  printf("MAX DIFF for float:  %.8f\n\n", epsilon_float);

  bool pass = true;

  for(int i = 0; i <SIZE; i++){
    fixed_10_3_t input1 = inputs[i];
    float input2 = inputs[i];

    // declare output and diff variable
    fixed_9_2_t result1 = test_fixed(input1);
    float result2 = test_float(input2);

    printf("result(fixed point): %.8f\n", result1.to_double());
    printf("result(float): %.8f\n\n", result2);

    // expected result is 1.0 = sqrt(sin^2(x) + cos^2(x))
    fixed_9_2_t diff1 = fabs(result1.to_double() - 1.0);
    float diff2 = fabs(result2 - 1.0);

    if(diff1 > epsilon_fixed_9_2 || diff2 > epsilon_float){
      pass = false;
    }
  }
  if(pass){
    printf("PASSED\n");  
  } else {
    printf("ERROR\n");
  }
  return 0;
}


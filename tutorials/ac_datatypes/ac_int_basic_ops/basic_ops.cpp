#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

// Definition of ac_int:
// ac_int<width, signed>
// For convenience:
//    ac_int<N, true>  are type defined as  intN up to 63.
//    ac_int<N, false> are type defined as uintN up to 63.

/******************************************************************************
 * ac_int integer promotion rules:                                            *
 *                                                                            *
 * ac_int automatically promotes the result of all operations to the number   *
 * number of bits needed to represent all possible results without            *
 * overflowing.                                                               *
 *                                                                            *
 * For example, the addition of two 8-bit integers results in a               *
 * 9-bit result to support overflow. Internally, the result will be 9-bits.   *
 * However, if the user attempts to store the result in an 8-bit container,   *
 * ac_int will let the user do this, but leads to the discard of the extra    *
 * carry bit. The responsibility lies on the user to use the correct          *
 * datatype.                                                                  *
 *                                                                            *
 * These promotions rules are consistent across all architectures so the      *
 * behavior should be equivalent on x86 or on FPGA.                           *
 *                                                                            *
 *****************************************************************************/

// For a 14-bit addition, the result should be stored in a 15-bit container
component int15 s_adder(int14 a, int14 b) {
  return (a + b);
}

// For a 14-bit multiplier, the result should be stored in a 28-bit container
component int28 s_multiplier(int14 a, int14 b) {
  return (a * b);
}

// For a 14-bit division, the result should be stored in a 15-bit container
component int15 s_divider(int14 a, int14 b) {
  return (a / b);
}

/* The behavior of shift operations in ac_int are slightly different that what
 * is defined in the C spec. For full details, see the ac_int documentation.
 * Some key points to remember are as follows:
 *    - If the datatype of the shift amount is signed, negative shift amounts are
 *      accepted. A shift by a negative amount is equivalent to a positive shift
 *      in the opposite direction.
 *    - If the user knows that the shift direction and that the shift value is
 *      always positive, it is advisable to use an unsigned datatype for the
 *      shift value to obtain better QoR.
 *    - Shift values of greater than the width of the datatypes are treated as a
 *      shift equal to the width of the datatype.
 */
component int14 shift_left(int14 a, int14 b) {
  // b can be positive or negative. If (b > 0), a will be shifted to the left.
  // Else, a will be shifted to the right
  return (a << b);
}

component int14 effcnt_left_shift(int14 a, uint4 b) {
  /* b is always positive in this case. This shift is more efficient in HW than
   * the shift_left method. If the direction of the shift is known at compile
   * time, this is recommended. Note that the two datatypes need not be the
   * same. Note that the datatype of b here is just 4 bits since 4 bits can
   * completely contain the value of the full width of a. This will generate a
   * more efficient datapath.
   */
  return (a << b);
}

// The method
//      x = y.slc<M>(n)
// is equivalent to the VHDL behavior of
//      x := y((M+n-1) downto n);
// Note that only static bit widths are supported
component uint4 get_bit_slice(int14 a, int lsb) {
  return (a.slc<4>(lsb));
}

// There is a set_slc(int lsb, const ac_int<W, S> &slc) which allows the user to
// set a bit slice as shown in the example below.
component int14 set_bit_slice(int14 a, int4 b) {
  // Sets the bits [9:6] of a with the value stored in b
  a.set_slc(6, b);

  // Bits can also be individually set as follows:
  a[3] = 0;
  a[2] = 0;
  a[1] = 0;
  a[0] = 0;

  return a;
}

int main() {
  bool passed = true;
  // Initialize two random ints
  int t1 = rand();
  int t2 = rand();
  // Truncate each of the two ints to 13 bits. The 14th bit is the sign bit. In
  // this testbench, even though the datatypes are signed, we will be storing
  // unsigned values to make the testing process more simple.
  t1 &= (1 << 13) - 1;
  t2 &= (1 << 13) - 1;

  printf("Arithmetic Operations:\n");
  // Test adder
  {
    // ac_int offers type casting from and to native datatypes
    int14 a = t1;
    int14 b = t2;
    int15 c = s_adder(a, b);
    int c_golden = t1 + t2;
    // We can check the result of the ac_int addition with the native C int addition
    if (c != c_golden) {
      passed = false;
      printf("Addition failed\n");
    }
    printf("ac_int: %d + %d = %d\n", (int)a, (int)b, (int)c);
    printf("int:    %d + %d = %d\n", t1, t2, c_golden);
  }

  // Test multiplier
  {
    t1 = rand() & ((1<<13) - 1);
    int14 a = t1;
    int14 b = t2;
    int28 c = s_multiplier(a, b);
    int c_golden = t1 * t2;
    // We can check the result of the ac_int multiplication with the native C
    // int multiplication
    if (c != c_golden) {
      passed = false;
      printf("Multiplier failed\n");
    }
    printf("ac_int: %d * %d = %d\n", (int)a, (int)b, (int)c);
    printf("int:    %d * %d = %d\n", t1, t2, c_golden);
  }

  // Test divider
  {
    t1 = rand() & ((1<<13) - 1);
    t2 = rand() % 50; // Use a small value for the divisor so that the result is not 0 or 1
    int14 a = t1;
    int14 b = t2;
    int28 c = s_divider(a, b);
    int c_golden = t1 / t2;
    // We can check the result of the ac_int division with the native C
    // int division
    if (c != c_golden) {
      passed = false;
      printf("divider failed\n");
    }
    printf("ac_int: %d / %d = %d\n", (int)a, (int)b, (int)c);
    printf("int:    %d / %d = %d\n", t1, t2, c_golden);
  }

  printf("\nBitwise Operations:\n");
  // Shift operator
  {
    t1 = rand() & ((1<<13) - 1);
    t2 = rand() % 8; // Use a small value for the shift
    int14 a = t1;
    int14 b = t2;
    int14 c = shift_left(a, b);
    // Note that the left shift in ac_int is logical so to check the result, we
    // need to do a little bit manipulation to get the correct signed value
    int c_golden = (t1 << t2) & ((1 << 14) - 1);
    if ((t1 << t2) & (1 << 13)) c_golden |= (~((1<<14) -1));
    if (c != c_golden) {
      passed = false;
      printf("left_shift failed\n");
    }
    printf("ac_int: %d << %d = %d\n", (int)a, (int)b, (int)c);
    printf("int:    %d << %d = %d\n", t1, t2, c_golden);
  }

  // Efficient left shift operator
  {
    t1 = rand() & ((1<<13) - 1);
    t2 = rand() % 14; // Use a small value for the shift
    int14 a = t1;
    uint4 b = t2;
    int14 c = effcnt_left_shift(a, b);
    // Note that the left shift in ac_int is logical so to check the result, we
    // need to do a little bit manipulation to get the correct signed value
    int c_golden = (t1 << t2) & ((1 << 14) - 1);
    if ((t1 << t2) & (1 << 13)) c_golden |= (~((1<<14) -1));
    if (c != c_golden) {
      passed = false;
      printf("efficient_left_shift failed\n");
    }
    printf("ac_int: %d << %d = %d\n", (int)a, (int)b, (int)c);
    printf("int:    %d << %d = %d\n", t1, t2, c_golden);
  }

  // Slice operations
  {
    t1 = rand() & ((1 << 13) - 1);
    int14 a = t1;
    uint4 b = get_bit_slice(a, 5);
    // Replicate the same operation using bitwise operation
    t2 = (t1 >> 5) & 0xF;
    // Compare the CPU result with the FPGA result
    if (b != t2) {
      passed = false;
      printf("get_bit_slice failed\n");
    }
    printf("(%d).slc<4>(5) = %d\n", (int)a, (int)b);

    int t2 = 10;
    uint4 d = t2;
    // Sets the bits (3, 2, 1, 0) as 0 and sets bits (9, 8, 7, 6) with d
    int14 c = set_bit_slice(a, d);
    // Replicate the same operation using bitwise operation
    int mask = -1;
    mask ^= (0xF << 6) | (0xF);
    int mask2 = (t2 << 6);
    int t3 = (t1 & mask) | mask2;
    // Compare the CPU result with the FPGA result
    if (c != t3) {
      passed = false;
      printf("set_bit_slice failed\n");
    }
    printf("Running these two ops on %d\n", (int)a);
    printf("\t(%d).set_slc(6, %d)%d\n", (int)a, (int)d, (int)c);
    printf("\ta[3] = 0; a[2] = 0; a[1] = 0; a[0] = 0;\n");
    printf("\tResult = %d\n", (int)c);
  }

  if (passed) printf("PASSED\n");
  else        printf("FAILED\n");

  return 0;
}


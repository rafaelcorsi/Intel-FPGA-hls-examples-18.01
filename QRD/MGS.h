#ifndef MGS_H_
#define MGS_H_

#include "HLS/hls.h"
#include "HLS/ac_int.h"
//////////////////////////////////////////
//           BIT MANIPULATION
//////////////////////////////////////////

// Number of bits needed to express N items, and still have room to roll over.
#define BITS_EXCL(N) (N < 2 ? 1 : (N < 4 ? 2 : (N < 8 ? 3 : (N < 16? 4 : (N < 32? 5: (N < 64? 6 : (N < 128 ? 7 :(N < 256 ? 8 : (N < 512 ? 9: N < 1024? 10: N < 2048 ? 11: -1)))))))))
// Number of bits needed to express N items.
#define BITS_INCL(N) (N <= 2 ? 1 : (N <= 4 ? 2 : (N <= 8 ? 3 : (N <= 16? 4 : (N <= 32? 5: (N <= 64? 6 : (N <= 128 ? 7 :(N <= 256 ? 8 : (N <= 512 ? 9: N <= 1024? 10: N <= 2048 ? 11: -1)))))))))

////////////////////////////////////////////
//          COMPONENT PARAMETERS
////////////////////////////////////////////

// If this macro is defined, the component will use double-precision floating
// point arithmetic, otherwise it will use single-precision arithmetic.
//#define DOUBLE_PRECISION

// Maximal number of rows of A matrix that the component can support.
#ifndef ROWS_COMPONENT
#define ROWS_COMPONENT 8
#endif
// Maximal number of columns of A matrix that the component can support.
#ifndef COLS_COMPONENT
#define COLS_COMPONENT 8
#endif

// Define bit-banking here. Bit-banking is automatically generated for
// matrices of size 4x4, 8x4, 8x8, 16x8, and 64x64 below.
// #define BANKBITS 4,5,6,7

//////////////////////////////////////////////////
//        INFERRED COMPONENT PARAMETERS
// The values of these parameters and macros do
// not need to be set by the user; they follow
// logically from the values that were set above.
//////////////////////////////////////////////////

#if ROWS_COMPONENT < COLS_COMPONENT
#error "QRD requires that ROWS_COMPONENT be >= COLS_COMPONENT. (MGS.h)"
#endif

#ifdef DOUBLE_PRECISION
typedef double QrdFloatingPoint;

// Use double precision floating-point square root if `DOUBLE_PRECISION`
// is defined.
#define SQRT sqrt

#ifdef INTELFPGA_CL
// Use double precision floating-point inverse square root if
// `DOUBLE_PRECISION` is defined.
#define RSQRT rsqrt
#else // G++ can't call functions in extendedmath.h
#define RSQRT 1.0/sqrt
#endif

#else // Single precision (#ifndef DOUBLE_PRECISION)
typedef float QrdFloatingPoint;

// Use single precision floating-point square root if
// `DOUBLE_PRECISION` is not defined.
#define SQRT sqrtf

#ifdef INTELFPGA_CL
// Use single precision floating-point inverse square
// root if `DOUBLE_PRECISION` is not defined.
#define RSQRT rsqrtf
#else // G++ can't call functions in extendedmath.h
#define RSQRT 1.0f/sqrtf
#endif

#endif // precision

// maximal sized R matrix that the component can support
#define R_COMPONENT COLS_COMPONENT

// Bits needed to address each column and allow overflow (e.g. for 8 columns,
// should be 4 bits).
const int BITS_COLS_COMP_ITR = BITS_EXCL(COLS_COMPONENT);

// Bits needed to address each row and allow overflow (e.g. for 8 rows,
// should be 4 bits).
const int BITS_ROWS_COMP_ITR = BITS_EXCL(ROWS_COMPONENT);

// Bits needed to address each column (e.g. for 8 columns, should be 3 bits).
const int BITS_COLS_COMP = BITS_INCL(COLS_COMPONENT);

// Bits needed to address each row (e.g. for 8 rows, should be 3 bits).
const int BITS_ROWS_COMP = BITS_INCL(ROWS_COMPONENT);

// Use `BITS_COLS_COMP_ITR` because without the extra bit, the loop
//      {@code for(uint3 itr = 0; itr < 8; itr++);}
// would never exit.
typedef ac_int<BITS_COLS_COMP_ITR, false> QRD_COL_LOOP;

// use `BITS_ROWS_COMP_ITR` because without the extra bit, the loop
//      {@code for(uint3 itr = 0; itr < 8; itr++);}
// would never exit.
typedef ac_int<BITS_ROWS_COMP_ITR, false> QRD_ROW_LOOP;

const int ROWS_MASK = (1 << BITS_ROWS_COMP) - 1;
const int COLS_MASK = (1 << BITS_COLS_COMP) - 1;

// same as COLS_MASK, since R_COMPONENT = COLS_COMPONENT.
const int R_MASK = (1 << BITS_COLS_COMP) - 1;

// bit banking examples.
#ifndef BANKBITS
#if COLS_COMPONENT == 4 && ROWS_COMPONENT == 4
#define BANKBITS 2,3 // 4x4 (2 bits x 2 bits)

#elif ROWS_COMPONENT == 8 && COLS_COMPONENT == 4
#define BANKBITS 2,3,4 // 8x4 (3 bits x 2 bits)

#elif ROWS_COMPONENT == 8 && COLS_COMPONENT == 8
#define BANKBITS 3,4,5 // 8x8 (3 bits x 3 bits)

#elif ROWS_COMPONENT == 16 && COLS_COMPONENT == 8
#define BANKBITS 3,4,5,6 // 16x8 (4 bits x 3 bits)

#elif ROWS_COMPONENT == 64 && COLS_COMPONENT == 64
#define BANKBITS 6,7,8,9,10,11 // 64x64 (6 bits x 6 bits)

#else
#error "BANKBITS not automatically set for the configured size."
#endif

#endif
/////////////////////////////////////////////////
//            COMPONENT FUNCTION
// The actual HLS component is declared here and
// implemented in MGS.cpp.
/////////////////////////////////////////////////

///
// Calculate the QR factorization using the Modified Gram-Schmidt (MGS)
// algorithm.
//
// @param matrixData Data to perform QRD on. Must contain `rows` * `cols`
// elements, stored in row-major format.
// @param rows Number of rows in `matrixData`
// @param cols Number of columns in `matrixData`
// @param qMatrixStream Resulting Q matrix. Will contain `rows`* `rows`
// elements stored in row-major format.
// @param rMatrixStream Resulting R matrix. Will contain `rows`* `cols`
// elements stored in row-major format.

component void qrd(ihc::stream_in<QrdFloatingPoint> &matrixData,
		hls_stable_argument int rows, hls_stable_argument int cols,
		ihc::stream_out<QrdFloatingPoint> &qMatrixStream,
		ihc::stream_out<QrdFloatingPoint> &rMatrixStream);

#endif // MGS_H_

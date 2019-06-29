#ifndef QRD_TESTBENCH_H_
#define QRD_TESTBENCH_H_

#include "MGS.h"
#include "TestbenchHelpers.h"

//////////////////////////////////////////////////////////
//                  GENERALIZED TESTING
// These functions call the ::qrd() component and compare
// its output with expected results to verify functional
// correctness.
//////////////////////////////////////////////////////////

///
// Invoke the ::qrd() component for a given matrix. Then,perform the following
// battery of tests to verify that Q and R form a valid Q-R factorization:
// 1.   Multiply the Q and R matrices together to verify that they are factors
//      of the input matrix.
// 2.   Verify that the Q matrix is orthonormal (i.e. each column has a
//      Euclidean norm of 1, and is orthogonal to each other column)
// 3.   Verify that the R matrix is upper-triangular.
// 4.   Compare the resulting Q and R factors with pre-calculated matrices.
//
// @param matrix Matrix to factorize.
// @param rows Number of rows in `matrix`.
// @param cols Number of columns in `matrix`.
// @param expected_q_matrix The Q matrix that was returned by ::qrd().
// @param expected_r_matrix The R matrix that should have been returned by
// ::qrd().
// @return QrdTestResult that summarizes whether the parameters represent
// a valid QR factorization.

QrdTestResult testQrd(QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT],
		int rows, int cols,
		QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
		QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT]);

///
// Convenience function that may be called for random matrices whose
// QR factorizations are not known beforehand.
//
// @param matrix Matrix to factorize.
// @param rows Number of rows in `matrix`.
// @param cols Number of columns in `matrix`.
// @return QrdTestResult that summarizes whether the parameters represent
// a valid QR factorization.

QrdTestResult testQrd(QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT],
		int rows, int cols);

///
// Test the ::qrd() component with a randomly generated matrix.
//
// @param matrixRows
// @param matrixCols
// @return Return `true` if the random QR factorization was correctly
// computed, `false` if otherwise.

bool randomMatrixTest(int matrixRows, int matrixCols);

///////////////////////////////////////////////////////////
//                     STATIC TESTS
// These functions test the ::qrd() component using matrices
// whose QR factorizations are known. These functions are
// useful for debugging memory issues.
///////////////////////////////////////////////////////////

///
// Factorize a 3x3 matrix with known Q and R factors.

QrdTestResult qrd_3x3();

///
// Factorize a 4x3 matrix with known Q and R factors.

QrdTestResult qrd_4x3();

///
// Factorize an 8x8 matrix with known Q and R factors.

QrdTestResult qrd_8x8();

#endif // QRD_TESTBENCH_H_

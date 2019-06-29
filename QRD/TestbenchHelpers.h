#ifndef TESTBENCHHELPERS_H_
#define TESTBENCHHELPERS_H_

#include "MGS.h"
#include <stdio.h>

//////////////////////////////////////////////////////
//             QRDTESTRESULT OUTPUT TYPE
// This struct allows QRD tests to return data that
// describe whether the ::qrd() component worked
// correctly. It also includes functions for printing
// this data to the screen nicely.
//////////////////////////////////////////////////////
struct QrdTestResult
{
	const static int FIELD_WIDTH = 16;
	const static int FIELD_WIDTH_WIDE = 20;
	const static int BOOL_WIDTH = 6;

	QrdTestResult() :
			qError(-1.0), rError(-1.0), aError(-1.0), orthogonalError(-1.0), success(
					false), upperTriangular(false), orthogonal(false)
	{
	}

	double qError;
	double rError;
	double aError;
	double orthogonalError;

	bool success;
	bool upperTriangular;
	bool orthogonal;

	///
	// Print labels for each of the columns produced by printFormattedDataline().
	//
	static void printFormattedDataHeader()
	{
		printf(" %-*s| %-*s| %-*s| %-*s| %-*s| %-*s| %-*s\n", FIELD_WIDTH,
				"MATRIX", FIELD_WIDTH, "Q ERROR", FIELD_WIDTH, "R ERROR",
				FIELD_WIDTH, "A ERROR", FIELD_WIDTH, "TRIANGULAR",
				FIELD_WIDTH_WIDE, "ORTHOGONAL (error)", FIELD_WIDTH, "SUCCESS");
	}

	///
	// Format the data stored in this result.
	//
	void printFormattedDataLine(const char *matrixDesc)
	{
		printf(" %-*s| %-*.4e| %-*.4e| %-*.4e| %-*s| %-*s(%-*.4e)| %-*s\n",
				FIELD_WIDTH, matrixDesc, FIELD_WIDTH, qError, FIELD_WIDTH,
				rError, FIELD_WIDTH, aError, FIELD_WIDTH,
				(upperTriangular ? "True" : "False"), BOOL_WIDTH,
				(orthogonal ? "True" : "False"),
				(FIELD_WIDTH_WIDE - BOOL_WIDTH - 2), orthogonalError,
				FIELD_WIDTH, (success ? "True" : "False"));
	}
};

///////////////////////////////////////////////////////////
//                  UTILITY FUNCTIONS
// These convenience functions are called by the ::testQrd()
// functions in QRD_Testbench.cpp.
///////////////////////////////////////////////////////////

///
// Verify that a QR factorization is valid by confirming that `computed_q_matrix`
// and `computed_r_matrix` meet the requirements of a Q and R matrix,
// namely that
// 1.   The Q and R matrices multiply together to form `inputMatrix`.
// 2.   The Q matrix is orthonormal (i.e. each column has a Euclidean norm of
//      1, and is orthogonal to every other column)
// 3.   The R matrix is upper-triangular. (i.e. all entries below the main
//      diagonal are 0)
// 
// @param mResult A data summary of any previous verification, to be considered
// when computing the `success` parameter.
// @param inputMatrix The A matrix that was provided to ::qrd().
// @param rows The number of rows in `inputMatrix`.
// @param cols The number of columns in `inputMatrix`.
// @param computed_q_matrix The Q matrix that was returned by ::qrd().
// @param computed_r_matrix The R matrix that was returned by ::qrd().
// @param epsilon The error threshold to use when evaluating whether two
// floating-point numbers are equivalent.
// @return a `QrdTestResult` that summarizes whether the parameters represent
// a valid QR factorization. When this function returns, this output will have
// had its `qError`, `rError`, and `success` fields set.

QrdTestResult verifyQrd(QrdTestResult mResult,
		QrdFloatingPoint inputMatrix[ROWS_COMPONENT][COLS_COMPONENT], int rows,
		int cols, QrdFloatingPoint computed_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
		QrdFloatingPoint computed_r_matrix[R_COMPONENT][R_COMPONENT],
		QrdFloatingPoint epsilon);


///
// Compare calculated Q and R matrices with pre-computed Q and R matrices.
//
// @param mResult A data summary of any previous verification, to be considered
// when computing the `success` parameter.
// @param rows The number of rows in `inputMatrix`.
// @param cols The number of columns in `inputMatrix`.
// @param computed_r_matrix The R matrix that was returned by ::qrd().
// @param computed_q_matrix The Q matrix that was returned by ::qrd().
// @param expected_r_matrix The R matrix that should have been returned by
// ::qrd().
// @param expected_q_matrix The Q matrix that should have been returned by
// ::qrd().
// @param epsilon The error threshold to use when evaluating whether two
// floating-point numbers are equivalent.
// @return QrdTestResult that summarizes whether the parameters represent
// a valid QR factorization. When this function returns, this output will have
// had its `qError`, `rError`, and `success` fields set.

QrdTestResult compareQRMatrices(QrdTestResult mResult, int rows, int cols,
		QrdFloatingPoint computed_r_matrix[R_COMPONENT][R_COMPONENT],
		QrdFloatingPoint computed_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
		QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT],
		QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
		QrdFloatingPoint epsilon);

///
// Compare two R matrices and return the maximum difference between
// any two corresponding elements.
// 
// @param data1 First matrix
// @param data2 Second matrix
// @param rDimension Matrix dimension (R matrices are square)
// @return the maximum difference between any two corresponding elements of
// `data1` and `data2`.

QrdFloatingPoint maxDifferenceR(
		QrdFloatingPoint data1[R_COMPONENT][R_COMPONENT],
		QrdFloatingPoint data2[R_COMPONENT][R_COMPONENT], int rDimension);

///
// Compare two Q matrices or two A matrices and return the maximum
// difference between any two corresponding elements.
// 
// @param data1 First matrix
// @param data2 Second matrix
// @param rows 
// @param cols
// @return the maximum difference between any two corresponding elements of
// `data1` and `data2`.

QrdFloatingPoint maxDifferenceQA(
		QrdFloatingPoint data1[ROWS_COMPONENT][COLS_COMPONENT],
		QrdFloatingPoint data2[ROWS_COMPONENT][COLS_COMPONENT], int rows,
		int cols);

///
// Print the data stored in a matrix to the screen. The matrix may contain fewer
// elements than it actually has room for (e.g. a 8x8 block of memory that
// actually contains a 3x5 matrix).
//
// @param matrixData Data to print.
// @param maxCols Width of the block of memory (i.e. actual width of the
// `matrixData` buffer).
// @param rows Number of matrix rows to print
// @param cols Number of matrix columns to print
// @param stream Stream to print the matrix to (e.g.`stderr` or `stdout`)

void printMatrix(QrdFloatingPoint matrixData[], int maxCols, int rows, int cols,
		FILE *stream);

///
// Multiply two matrices together using the textbook n^3 algorithm.
// {@code result = q * r}
// 
// @param q First matrix (i.e. left-most). `q` is a rectangular matrix. It
//          should have `rDim` columns.
// @param qRows Number of rows in `q`
// @param r Second matrix (i.e. right-most). It should have `qRows` columns.
// @param rDim Number of rows/columns in `r`.
// @param result Resulting matrix product. It will have dimensions
//               `qRows` X `rDim`.

void qrMatrixMultiply(QrdFloatingPoint q[ROWS_COMPONENT][COLS_COMPONENT], int qRows,
		QrdFloatingPoint r[R_COMPONENT][R_COMPONENT], int rDim,
		QrdFloatingPoint result[ROWS_COMPONENT][COLS_COMPONENT]);

///
// Compute a vector dot product, using one column each from two matrices.
//
// @param rows Number of rows of `aMatrix` and `bMatrix` to use for the dot-
//             product (should be less than or equal to actual number of rows in
//             `aMatrix` and `bMatrix`).
// @param aMatrix First matrix
// @param aCol Column of `aMatrix` to use for the dot-product.
// @param maxACols Width of the block of memory (i.e. actual width of the
//                 `aMatrix` buffer).
// @param bMatrix Second matrix
// @param bCol Column of `bMatrix` to use for the dot-product
// @param maxBCols Width of the block of memory (i.e. actual width of the
//                 `bMatrix` buffer)
// @return the dot product

QrdFloatingPoint dotProductColumn(int rows, QrdFloatingPoint aMatrix[],
		int aCol, int maxACols, QrdFloatingPoint bMatrix[], int bCol,
		int maxBCols);
#endif // TESTBENCHHELPERS_H_ 


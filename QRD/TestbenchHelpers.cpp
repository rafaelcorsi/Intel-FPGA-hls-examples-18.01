#include "TestbenchHelpers.h"
#include "MGS.h"
#include <assert.h>

QrdTestResult verifyQrd(QrdTestResult mResult,
        QrdFloatingPoint inputMatrix[ROWS_COMPONENT][COLS_COMPONENT], int rows,
        int cols,
        QrdFloatingPoint computed_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
        QrdFloatingPoint computed_r_matrix[R_COMPONENT][R_COMPONENT],
        QrdFloatingPoint epsilon)
{
	int rDimension = cols;
	QrdFloatingPoint matrixCalculated[ROWS_COMPONENT][COLS_COMPONENT];

	qrMatrixMultiply(computed_q_matrix, rows, computed_r_matrix, rDimension,
	        matrixCalculated);
	QrdFloatingPoint diffA = maxDifferenceQA(inputMatrix, matrixCalculated,
	        rows, cols);

	mResult.aError = diffA;

	mResult.success = (diffA < epsilon);

	// UPPER TRIANULAR TEST
	mResult.upperTriangular = true;
	for (int row = 0; row < rDimension; row++)
	{
		for (int col = 0; col < rDimension; col++)
		{
			if (col < row)
			{
				mResult.upperTriangular &= (fabs(computed_r_matrix[row][col])
				        < epsilon);
			}

			if (!mResult.upperTriangular)
			{
				break;
			}
		}
	}
	mResult.success &= mResult.upperTriangular;

	// ORTHONORMAL TEST
	mResult.orthogonal = true;
	QrdFloatingPoint dotProduct = 0.0;
	for (int col_1 = 0; col_1 < cols; col_1++)
	{
		for (int col_2 = col_1 + 1; col_2 < cols; col_2++)
		{
			dotProduct = dotProductColumn(rows,
			        (QrdFloatingPoint *) computed_q_matrix, col_1, COLS_COMPONENT,
			        (QrdFloatingPoint *) computed_q_matrix, col_2, COLS_COMPONENT);

#ifdef DOUBLE_PRECISION
			dotProduct = fabs(dotProduct);
#else
			dotProduct = fabsf(dotProduct);
#endif
			if (mResult.orthogonalError < dotProduct)
			{
				mResult.orthogonalError = dotProduct;
			}
			if (fabs(dotProduct) > epsilon)
			{
				mResult.orthogonal = false;
				mResult.success = false;
			}
		}
	}

	return mResult;
}

QrdTestResult compareQRMatrices(QrdTestResult mResult, int rows, int cols,
        QrdFloatingPoint computed_r_matrix[R_COMPONENT][R_COMPONENT],
        QrdFloatingPoint computed_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
        QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT],
        QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
        QrdFloatingPoint epsilon)
{
	int rDimension = cols;
	// Compare computed Q and R matrices with known examples
	QrdFloatingPoint largestDiffQ = -1.0, largestDiffR = -1.0;

	if (expected_q_matrix && expected_r_matrix)
	{
		largestDiffQ = maxDifferenceQA(computed_q_matrix, expected_q_matrix,
		        rows, cols);
		largestDiffR = maxDifferenceR(computed_r_matrix, expected_r_matrix,
		        rDimension);
	}

	mResult.qError = largestDiffQ;
	mResult.rError = largestDiffR;

	mResult.success = mResult.success && (largestDiffQ < epsilon)
	        && (largestDiffR < epsilon);

	return mResult;
}

QrdFloatingPoint maxDifferenceR(
        QrdFloatingPoint data1[R_COMPONENT][R_COMPONENT],
        QrdFloatingPoint data2[R_COMPONENT][R_COMPONENT], int rDimension)
{
	QrdFloatingPoint largestDiscrepancy = 0.0;
	for (int row = 0; row < rDimension; row++)
	{
		for (int col = 0; col < rDimension; col++)
		{
			QrdFloatingPoint data1Val = data1[row][col];
			QrdFloatingPoint data2Val = data2[row][col];
			QrdFloatingPoint diff = (QrdFloatingPoint) fabs(
			        data1Val - data2Val);
			if (diff > largestDiscrepancy)
			{
				largestDiscrepancy = diff;
			}
		}

	}
	return largestDiscrepancy;
}

QrdFloatingPoint maxDifferenceQA(
		QrdFloatingPoint data1[ROWS_COMPONENT][COLS_COMPONENT],
		QrdFloatingPoint data2[ROWS_COMPONENT][COLS_COMPONENT], int rows,
		int cols)
{
	QrdFloatingPoint largestDiscrepancy = 0.0;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			QrdFloatingPoint data1Val = data1[row][col];
			QrdFloatingPoint data2Val = data2[row][col];
			QrdFloatingPoint diff = (QrdFloatingPoint) fabs(
			        data1Val - data2Val);
			if (diff > largestDiscrepancy)
			{
				largestDiscrepancy = diff;
			}
		}

	}
	return largestDiscrepancy;
}

void printMatrix(QrdFloatingPoint matrixData[], int maxCols, int rows, int cols,
        FILE *stream)
{
	printf("\n");
	for (int row = 0; row < rows; row++)
	{

		fprintf(stream, "| ");
		int base = row * maxCols;

		for (int col = 0; col < cols; col++)
		{
			fprintf(stream, "%.2f\t", matrixData[base + col]);
		}

		fprintf(stream, "|\n");
	}
	printf("\n");
}

void qrMatrixMultiply(QrdFloatingPoint q[ROWS_COMPONENT][COLS_COMPONENT], int qRows,
		QrdFloatingPoint r[R_COMPONENT][R_COMPONENT], int rDim,
		QrdFloatingPoint result[ROWS_COMPONENT][COLS_COMPONENT])
{
	for (int rowQ = 0; rowQ < qRows; rowQ++)
	{
		for (int colR = 0; colR < rDim; colR++)
		{
			QrdFloatingPoint sum = 0;
			// multiply current row ROWQ of q by column COLR of r.
			for (int idx = 0; idx < rDim; idx++)
			{
				sum += q[rowQ][idx] * r[idx][colR];
			}

			result[rowQ][colR] = sum;
		}
	}
}

QrdFloatingPoint dotProductColumn(int rows, QrdFloatingPoint aMatrix[],
        int aCol, int maxACols, QrdFloatingPoint bMatrix[], int bCol,
        int maxBCols)
{
	QrdFloatingPoint sum = 0.0;

	for (int row = 0; row < rows; row++)
	{
		sum += aMatrix[row * maxACols + aCol] * bMatrix[row * maxBCols + bCol];
	}

	return sum;
}

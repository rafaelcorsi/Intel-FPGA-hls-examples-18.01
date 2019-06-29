#include "MGS.h"
#include "HLS/math.h"
#include "HLS/extendedmath.h"
#include "HLS/hls.h"

// for debugging output, but only works when compiled with -march="x86-64"
//#include "QRD_Testbench.h"

component void qrd(ihc::stream_in<QrdFloatingPoint> &matrixData,
        hls_stable_argument int rows, hls_stable_argument int cols,
        ihc::stream_out<QrdFloatingPoint> &qMatrixStream,
        ihc::stream_out<QrdFloatingPoint> &rMatrixStream)
{
	hls_memory hls_singlepump hls_bankbits(BANKBITS) hls_bankwidth(sizeof(QrdFloatingPoint))
	QrdFloatingPoint q_matrix[ROWS_COMPONENT][COLS_COMPONENT];

	hls_memory hls_singlepump hls_bankwidth(sizeof(QrdFloatingPoint))
	QrdFloatingPoint r_matrix[R_COMPONENT][R_COMPONENT];

	hls_memory hls_bankbits(BANKBITS) hls_bankwidth(sizeof(QrdFloatingPoint))
	QrdFloatingPoint t_matrix[ROWS_COMPONENT][COLS_COMPONENT];

	hls_register
	QrdFloatingPoint t_matrix_j[ROWS_COMPONENT];

	// zero out elements of t_matrix and q_matrix that may have been written to by
	// a previous invocation to avoid feeding garbage to dot-products.
#pragma unroll 1
	for (int col = 0; col < COLS_COMPONENT; col++)
	{
#pragma unroll
		for (int row = 0; row < ROWS_COMPONENT; row++)
		{
			q_matrix[row][col] = 0.0f;
			t_matrix[row][col] = 0.0f;
		}
	}

#pragma loop_coalesce 2
	for (QRD_ROW_LOOP row = 0; row < rows; row++)
	{
		for (QRD_COL_LOOP col = 0; col < cols; col++)
		{
			t_matrix[row][col] = matrixData.read(); // store in row-major order
		}
	}

	// Main loop of MGS.
	for (QRD_COL_LOOP i = 0; i < cols; i++)
	{
		QrdFloatingPoint t_magnitude_inv = 1.0f;
		// find magnitude of t_(*i) (i-th column)
		QrdFloatingPoint sum = 0;
#pragma unroll
		for (int row = 0; row < ROWS_COMPONENT; row++)
		{
			// hardened dot-product
			QrdFloatingPoint val = t_matrix[row][i & COLS_MASK];
			sum = sum + (val * val);
		}
		t_magnitude_inv = RSQRT(sum);

		// generate Q_(*i) (i-th column)
#pragma unroll
		for (int mRow = 0; mRow < ROWS_COMPONENT; mRow++)
		{
			q_matrix[mRow][i & COLS_MASK] = t_matrix[mRow][i & COLS_MASK]
			        * t_magnitude_inv;
		}
#pragma unroll 1
		for (QRD_COL_LOOP j = 0; j < cols; j++)
		{
			// generate the remaining columns of the i-th row of r_matrix by taking
			// dot-product of i-th column of q_matrix and each successive column of t_matrix.
			QrdFloatingPoint dotProduct = 0.0f;

#pragma unroll
			for (int mRow = 0; mRow < ROWS_COMPONENT; mRow++)
			{
				// hardened dot-product
				dotProduct += q_matrix[mRow][i & COLS_MASK]
				        * t_matrix[mRow][j & COLS_MASK];
			}

			if (j > i)
			{
				r_matrix[i][j & R_MASK] = dotProduct;
			} else if (j < i)
			{
				r_matrix[i][j & R_MASK] = 0.0f; // zero out elements below main diagonal
			} else // if (j == i)
			{
				r_matrix[i][j & R_MASK] = 1.0f / t_magnitude_inv;
			}

#pragma unroll
			for (int mRow = 0; mRow < ROWS_COMPONENT; mRow++)
			{
				// generate the next column of the t-matrix.
				t_matrix_j[mRow] = t_matrix[mRow][j & COLS_MASK]
				        - dotProduct * q_matrix[mRow][i & COLS_MASK];
			}

			if (j > i)
			{
#pragma unroll
				for (int mRow = 0; mRow < ROWS_COMPONENT; mRow++)
				{
					t_matrix[mRow][j & COLS_MASK] = t_matrix_j[mRow];
				}
			}
		}

#if defined(QRD_TESTBENCH_H_)
		printf("i = %d\nT MATRIX:\n", i.to_int());
		printMatrix((QrdFloatingPoint *) t_matrix, COLS_COMPONENT, rows, cols,
				stdout);
		printf("\nQ MATRIX:\n");
		printMatrix((QrdFloatingPoint *) q_matrix, COLS_COMPONENT, rows, rows,
				stdout);
		printf("R MATRIX:\n");
		printMatrix((QrdFloatingPoint *) r_matrix, R_COMPONENT, rows, cols,
				stdout);
		printf("\n\n");
#endif
	}

	// Stream out q_matrix and r_matrix. Merge into one loop to save time and area.
#pragma loop_coalesce 2
	for (QRD_ROW_LOOP row = 0; row < rows; row++)
	{
		for (QRD_COL_LOOP col = 0; col < cols; col++)
		{
			qMatrixStream.write(q_matrix[row][col]);

			// r_matrix has fewer rows than q_matrix
			if (row < cols)
			{
				rMatrixStream.write(r_matrix[row][col]);
			}
		}
	}
}

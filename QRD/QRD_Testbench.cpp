#include "QRD_Testbench.h"
#include "TestbenchHelpers.h"
#include "MGS.h"
#include <assert.h>
#include "stdlib.h"

/////////////////////////////////////////////////////////
//                TESTBENCH CONFIGURATION
// These macros configure how the testbench behaves.
// They may be commented/uncommented to enable or
// adjust testbench functionality.
/////////////////////////////////////////////////////////

// Configure which tests are run
#define RANDOM_TEST      //< test using randomly generated matrices.
#define NUM_TESTS 5      //< number of random tests to run
#define PREDEFINED_TEST  //< run the pre-defined tests with known QR
                         //  Factorizations. Note that this test may fail if
                         //  ROWS_COMPONENT and COLS_COMPONENT in MGS.h are
                         //  smaller than the pre-defined tests.

// Configure how tests are run
#define SHOW_MATRICES   //< for each test, print the input matrix and its
                        //  computed Q-R factors.
#ifndef TEST_ROWS
#define TEST_ROWS 8      //< should be less than or equal to MAX_ROWS defined in
#endif                   //  MGS.h

#ifndef TEST_COLS
#define TEST_COLS 8    //< should be less than or equal to MAX_COLS defined in
#endif                 //  MGS.h

#ifndef EPSILON
#define EPSILON 2e-5    //< threshold past which to fail comparison
#endif

// Configure range of values that may appear in randomly-generated matrices
#define RANDOM_SEED 1138
#define RANDOM_MIN -10
#define RANDOM_MAX 10

// Error out if rows < cols, QRD does not work for this case.
#if TEST_ROWS < TEST_COLS
#error "QRD requires that TEST_ROWS be >= TEST_COLS (QRD_Testbench.cpp)."
#endif

int main()
{
	bool passed = false;
	bool passed_init = false;

#if defined(PREDEFINED_TEST)
	printf("PREDEFINED TEST:\n");
	QrdTestResult mResult8 = qrd_8x8();
	QrdTestResult mResult3 = qrd_3x3();
	QrdTestResult mResult4 = qrd_4x3();
	printf("\nSummary:\n");
	QrdTestResult::printFormattedDataHeader();
	mResult8.printFormattedDataLine("8x8");
	mResult3.printFormattedDataLine("3x3");
	mResult4.printFormattedDataLine("4x3");

	if (!passed_init)
	{
		passed_init = true;
		passed = true;
	}
	passed = passed && mResult3.success && mResult4.success && mResult8.success;

	printf("Done testing pre-defined QR Factorizations.\n");
#endif

#if defined(RANDOM_TEST)
	printf("RANDOM TEST: Testing...\n");
	if (!passed_init)
	{
		passed_init = true;
		passed = true;
	}

	passed &= randomMatrixTest(TEST_ROWS, TEST_COLS);
	printf("Done testing random QR Factorizations.\n");
#endif

	if (passed)
	{
		printf("PASSED\n");
	} else
	{
		printf("FAILED\n");
	}

	return 0;
}

//////////////////////////////////////////////////////////
//                 GENERALIZED TESTING
//////////////////////////////////////////////////////////
QrdTestResult testQrd(QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT],
        int rows, int cols,
        QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT],
        QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT])
{
#ifdef SHOW_MATRICES
	if (NULL != expected_q_matrix && NULL != expected_r_matrix)
	{
		printf("\n--------------------------------------\n");
		printf("QRD TEST: WITH KNOWN FACTORIZATION\nTesting %d x %d matrix:\n", rows, cols);
	} else
	{
		printf("\n--------------------------------------\n");
		printf("QRD TEST: NO KNOWN FACTORIZATION\nTesting %d x %d matrix:\n", rows, cols);
	}
	printMatrix((QrdFloatingPoint*) matrix, COLS_COMPONENT, rows, cols, stdout);
#endif

	int r_dim = cols;

	// fill stream with input data and prepare output streams.
	ihc::stream_out<QrdFloatingPoint> computedQmatrixStream;
	ihc::stream_out<QrdFloatingPoint> computedRmatrixStream;
	ihc::stream_in<QrdFloatingPoint> matrixStream;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			matrixStream.write(matrix[row][col]);
		}
	}

	// invoke the component and collect the results
	qrd(matrixStream, rows, cols, computedQmatrixStream, computedRmatrixStream);
	QrdFloatingPoint computed_r_matrix[R_COMPONENT][R_COMPONENT];
	QrdFloatingPoint computed_q_matrix[ROWS_COMPONENT][COLS_COMPONENT];

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			computed_q_matrix[row][col] = computedQmatrixStream.read();
		}
	}

	for (int row = 0; row < r_dim; row++)
	{
		for (int col = 0; col < r_dim; col++)
		{
			computed_r_matrix[row][col] = computedRmatrixStream.read();
		}
	}

	QrdTestResult mResult;
	mResult = verifyQrd(mResult, matrix, rows, cols, computed_q_matrix,
	        computed_r_matrix, (QrdFloatingPoint) EPSILON);
	mResult = compareQRMatrices(mResult, rows, cols, computed_r_matrix,
	        computed_q_matrix, expected_r_matrix, expected_q_matrix,
	        (QrdFloatingPoint) EPSILON);

#ifdef SHOW_MATRICES
	if (mResult.qError > 0.0)
	{
		printf("Q-Matrix (max discrepancy from known: %.10e):\n",
		        mResult.qError);
	} else
	{
		printf("Q-Matrix (known factorization not provided):\n");
	}
	printMatrix((QrdFloatingPoint*) computed_q_matrix, COLS_COMPONENT, rows,
	        cols, stdout);

	if (mResult.rError > 0.0)
	{
		printf("R-Matrix (max discrepancy from known: %.10e):\n",
		        mResult.rError);
	} else
	{
		printf("R-Matrix (known factorization not provided):\n");
	}
	printMatrix((QrdFloatingPoint*) computed_r_matrix, R_COMPONENT, r_dim,
	        r_dim, stdout);
#endif
	return mResult;
}

QrdTestResult testQrd(QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT],
        int rows, int cols)
{
	return testQrd(matrix, rows, cols, NULL, NULL);
}

bool randomMatrixTest(int matrixRows, int matrixCols)
{
	bool passed = true;
	assert((matrixRows <= ROWS_COMPONENT) && (matrixCols <= COLS_COMPONENT));
	printf("Testing %d random %dx%d matrices...\n", NUM_TESTS, matrixRows,
	        matrixCols);

	QrdTestResult mResult[NUM_TESTS];
	srand(RANDOM_SEED);

	for (int currTest = 0; currTest < NUM_TESTS; currTest++)
	{
		int size = matrixRows * matrixCols;
		QrdFloatingPoint matrixA[ROWS_COMPONENT][COLS_COMPONENT];
		for (int row = 0; row < matrixRows; row++)
		{
			for (int col = 0; col < matrixCols; col++)
			{
				int randomVal = rand();
				QrdFloatingPoint randomDouble = (randomVal - RANDOM_MIN)
				        % RANDOM_MAX;
				matrixA[row][col] = randomDouble;
				// Don't initialize all values; only care about values in the
				// range (0, 0) to (matrixRows, matrixCols).
				// The rest of the buffer will be "don't care"s
			}
		}
		mResult[currTest] = testQrd(matrixA, matrixRows, matrixCols);
	}

	printf("\nSummary:\n");
	QrdTestResult::printFormattedDataHeader();
	for (int currTest = 0; currTest < NUM_TESTS; currTest++)
	{
		char matrixDesc[50];
		sprintf(matrixDesc, "Matrix %d", currTest);

		mResult[currTest].printFormattedDataLine(matrixDesc);
		passed &= mResult[currTest].success;
	}

	return passed;
}

//////////////////////////////////////////
//             STATIC TESTS
//////////////////////////////////////////
QrdTestResult qrd_3x3()
{
	int rows = 3, cols = 3, r_dim = cols;

#if (!(ROWS_COMPONENT < 3)) && (!(COLS_COMPONENT < 3))
	double matrixDouble[ROWS_COMPONENT][COLS_COMPONENT] =
	{
	{ 12.0, -51.0, 4.0 },
	{ 6.0, 167.0, -68.0 },
	{ -4.0, 24.0, -41.0 } };

	double expected_q_matrixDouble[ROWS_COMPONENT][COLS_COMPONENT] =
	{
	{ (6.0 / 7.0), (-69.0 / 175.0), (-58.0 / 175.0) },
	{ (3.0 / 7.0), (158.0 / 175.0), (6.0 / 175.0) },
	{ (-2.0 / 7.0), (6.0 / 35.0), (-33.0 / 35.0) } };

	double expected_r_matrixDouble[R_COMPONENT][R_COMPONENT] =
	{
	{ (14.0), (21.0), (-14.0) },
	{ (0.0), (175.0), (-70.0) },
	{ (0.0), (0.0), (35.0) } };

	QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			matrix[row][col] = (QrdFloatingPoint) matrixDouble[row][col];
		}
	}
	QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			expected_q_matrix[row][col] =
			        (QrdFloatingPoint) expected_q_matrixDouble[row][col];
		}
	}

	QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			expected_r_matrix[row][col] =
			        (QrdFloatingPoint) expected_r_matrixDouble[row][col];
		}
	}

	return testQrd(matrix, rows, cols, expected_q_matrix, expected_r_matrix);
#else
	QrdTestResult mResult;
	mResult.success = true;
	return mResult;
#endif  
}

QrdTestResult qrd_4x3()
{
	int rows = 4, cols = 3, r_dim = cols;

#if (!(ROWS_COMPONENT < 4)) && (!(COLS_COMPONENT < 3))
	double matrixDouble[ROWS_COMPONENT][COLS_COMPONENT] =
	{
	{ 1.0, -1.0, -1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, -1.0, 0.0 },
	{ 0.0, 1.0, -1.0 } };

	double expected_q_matrixDouble[ROWS_COMPONENT][COLS_COMPONENT] =
	{
	{ 1.0 / sqrt(3.0), -1.0 / sqrt(15.0), -4.0 / sqrt(35.0) },
	{ 1.0 / sqrt(3.0), 2.0 / sqrt(15.0), 3.0 / sqrt(35.0) },
	{ 1.0 / sqrt(3.0), -1.0 / sqrt(15.0), 1.0 / sqrt(35.0) },
	{ 0.0, 3.0 / sqrt(15.0), -3.0 / sqrt(35.0) } };

	double expected_r_matrixDouble[R_COMPONENT][R_COMPONENT] =
	{
	{ 3.0 / sqrt(3.0), -2.0 / sqrt(3.0), -1.0 / sqrt(3.0) },
	{ 0, 5.0 / sqrt(15.0), -2.0 / sqrt(15.0) },
	{ 0.0, 0.0, 7.0 / sqrt(35.0) } };
	
	QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			matrix[row][col] = (QrdFloatingPoint) matrixDouble[row][col];
		}
	}
	QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			expected_q_matrix[row][col] =
			        (QrdFloatingPoint) expected_q_matrixDouble[row][col];
		}
	}

	QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT];
	for (int row = 0; row < r_dim; row++)
	{
		for (int col = 0; col < r_dim; col++)
		{
			expected_r_matrix[row][col] =
			        (QrdFloatingPoint) expected_r_matrixDouble[row][col];
		}
	}

	return testQrd(matrix, rows, cols, expected_q_matrix, expected_r_matrix);
#else
	QrdTestResult mResult;
	mResult.success = true;
	return mResult;
#endif    
}

QrdTestResult qrd_8x8()
{
	int rows = 8, cols = 8, r_dim = cols;

#if (!(ROWS_COMPONENT < 8)) && (!(COLS_COMPONENT < 8)) 
	double matrixDouble[ROWS_COMPONENT][COLS_COMPONENT] =
	{
	{ 7.0, 8.0, 4.0, 6.0, 3.0, 4.0, 6.0, 8.0 },
	{ 8.0, 8.0, 8.0, 7.0, 1.0, 4.0, 7.0, 3.0 },
	{ 2.0, 2.0, 7.0, 6.0, 1.0, 7.0, 3.0, 5.0 },
	{ 8.0, 8.0, 8.0, 4.0, 7.0, 7.0, 6.0, 2.0 },
	{ 6.0, 8.0, 6.0, 6.0, 6.0, 2.0, 6.0, 7.0 },
	{ 1.0, 4.0, 1.0, 2.0, 3.0, 4.0, 2.0, 3.0 },
	{ 3.0, 7.0, 7.0, 6.0, 8.0, 4.0, 1.0, 5.0 },
	{ 5.0, 2.0, 8.0, 1.0, 1.0, 6.0, 4.0, 6.0 }, };

	double expected_r_matrixDouble[R_COMPONENT][R_COMPONENT] =
	{
	{ 15.874507866, 17.071395364, 16.882413128, 12.787798003, 9.764082219, 11.842886821, 13.417738792, 12.346839452, },
	{ 0.000000000, 6.129229994, 0.455791477, 4.844726740, 6.740404929, 0.624123557, -0.009711466, 2.973003500, },
	{ 0.000000000, 0.000000000, 7.601077628, 3.144675942, 2.379466692, 5.891141237, -0.068330195, 3.315383309, },
	{ 0.000000000, 0.000000000, 0.000000000, 4.136648191, -3.625926614, -0.240262794, 1.372752075, 2.002083582, },
	{ 0.000000000, 0.000000000, 0.000000000, 0.000000000, 3.228069741, -0.226363060, -0.559578319, 0.550099946, },
	{ 0.000000000, 0.000000000, 0.000000000, 0.000000000, 0.000000000, 5.151891886, 0.913644070, 1.552131452, },
	{ 0.000000000, 0.000000000, 0.000000000, 0.000000000, 0.000000000, 0.000000000, 1.981716616, 1.882262192, },
	{ 0.000000000, 0.000000000, 0.000000000, 0.000000000, 0.000000000, 0.000000000, 0.000000000, 6.201779393, }, };

	double expected_q_matrixDouble[ROWS_COMPONENT][COLS_COMPONENT] =
	{
	{ 0.440958552, 0.077044298, -0.457772009, 0.345061291, 0.159709946, 0.299996590, -0.305594445, 0.511960974 },
	{ 0.503952631, -0.098409524, -0.060923241, 0.295869167, -0.631817258, -0.314420531, -0.120835218, -0.363926694 },
	{ 0.125988158, -0.024602381, 0.642570655, 0.601309105, 0.181841950, 0.373348568, 0.145525675, -0.124171860 },
	{ 0.503952631, -0.098409524, -0.060923241, -0.429355667, 0.412270461, 0.279943329, -0.102282751, -0.538044998 },
	{ 0.377964473, 0.252498120, -0.065259019, 0.035922983, 0.276673082, -0.422771077, 0.715709591, 0.142529856 },
	{ 0.062994079, 0.477156703, -0.036965288, -0.241984693, -0.502086371, 0.582725256, 0.340964400, 0.022672274 },
	{ 0.188982237, 0.615706953, 0.464261780, -0.207789354, 0.045390387, -0.271172909, -0.474141206, 0.161463089 },
	{ 0.314970394, -0.550963845, 0.385953736, -0.380068079, -0.203880486, 0.039312856, 0.084051037, 0.503629954 }, };

	QrdFloatingPoint matrix[ROWS_COMPONENT][COLS_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			matrix[row][col] = (QrdFloatingPoint) matrixDouble[row][col];
		}
	}
	QrdFloatingPoint expected_q_matrix[ROWS_COMPONENT][COLS_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			expected_q_matrix[row][col] =
			(QrdFloatingPoint) expected_q_matrixDouble[row][col];
		}
	}

	QrdFloatingPoint expected_r_matrix[R_COMPONENT][R_COMPONENT];
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			expected_r_matrix[row][col] =
			(QrdFloatingPoint) expected_r_matrixDouble[row][col];
		}
	}

	return testQrd(matrix, rows, cols, expected_q_matrix,
			expected_r_matrix);
#else
	QrdTestResult mResult;
	mResult.success = true;
	return mResult;
#endif
}

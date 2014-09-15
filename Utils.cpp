#include "Utils.h"
extern m_random r;
extern m_timer timer;

/*
	Function: Generate a random double matrix
*/
double* gen_dmat(int rows, int cols, double start, double end) {
	double *mat = new double[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = r.next_double(start, end);
	}
	return mat;
}
double* gen_dmat(int rows, int cols) {
	double *mat = new double[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = r.next_double();
	}
	return mat;
}
int* gen_imat(int rows, int cols, int start, int end) {
	int *mat = new int[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = r.next_int(start, end);
	}
	return mat;
}
int *gen_imat(int rows, int cols) {
	int *mat = new int[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = r.next_int();
	}
	return mat;
}

/*
	Function: normalize the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   inplace --> whether normalize in the given matrix or create a new one
			   horizontal --> normalize the matrix horizontally or vertically (default true)
*/
double* mat_normalize(double *mat, int rows, int cols, bool inplace, bool horizontal) {
	double *mat_t, tot;
	if (inplace) {
		mat_t = mat;
	} else {
		mat_t = new double[rows*cols];
		memcpy(mat_t, mat, sizeof(double)*rows*cols);
	}
	if (horizontal) {
		for (int i = 0; i < rows; i++) {
			tot = (double)0;
			// accumulate horizontally
			for (int j = 0; j < cols; j++) tot += mat_t[i*cols + j];
			if (tot > 0)
				for (int j = 0; j < cols; j++) mat_t[i*cols + j] /= tot;
		}
	} else {
		for (int j = 0; j < cols; j++) {
			tot = (double)0;
			// accumulate vertically
			for (int i = 0; i < rows; i++) tot += mat_t[i*cols + j];
			if (tot > 0)
				for (int i = 0; i < rows; i++) mat_t[i*cols + j] /= tot;
		}
	}

	return mat_t;
}

/*
	Function: Scale the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   inplace --> whether scale in the given matrix or create a new one
			   start, end --> scale range
			   horizontal --> scale the matrix horizontally or vertically (default false)
*/
double* mat_scale(double* mat, int rows, int cols, bool inplace, double start, double end, bool horizontal) {
	double* mat_t, *max_vec, *min_vec;
	// check arguments
	if (start > end) {
		std::cout << "`end` must larger than `start`" << std::endl;
		return NULL;
	}
	
	if (inplace) {
		mat_t = mat;
	} else {
		mat_t = new double[rows*cols];
		memcpy(mat_t, mat, sizeof(double)*rows*cols);
	}

	if (horizontal) {
		max_vec = mat_max(mat_t, rows, cols, true);
		min_vec = mat_min(mat_t, rows, cols, true);
		for (int i = 0; i < rows; i++) {
			if (max_vec[i] > min_vec[i]) {
				for (int j = 0; j < cols; j++) {
					mat_t[i*cols + j] = (mat_t[i*cols + j] - min_vec[i]) / (max_vec[i] - min_vec[i]) * (end - start) + start;
				}
			} else {
				for (int j = 0; j < cols; j++) {
					// if all elements in a row is same, then we scale it to (start+end)/2
					mat_t[i*cols + j] = (start + end) / 2;	
				}
			}
		}
	} else {
		max_vec = mat_max(mat_t, rows, cols, false);
		min_vec = mat_min(mat_t, rows, cols, false);
		for (int j = 0; j < cols; j++) {
			if (max_vec[j] > min_vec[j]) {
				for (int i = 0; i < rows; i++) {
					mat_t[i*cols + j] = (mat_t[i*cols + j] - min_vec[j]) / (max_vec[i] - min_vec[j]) * (end - start) + start;
				}
			}
			else {
				for (int i = 0; i < rows; i++) {
					// if all elements in a column is same, then we scale it to (start+end)/2
					mat_t[i*cols + j] = (start + end) / 2;
				}
			}
		}
	}

	delete[] max_vec;
	delete[] min_vec;
	return mat_t;
}
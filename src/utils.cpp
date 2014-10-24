#include "utils.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include "random.h"

/*
	Function: Randomly sample m instances from n instances, return m*cols matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   m --> size of instances to be sample from matrix
*/
int* random_sample(int size, int m, int *idx) {
	int tmp, instance, u, end;
	idx = ordered_sequence<int>(size, idx);
	if (m > size) {
		std::cerr << "m must less than the total instances" << std::endl;
		return NULL;
	} else if (m == size) {
		return idx;
	}

	// we just need to sample the small part
	if (m * 2 <= size) {
		end = m;	// use 0~m
		u = 0;
	} else {
		end = size - m;		// use size-m ~ size
		u = size - m;
	}
	// sample index
	for (int i = 0; i < end; i++) {
		instance = m_random::getInstance().next_int(i, size);
		tmp = idx[instance]; idx[instance] = idx[i]; idx[i] = tmp;
	}
	end = u + m;
	// sort the index to return to original order
	std::sort(idx + u, idx + end);
	return idx;
}

/*
	Function: Generate a random double matrix
*/
double* gen_dmat(int rows, int cols, double start, double end) {
	double *mat = new double[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = m_random::getInstance().next_double(start, end);
	}
	return mat;
}
double* gen_dmat(int rows, int cols) {
	double *mat = new double[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = m_random::getInstance().next_double();
	}
	return mat;
}
double* gen_dvec(int size) {
	return gen_dmat(1, size);
}
double* gen_dvec(int size, double start, double end) {
	return gen_dmat(1, size, start, end);
}
double* gen_dzeros(int rows, int cols) {
	double* mat = new double[rows*cols];
	memset(mat, 0, sizeof(double)*rows*cols);
	return mat;
}
double* gen_dzeros(int size) {
	return gen_dzeros(1, size);
}
double* gen_dones(int rows, int cols) {
	double* mat = new double[rows*cols];
	for (int i = 0; i < rows*cols; i++) mat[i] = 1.0;
	return mat;
}
double* gen_dones(int size) {
	return gen_dones(1, size);
}
int* gen_imat(int rows, int cols, int start, int end) {
	int *mat = new int[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = m_random::getInstance().next_int(start, end);
	}
	return mat;
}
int *gen_imat(int rows, int cols) {
	int *mat = new int[rows*cols];
	for (int i = 0; i < rows*cols; i++) {
		mat[i] = m_random::getInstance().next_int();
	}
	return mat;
}
int* gen_ivec(int size) {
	return gen_imat(1, size);
}
int* gen_ivec(int size, int start, int end) {
	return gen_imat(1, size, start, end);
}
int* gen_izeros(int rows, int cols) {
	int* mat = new int[rows*cols];
	memset(mat, 0, sizeof(int)*rows*cols);
	return mat;
}
int* gen_izeros(int size) {
	return gen_izeros(1, size);
}
int* gen_iones(int rows, int cols) {
	int* mat = new int[rows*cols];
	for (int i = 0; i < rows*cols; i++) mat[i] = 1;
	return mat;
}
int* gen_iones(int size) {
	return gen_iones(1, size);
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
double *vec_normalize(double *vec, int size, bool inplace) {
	return mat_normalize(vec, 1, size, inplace, HORIZONTAL);
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
		std::cerr << "`end` must larger than `start`" << std::endl;
		exit(EXIT_FAILURE);
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


int* double2int(double* val, int size) {
	int* val_i = new int[size];
	for (int i = 0; i < size; i++) val_i[i] = (int) val[i];
	return val_i;
}


bool is_number(std::string str) {
	bool has_dot = false;	
	if (!(str[0] == '+' || str[0] == '-' || isdigit(str[0]))) {
		if (str[0] == '.')
			has_dot = true;
		else
			return false;
	}
	for (int i = 1; i < str.length(); i++) {
		if (str[i] == '.') {
			if (has_dot) return false;
			has_dot = true;	
		} else if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

std::string color_msg(std::string msg, std::string color) {
	std::string control_str;
	if (color == "black") control_str = "\033[;30m";
	else if (color == "red") control_str = "\033[;31m";
	else if (color == "green") control_str = "\033[;32m";
	else if (color == "yellow") control_str = "\033[;33m";
	else if (color == "blue") control_str = "\033[;34m";
	else if (color == "purple") control_str = "\033[;35m";
	else if (color == "dark green") control_str = "\033[;36m";
	else if (color == "white") control_str = "\033[;37m";
	else return NULL;
	return control_str + msg;

}

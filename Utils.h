#ifndef __UTILS
#define __UTILS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <algorithm>

#define HORIZONTAL 1
#define VERTICAL 0
#define ALL -1
#define INPLACE true
#define NOT_INPLACE false

/*
	Class: record elapsed time
*/
class m_timer {
	private:
		clock_t tic_time;
		bool is_tic;
	public:
		m_timer() {
			is_tic = false;
		}
		void tic() {
			tic_time = clock();
			is_tic = true;
		}
		void toc(char* msg = NULL) {
			if (msg != NULL) {
				std::cout << std::string(msg) << std::endl;
			}
			if (!is_tic) {
				std::cout << "please call `tic` first." << std::endl;
			} else {
				std::cout << "Time elapsed:"
					<< ((double)(clock() - tic_time)) / CLOCKS_PER_SEC
					<< std::endl;
			}
			is_tic = false;
		}
};

class m_random {
	public:
		m_random() {
			srand((unsigned int)time(NULL));
		}

		~m_random() { }
		
		// return a random integer
		int next_int() {
			return rand();
		}
		// return a random integer between `begin` and `end` ( [begin, end) )
		int next_int(int begin, int end) {
			if (begin > end) {
				std::cout << "In `next_int`, `end` must larger than `begin`." << std::endl;
				exit(1);
			}
			return rand() % (end - begin) + begin;
		}
		// return a real number between 0 and 1 ( [0, 1] )
		double next_double() {
			return (double)rand() / RAND_MAX;
		}
		// return a real number between `begin` and `end` ( [begin, end] )
		double next_double(double begin, double end) {
			if (begin > end) {
				std::cout << "In `next_int`, `end` must larger than `begin`." << std::endl;
				exit(1);
			}
			return (double)rand() / RAND_MAX * (end - begin) + begin;
		}

};
static m_random r;
static m_timer timer;
/*
	Function: generate a order sequence
	Arguments: size --> the sequence's size
			   start_from --> the number sequence start from, default is 0
*/
int* ordered_sequence(int size, int start_from = 0) {
	int *idx = new int[size];
	for (int i = 0; i < size; i++) idx[i] = i + start_from;
	return idx;
}

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
	Function: print an vector
*/
template <class T>
void print_vec(T *vec, int size, char* msg = NULL) {
	if (msg != NULL) {
		std::cout << std::string(msg) << ":" << std::endl;
	}
	for (int i = 0; i < size; i++) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}
/*
	Function: print a matrix
*/
template <class T>
void print_mat(T* mat, int rows, int cols, char* msg = NULL) {
	if (msg != NULL) {
		std::cout << std::string(msg) << ":" << std::endl;
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << mat[i*cols + j] << " ";
		}
		std::cout << std::endl;
	}
}


/*
	Function: Sort array and return index in order
	Arguments: arr --> array to be sorted;
			   size --> array size;
			   asc --> sort int ascent order (asc=1) or descent order (asc=-1)
*/
template <class T>
int* argsort(T *arr, int size, int asc = 1) {
	int st_head = 0, u, v, tmp, *m_stack = new int[size*2], from, to;
	// get an ordered sequence start from 0
	int *idx = ordered_sequence(size);
	
	// check argument
	if (asc != 1 && asc != -1) {
		std::cout << "The third arguments must be +1 or -1. +1 means ascent, -1 means descent." << std::endl;
	}

	// if array size is too small, just return idx
	if (size <= 1) return idx;

	// quick sort
	m_stack[st_head++] = 0;
	m_stack[st_head++] = size;
	while (st_head != 0) {
		from = m_stack[st_head - 2];
		to = m_stack[st_head - 1];
		st_head -= 2;

		if (to - from <= 1) continue;
		u = from + 1;
		v = to - 1;
		while (u <= v) {
			while (u <= v && asc*arr[idx[u]] <= asc*arr[idx[from]]) u++;
			while (u <= v && asc*arr[idx[v]] >= asc*arr[idx[from]]) v--;
			if (u <= v) 
				tmp = idx[u]; idx[u] = idx[v]; idx[v] = tmp;
			
		}
		tmp = idx[from]; idx[from] = idx[v]; idx[v] = tmp;
		
		// push left part to stack
		m_stack[st_head++] = from;
		m_stack[st_head++] = v;
		// push right part to stack
		m_stack[st_head++] = u;
		m_stack[st_head++] = to;
	}

	delete[] m_stack;
	return idx;
}

/*
	Function: Randomly sample m instances from n instances, return m*cols matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   m --> size of instances to be sample from matrix
*/
template <class T>
T* random_sample(T *mat, int rows, int cols, int m) {
	int *idx = ordered_sequence(rows), tmp, instance, u, end;
	T* ret;
	if (m > rows) {
		std::cout << "m must less than the total instances" << std::endl;
		return NULL;
	}

	// we just need to sample the small part
	if (m * 2 <= rows) {
		end = m;	// use 0~m
		u = 0;
	} else {
		end = rows - m;		// use rows-m ~ rows
		u = rows - m;
	}
	// sample index
	for (int i = 0; i < end; i++) {
		instance = r.next_int(i, rows);
		tmp = idx[instance]; idx[instance] = idx[i]; idx[i] = tmp;
	}
	end = u + m;
	// sort the index to return to original order
	std::sort(idx + u, idx + end);
	ret = new T[m * cols];
	// copy corresponding columns to new matrix
	for (int i = u; i < end; i++) {
		memcpy(ret + (i - u) * cols, mat + idx[i] * cols, sizeof(T)*cols);
	}

	return ret;
}





/*
	Function: return max vector of a matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   horizontal --> calculate the max vector horizontally or vertically
*/
template <class T>
T* mat_max(T *mat, int rows, int cols, bool horizontal = true) {
	T* max_vec;
	if (horizontal) {
		max_vec = new T[rows];
		// initialize max_vec using first column
		for (int i = 0; i < rows; i++) max_vec[i] = mat[i*cols + 0];
		for (int j = 1; j < cols; j++) {
			for (int i = 0; i < rows; i++) {
				max_vec[i] = max_vec[i] > mat[i*cols + j] ? max_vec[i] : mat[i*cols + j];
			}
		}
	} else {
		max_vec = new T[cols];
		// initialize max_vec using first row
		for (int j = 0; j < cols; j++) max_vec[j] = mat[0*cols + j];
		for (int i = 1; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				max_vec[j] = max_vec[j] > mat[i*cols + j] ? max_vec[j] : mat[i*cols + j];
			}
		}
	}
	return max_vec;
}

/*
	Function: return min vector of a matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   horizontal --> calculate the min vector horizontally or vertically
*/
template <class T>
T* mat_min(T *mat, int rows, int cols, bool horizontal = true) {
	T* min_vec;
	if (horizontal) {
		min_vec = new T[rows];
		// initialize min_vec using first column
		for (int i = 0; i < rows; i++) min_vec[i] = mat[i*cols + 0];
		for (int j = 1; j < cols; j++) {
			for (int i = 0; i < rows; i++) {
				min_vec[i] = min_vec[i] < mat[i*cols + j] ? min_vec[i] : mat[i*cols + j];
			}
		}
	} else {
		min_vec = new T[cols];
		// initialize min_vec using first row
		for (int j = 0; j < cols; j++) min_vec[j] = mat[0 * cols + j];
		for (int i = 1; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				min_vec[j] = min_vec[j] < mat[i*cols + j] ? min_vec[j] : mat[i*cols + j];
			}
		}
	}
	return min_vec;
}


/*
	Function: normalize the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   inplace --> whether normalize in the given matrix or create a new one
			   horizontal --> normalize the matrix horizontally or vertically (default true)
*/
double* mat_normalize(double *mat, int rows, int cols, bool inplace, bool horizontal = true) {
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
double* mat_scale(double* mat, int rows, int cols, bool inplace, double start, double end, bool horizontal = false) {
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

/*
	Function: accumulate the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   horizontal --> accumulate the matrix horizontally or vertically or accumulate the whole matrix
*/
template <class T>
T* mat_accumulate(T *mat, int rows, int cols, int horizontal = HORIZONTAL) {
	T* accu_vec;
	if (horizontal == HORIZONTAL) {
		accu_vec = new T[rows];
		for (int i = 0; i < rows; i++) {
			accu_vec[i] = (T)0;
			for (int j = 0; j < cols; j++) {
				accu_vec[i] += mat[i*cols + j];	
			}
		}
	} else if (horizontal == VERTICAL) {
		accu_vec = new T[cols];
		for (int j = 0; j < cols; j++) {
			accu_vec[j] = (T)0;
			for (int i = 0; i < rows; i++) {
				accu_vec[j] += mat[i*cols + j];	
			}
		}
	} else if (horizontal == ALL) {
		accu_vec = new T;
		*accu_vec = (T)0;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				*accu_vec += mat[i*cols + j];
			}
		}
	} else {
		std::cout << "function mat_accumulate: invalid horizontal argument. must be `HORIZONTAL`, `VERTICAL` or `ALL`" << std::endl;
		exit(1);
	}
	return accu_vec;	
}

#endif

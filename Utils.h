#ifndef __UTILS
#define __UTILS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>

#define eps 1e-5
#define HORIZONTAL 1
#define VERTICAL 0
#define ALL -1
#define INPLACE true
#define NOT_INPLACE false
#define ASC 1
#define DESC -1
#define INF 0x7fffffff

#define zero(a) ((a) < eps && (a) > -eps)

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
				std::cout << "Time elapsed: "
					<< ((double)(clock() - tic_time)) / CLOCKS_PER_SEC
					<< "s" 
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
template <class T>
T* ordered_sequence(T size, T *idx = NULL, T start_from = 0) {
	// T *idx = new T[size];
	if (idx == NULL) {
		idx = new T[size];
	}
	for (T i = 0; i < size; i++) idx[i] = i + start_from;
	return idx;
}

/*
	Function: Generate a random double matrix
*/
double* gen_dmat(int rows, int cols, double start, double end);
double* gen_dmat(int rows, int cols);
int* gen_imat(int rows, int cols, int start, int end);
int *gen_imat(int rows, int cols);

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
int* argsort(T *arr, int size, int asc = ASC) {
	int st_head = 0, u, v, tmp, *m_stack = new int[size*2], from, to;
	// get an ordered sequence start from 0
	int *idx = ordered_sequence<int>(size);
	
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
	Function: Sort one column of a matrix and return index in order
	Arguments: mat --> matrix to be sorted;
			   rows, cols --> shape of the matrix;
			   target --> column index to be sorted;
			   asc --> sort int ascent order (asc=1) or descent order (asc=-1).
*/
template <class T>
int* argsort(T *mat, int rows, int cols, int target, int asc = ASC, int *idx = NULL) {
	int st_head = 0, u, v, tmp, *m_stack = new int[rows*2], from, to;
	// get an ordered sequence start from 0
	if (idx == NULL)
		idx = ordered_sequence<int>(rows);				// need to allocate space
	else
		idx = ordered_sequence<int>(rows, idx);			// do not need to allocate space
	
	// check argument
	if (asc != 1 && asc != -1) {
		std::cout << "The third arguments must be +1 or -1. +1 means ascent, -1 means descent." << std::endl;
	}

	// if array size is too small, just return idx
	if (rows <= 1) return idx;

	// quick sort
	m_stack[st_head++] = 0;
	m_stack[st_head++] = rows;
	while (st_head != 0) {
		from = m_stack[st_head - 2];
		to = m_stack[st_head - 1];
		st_head -= 2;

		if (to - from <= 1) continue;
		u = from + 1;
		v = to - 1;
		while (u <= v) {
			while (u <= v && asc*mat[idx[u]*cols + target] <= asc*mat[idx[from]*cols + target]) u++;
			while (u <= v && asc*mat[idx[v]*cols + target] >= asc*mat[idx[from]*cols + target]) v--;
			if (u <= v) {
				tmp = idx[u]; idx[u] = idx[v]; idx[v] = tmp;
			}
			
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
	Function: Sort one column of a part of matrix(specify some rows) and return index in order
	Arguments: mat --> matrix to be sorted;
			   rows, cols --> shape of the matrix;
			   target --> column index to be sorted;
			   asc --> sort int ascent order (asc=1) or descent order (asc=-1)
			   idx --> order index array
			   active_row --> row index to sort
			   active_row_size --> row size to sort
*/
template <class T>
int* partial_argsort(T *mat, int rows, int cols, int *active_row, int active_row_size, 
					int target, int asc = ASC, int *idx = NULL) {
	int st_head = 0, u, v, tmp, *m_stack = new int[rows*2], from, to;
	
	// get an ordered sequence start from 0
	idx = ordered_sequence<int>(active_row_size, idx);			// do not need to allocate space
	
	// check argument
	if (asc != 1 && asc != -1) {
		std::cout << "The third arguments must be +1 or -1. +1 means ascent, -1 means descent." << std::endl;
	}

	// if array size is too small, just return idx
	if (rows <= 1) return idx;

	// quick sort
	m_stack[st_head++] = 0;
	m_stack[st_head++] = rows;
	while (st_head != 0) {
		from = m_stack[st_head - 2];
		to = m_stack[st_head - 1];
		st_head -= 2;

		if (to - from <= 1) continue;
		u = from + 1;
		v = to - 1;
		while (u <= v) {
			while (u <= v && asc*mat[active_row[idx[u]]*cols + target] <= asc*mat[active_row[idx[from]]*cols + target]) u++;
			while (u <= v && asc*mat[active_row[idx[v]]*cols + target] >= asc*mat[active_row[idx[from]]*cols + target]) v--;
			if (u <= v) {
				tmp = idx[u]; idx[u] = idx[v]; idx[v] = tmp;
			}
		}
		tmp = idx[from]; idx[from] = idx[v]; idx[v] = tmp;
		
		// push left part to stack
		m_stack[st_head++] = from;
		m_stack[st_head++] = v;
		// push right part to stack
		m_stack[st_head++] = u;
		m_stack[st_head++] = to;
	}

	for (int i = 0; i < active_row_size; i++) {
		idx[i] = active_row[idx[i]];
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
T* random_sample(T *mat, int rows, int cols, int m, T *ret = NULL) {
	int *idx = ordered_sequence(rows), tmp, instance, u, end;
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
	if(ret == NULL)
		ret = new T[m * cols];
	// copy corresponding rows to new matrix
	for (int i = u; i < end; i++) {
		memcpy(ret + (i - u) * cols, mat + idx[i] * cols, sizeof(T)*cols);
	}

	return ret;
}

int* random_sample(int size, int m, int *idx = NULL); 

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
double* mat_normalize(double *mat, int rows, int cols, bool inplace, bool horizontal = true);
double* vec_normalize(double *vec, int size, bool inplace);

/*
	Function: Scale the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   inplace --> whether scale in the given matrix or create a new one
			   start, end --> scale range
			   horizontal --> scale the matrix horizontally or vertically (default false)
*/
double* mat_scale(double* mat, int rows, int cols, bool inplace, double start, double end, bool horizontal = false);

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

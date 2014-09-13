#ifndef __PARALLEL
#define __PARALLEL

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <thread>
#include <algorithm>
#include <vector>

struct parallel_unit{
	unsigned long const num_threads;	// launch `num_threads` threads
	unsigned long const block_size;		// size of block for each thread to handle
	// constructor
	parallel_unit(int num_threads, int block_size): num_threads(num_threads), block_size(block_size) {}
};

/*
	Function: Initalize the parallel setting
	Arguments: length --> the length to be paralleled
			   min_per_thread -> the minimum length each thread deal with 
*/
struct parallel_unit init_block(int length, unsigned long const min_per_thread = 100) {
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;
	struct parallel_unit pu(num_threads, block_size);
	return pu;
}


/*
	Function: do max operation in a fraction of the total dataset
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   block_start, block_end --> range of the block (depending on the `horizontal` argument)
			   max_vec --> result vector
			   horizontal --> the direction of max opeartion (default true)
*/
template <class T>
void block_max(T *mat, int rows, int cols, int block_start, int block_end, T *max_vec, bool horizontal = true) {
	if (horizontal) {			// check direction
		for (int i = block_start; i < block_end; i++) {
			max_vec[i] = mat[i*cols + 0];				// initialize using first element
			for (int j = 1; j < cols; j++) {
				max_vec[i] = max_vec[i] > mat[i*cols + j] ? max_vec[i] : mat[i*cols + j];
			}
		}
	}
	else {
		for (int j = block_start; j < block_end; j++) {
			max_vec[j] = mat[0 * cols + j];
			for (int i = 1; i < rows; i++) {
				max_vec[j] = max_vec[j] > mat[i*cols + j] ? max_vec[j] : mat[i*cols + j];
			}
		}
	}
	
}
template <class T>
T* mat_parallel_max(T *mat, int rows, int cols, bool horizontal = true) {
	T* max_vec;
	int block_start = 0, block_end;

	if (horizontal) {
		max_vec = new T[rows];
		struct parallel_unit pu = init_block(rows);
		std::vector<std::thread> threads(pu.num_threads - 1);
		// do first pu.num_threads-1 blocks in the new threads
		for (int i = 0; i < pu.num_threads - 1; i++) {
			// calculate the block boundary
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_max<T>, mat, rows, cols, block_start, block_end, max_vec, true);
			block_start = block_end;
		}
		// do last block in this thread
		block_max<T>(mat, rows, cols, block_start, rows, max_vec, true);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	} else {
		max_vec = new T[cols];
		struct parallel_unit pu = init_block(cols);
		std::vector<std::thread> threads(pu.num_threads - 1);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_max<T>, mat, rows, cols, block_start, block_end, max_vec, false);
			block_start = block_end;
		}
		block_max<T>(mat, rows, cols, block_start, cols, max_vec, false);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	}
	return max_vec;
}

/*
	Function: do min operation in a fraction of the total dataset
	Arguments: mat --> data matrix
		   	   rows, cols --> shape of the matrix
			   block_start, block_end --> range of the block (depending on the `horizontal` argument)
			   min_vec --> result vector
			   horizontal --> the direction of min opeartion (default true)
*/
template <class T>
void block_min(T *mat, int rows, int cols, int block_start, int block_end, T *min_vec, bool horizontal = true) {
	if (horizontal) {			// check direction
		for (int i = block_start; i < block_end; i++) {
			min_vec[i] = mat[i*cols + 0];				// initialize using first element
			for (int j = 1; j < cols; j++) {
				min_vec[i] = min_vec[i] < mat[i*cols + j] ? min_vec[i] : mat[i*cols + j];
			}
		}
	}
	else {
		for (int j = block_start; j < block_end; j++) {
			min_vec[j] = mat[0 * cols + j];
			for (int i = 1; i < rows; i++) {
				min_vec[j] = min_vec[j] < mat[i*cols + j] ? min_vec[j] : mat[i*cols + j];
			}
		}
	}

}
template <class T>
T* mat_parallel_min(T *mat, int rows, int cols, bool horizontal = true) {
	T* min_vec;
	int block_start = 0, block_end;

	if (horizontal) {
		min_vec = new T[rows];
		struct parallel_unit pu = init_block(rows);
		std::vector<std::thread> threads(pu.num_threads - 1);
		// do first pu.num_threads-1 blocks in the new threads
		for (int i = 0; i < pu.num_threads - 1; i++) {
			// calculate the block boundary
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_min<T>, mat, rows, cols, block_start, block_end, min_vec, true);
			block_start = block_end;
		}
		// do last block in this thread
		block_min<T>(mat, rows, cols, block_start, rows, min_vec, true);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	}
	else {
		min_vec = new T[cols];
		struct parallel_unit pu = init_block(cols);
		std::vector<std::thread> threads(pu.num_threads - 1);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_min<T>, mat, rows, cols, block_start, block_end, min_vec, false);
			block_start = block_end;
		}
		block_min<T>(mat, rows, cols, block_start, cols, min_vec, false);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	}
	return min_vec;
}

/*
	Function: normalize the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   block_start, block_end --> range of the block (depending on the `horizontal` argument)
			   horizontal --> the direction of min opeartion (default true)
*/
void block_normalize(double *mat, int rows, int cols, int block_start, int block_end, bool horizontal) {
	double tot;

	if(horizontal) {
		for (int i = block_start; i < block_end; i++) {
			tot = (double)0;
			for (int j = 0; j < cols; j++) tot += mat[i*cols + j];
			if (tot > 0)
				for (int j = 0; j < cols; j++) mat[i*cols + j] /= tot;
		}
	} else {
		for (int j = block_start; j < block_end; j++) {
			tot = (double)0;
			for (int i = 0; i < rows; i++) tot += mat[i*cols + j];
			if(tot > 0)
				for (int i = 0; i < rows; i++) tot += mat[i*cols + j];
		}
	}
}
double* mat_parallel_normalize(double *mat, int rows, int cols, bool inplace, bool horizontal = HORIZONTAL) {
	double *mat_t;
	int block_start = 0, block_end; 

	if(inplace) {
		mat_t = mat;
	} else {
		mat_t = new double[rows*cols];
		memcpy(mat_t, mat, sizeof(double)*rows*cols);
	}
	if(horizontal) {
		struct parallel_unit pu = init_block(rows);
		std::vector<std::thread> threads(pu.num_threads - 1);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_normalize, mat_t, rows, cols, block_start, block_end, HORIZONTAL);
			block_start = block_end;
		}
		block_normalize(mat_t, rows, cols, block_start, rows, HORIZONTAL);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	} else {
		struct parallel_unit pu = init_block(cols);
		std::vector<std::thread> threads(pu.num_threads - 1);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_normalize, mat_t, rows, cols, block_start, block_end, VERTICAL);
			block_start = block_end;
		}
		block_normalize(mat_t, rows, cols, block_start, cols, VERTICAL);	
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	}
	return mat_t;
}

void block_scale(double *mat, int rows, int cols, int block_start, int block_end, double start, double end, double *max_vec, double *min_vec, bool horizontal = true) {
	if (horizontal) {
		for (int i = block_start; i < block_end; i++) {
			if (max_vec[i] > min_vec[i]) { // if the row is not same for all elements
				for (int j = 0; j < cols; j++) {
					mat[i*cols + j] = (mat[i*cols + j] - min_vec[i]) / (max_vec[i] - min_vec[i]) * (end - start) + start;
				}
			} else {
				for (int j = 0; j < cols; j++) {
					mat[i*cols + j] = (start + end) / 2;
				}
			}
		}

	} else {
		for (int j = block_start; j < block_end; j++) {
			if (max_vec[j] > min_vec[j]) { // if the column is not same for all elements
				for (int i = 0; i < rows; i++) {
					mat[i*cols + j] = (mat[i*cols + j] - min_vec[j]) / (max_vec[i] - min_vec[j]) * (end - start) + start;
				}
			} else {
				for (int i = 0; i < rows; i++) {
					mat[i*cols + j] = (start + end) / 2;
				}
			}
		}

	}
}
/*
	Function: Scale the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   inplace --> whether scale in the given matrix or create a new one
			   start, end --> scale range
			   horizontal --> scale the matrix horizontally or vertically (default false)
*/
double* mat_parallel_scale(double *mat, int rows, int cols, bool inplace, double start, double end, bool horizontal = true) {
	double *mat_t, *max_vec, *min_vec;
	int block_start = 0, block_end;
	
	if(inplace) {
		mat_t = mat;
	} else {
		mat_t = new double[rows*cols];
		memcpy(mat_t, mat, sizeof(double)*rows*cols);
	}
	if(horizontal) {
		struct parallel_unit pu = init_block(rows);
		std::vector<std::thread> threads(pu.num_threads - 1);
		max_vec = new double[rows];
		min_vec = new double[rows];
		max_vec = mat_parallel_max(mat, rows, cols, HORIZONTAL);
		min_vec = mat_parallel_min(mat, rows, cols, HORIZONTAL);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_scale, mat_t, rows, cols, block_start, block_end, start, end, max_vec, min_vec, HORIZONTAL);
			block_start = block_end;
		}
		block_scale(mat_t, rows, cols, block_start, rows, start, end, max_vec, min_vec, VERTICAL);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	} else {
		struct parallel_unit pu = init_block(rows);
		std::vector<std::thread> threads(pu.num_threads - 1);
		max_vec = new double[cols];
		min_vec = new double[cols];
		max_vec = mat_parallel_max(mat, rows, cols, VERTICAL);
		min_vec = mat_parallel_min(mat, rows, cols, VERTICAL);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_scale, mat_t, rows, cols, block_start, block_end, start, end, max_vec, min_vec, VERTICAL);
			block_start = block_end;
		}
		block_scale(mat_t, rows, cols, block_start, cols, start, end, max_vec, min_vec, VERTICAL);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	}
	delete[] max_vec;
	delete[] min_vec;
	return mat_t;
}

/*
	Function:
	Arguments:
*/
template <class T>
void block_accumulate(T *mat, int rows, int cols, int block_start, int block_end, T* accu_vec, bool horizontal = HORIZONTAL) {
	if(horizontal = HORIZONTAL) {
		for (int i = block_start; i < block_end; i++) {
			accu_vec[i] = (T)0;
			for (int j = 0; j < cols; j++) {
				accu_vec[i] += mat[i*cols + j];
			}
		}
	} else {
		for (int j = block_start; j < block_end; j++) {
			accu_vec[j] = (T)0;
			for (int i = 0; i < rows; i++) {
				accu_vec[j] += mat[i*cols + j];
			}
		}
	}
}
/*
	Function: accumulate the matrix
	Arguments: mat --> data matrix
			   rows, cols --> shape of the matrix
			   horizontal --> accumulate the matrix horizontally or vertically or accumulate the whole matrix
*/
template <class T>
T* mat_parallel_accumulate(T *mat, int rows, int cols, int horizontal = HORIZONTAL) {
	T* accu_vec;
	int block_start = 0, block_end;
	if (horizontal == HORIZONTAL) {
		accu_vec = new T[rows];
		struct parallel_unit pu = init_block(rows);
		std::vector<std::thread> threads(pu.num_threads - 1);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_accumulate<T>, mat, rows, cols, block_start, block_end, accu_vec, HORIZONTAL);
			block_start = block_end;
		}
		block_accumulate(mat, rows, cols, block_start, rows, accu_vec, HORIZONTAL);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	} else if (horizontal == VERTICAL) {
		accu_vec = new T[cols];
		struct parallel_unit pu = init_block(cols);
		std::vector<std::thread> threads(pu.num_threads - 1);
		for (int i = 0; i < pu.num_threads - 1; i++) {
			block_end = block_start + pu.block_size;
			threads[i] = std::thread(block_accumulate<T>, mat, rows, cols, block_start, block_end, accu_vec, VERTICAL);
			block_start=  block_end;
		}
		block_accumulate(mat, rows, cols, block_start, cols, accu_vec, VERTICAL);
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	} else if (horizontal == ALL) {
		accu_vec = new T;
		*accu_vec = (T)0;
		T* accu_vec_t;
		if (rows <= cols) {
			accu_vec_t = mat_parallel_accumulate(mat, rows, cols, HORIZONTAL);
			for (int i = 0; i < rows; i++) *accu_vec += accu_vec_t[i];
		} else {
			accu_vec_t = mat_parallel_accumulate(mat, rows, cols, VERTICAL);
			for (int j = 0; j < cols; j++) *accu_vec += accu_vec_t[j];
		}
		delete[] accu_vec_t;
	} else {
		std::cout << "function mat_accumulate: invalid horizontal argument. must be `HORIZONTAL`, `VERTICAL` or `ALL`" << std::endl;
		exit(1);
	}
	return accu_vec;
}
#endif

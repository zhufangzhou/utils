#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <thread>
#include <algorithm>
#include <vector>
#include "container.h"

template <class T>
struct item {
	int item_id;
	T val;
	void set(int _id, T _val) {
		item_id = _id;
		val = _val;
	}
	bool operator < (item &x) {
		return this->val < x.val;
	}
	bool operator > (item &x) {
		return this->val > x.val;
	}
	bool operator == (item &x) {
		return this->val == x.val;
	}
	T operator * (int x) {
		return val*x;
	}
};

struct parallel_unit{
	unsigned long const num_threads;	// launch `num_threads` threads
	unsigned long const block_size;		// size of block for each thread to handle
	// constructor
	parallel_unit(int num_threads, int block_size): num_threads(num_threads), block_size(block_size) {}
};

/* declaration */
struct parallel_unit init_block(int length, unsigned long const min_per_thread = 100, int specified_num_threads = -1);
struct parallel_unit init_block(int length, int specified_num_threads);
void block_normalize(double *mat, int rows, int cols, int block_start, int block_end, bool horizontal);
double* mat_parallel_normalize(double *mat, int rows, int cols, bool inplace, bool horizontal = HORIZONTAL);
void block_scale(double *mat, int rows, int cols, int block_start, int block_end, double start, double end, double *max_vec, double *min_vec, bool horizontal = true);
double* mat_parallel_scale(double *mat, int rows, int cols, bool inplace, double start, double end, bool horizontal = true);

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
	Function:
	Arguments:
*/
template <class T>
void block_accumulate(T *mat, int rows, int cols, int block_start, int block_end, T* accu_vec, bool horizontal = HORIZONTAL) {
	if(horizontal == HORIZONTAL) {
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


/*
	MergeSort: Parallel Version
 */
template <class T>
void block_sort(T* start, T* end) {
	std::sort(start, end);
}
template <class T>
void parallel_mergesort(T *vec, int size) {
	struct parallel_unit pu = init_block(size);
	int block_start, block_end;
	block_start = 0;
	std::vector<std::thread> threads(pu.num_threads - 1);
	if (size < 1000) {
		std::sort(vec, vec+size);
		return ;
	}

	// sort
	for (int i = 0; i < pu.num_threads - 1; i++) {
		block_end = block_start + pu.block_size;
//		threads[i] = std::thread(block_sort<T>, vec+block_start, vec+block_end);
		threads[i] = std::thread(parallel_mergesort<T>, vec+block_start, pu.block_size);
		block_start = block_end;
	}
	//block_sort(vec+block_start, vec+size);
	parallel_mergesort(vec+block_start, size-block_start);
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	
	// merge
	int* block_end_idx = new int[pu.num_threads];
	int* block_idx = new int[pu.num_threads];
	int merge_count = 0;
	T* ret;
	heap<item<T> > my_heap(MIN_HEAP);
	item<T> item_temp;
	for (int i = 0; i < pu.num_threads - 1; i++) {
		block_idx[i] = 0 + i*pu.block_size;
		block_end_idx[i] = block_idx[i] + pu.block_size;
	}
	block_idx[pu.num_threads-1] = (pu.num_threads-1)*pu.block_size; 
	block_end_idx[pu.num_threads-1] = size;
	// initialize the heap
	for (int i = 0; i < pu.num_threads; i++) {
		int &cur_pos = block_idx[i];
		if (cur_pos < block_end_idx[i]) {
			item_temp.set(i, vec[cur_pos]);
			my_heap.push(item_temp);
			cur_pos++;
		}
	}
	ret = new T[size];
	while (merge_count < size) {
		if (!my_heap.is_empty()) {
			item_temp = my_heap.extract();
			int &cur_pos = block_idx[item_temp.item_id];
			ret[merge_count++] = item_temp.val;
			if (cur_pos < block_end_idx[item_temp.item_id]) {
				item_temp.val = vec[cur_pos];
				my_heap.push(item_temp);	
				cur_pos++;
			}
		} else {
			throw "error occur when merge";
		}
	}
	memcpy(vec, ret, sizeof(T)*size);
	delete[] ret;
}


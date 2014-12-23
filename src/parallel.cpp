#include "parallel.h"

/*
 *     Function: Initalize the parallel setting
 *         Arguments: length --> the length to be paralleled
 *                        min_per_thread -> the minimum length each thread deal with
 *                        */
struct parallel_unit init_block(int length, unsigned long const min_per_thread, int specified_num_threads) {
    unsigned long max_threads;
    unsigned long hardware_threads;
    unsigned long num_threads;
    unsigned long block_size;

    if (specified_num_threads != -1 && specified_num_threads < 1) {
        throw "`n_threads` must satisfy `n_threads` > 1 or `n_threads` == -1 (means max threads)";
    }

    hardware_threads = std::thread::hardware_concurrency();
    if (specified_num_threads == -1) {
        max_threads = (length + min_per_thread - 1) / min_per_thread;
    } else {
        max_threads = specified_num_threads;
    }

    num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    block_size = length / num_threads;

    struct parallel_unit pu(num_threads, block_size);
    return pu;
}
struct parallel_unit init_block(int length, int specified_num_threads) {
    return init_block(length, 1, specified_num_threads);
}

/*
 *     Function: normalize the matrix
 *         Arguments: mat --> data matrix
 *                        rows, cols --> shape of the matrix
 *                                       block_start, block_end --> range of the block (depending on the `horizontal` argument)
 *                                                      horizontal --> the direction of min opeartion (default true)
 *                                                      */
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
double* mat_parallel_normalize(double *mat, int rows, int cols, bool inplace, bool horizontal) {
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

void block_scale(double *mat, int rows, int cols, int block_start, int block_end, double start, double end, double *max_vec, double *min_vec, bool horizontal) {
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
 *     Function: Scale the matrix
 *         Arguments: mat --> data matrix
 *                        rows, cols --> shape of the matrix
 *                                       inplace --> whether scale in the given matrix or create a new one
 *                                                      start, end --> scale range
 *                                                                     horizontal --> scale the matrix horizontally or vertically (default false)
 *                                                                     */
double* mat_parallel_scale(double *mat, int rows, int cols, bool inplace, double start, double end, bool horizontal) {
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

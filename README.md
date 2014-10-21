# Utils Library
======

## utils.h
	#define eps 1e-5  
	#define HORIZONTAL 1  
	#define VERTICAL 0  
	#define ALL -1  
	#define INPLACE true  
	#define NOT_INPLACE false  
	#define ASC 1  
	#define DESC -1  
	#define INF 0x7fffffff  
	#define is_zero(a) ((a) < eps && (a) > -eps)  
	
	1. Display  

	void print_vec(T* vec, int size, char* msg = NULL)  
	void print_mat(T* mat, int rows, int cols, char* msg = NULL)

	2. Sort
		int* argsort(T* arr, int size, int asc = ASC)
		int* argsort(T* mat, int rows, int cols, int target, int asc = ASC, int* idx = NULL)
		int* partial_argsort(T* mat, int rows, int cols, int* active_row, int active_row_size, int target, int asc = ASC, int* idx = NULL)
	
	3. Sample
		int* random_sample(int size, int m, int* idx = NULL)
		T* random_sample(T* mat, int rows, int cols, int m, T* ret = NULL)
	
	4. Matrix/Vector Manipulation
		T* mat_max(T* mat, int rows, int cols, bool horizontal = true)
		T* mat_min(T* mat, int rows, int cols, bool horizontal = true)
		double* mat_normalize(double* mat, int rows, int cols, bool inplace, bool horizontal = true)
		double* vec_normalize(double* vec, int size, bool inplace)
		double* mat_scale(double* mat, int rows, int cols, bool inplace, double start, double end, bool horizontal = false)
		T* mat_accumulate(T* mat, int rows, int cols, int horizontal = HORIZONTAL)
	
	5. Generate Matrix/Vector
		double* gen_dmat(int rows, int cols, double start, double end)
		double* gen_dmat(int rows, int cols)
		double* gen_dvec(int size, double start, double end)
		double* gen_dvec(int size)
		int* gen_imat(int rows, int cols, int start, int end)
		int *gen_imat(int rows, int cols)
		int* gen_ivec(int size, int start, int end)
		int* gen_ivec(int size)
		
	6. Weighted Median
		T weighted_median(T* val, double* w, int size)
		
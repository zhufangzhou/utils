#include "Utils.h"
//#include "Parallel.h"
//#include "cmdLine.h"
#include "distance.h"
/*
void test_argsort() {
	int iarr[] = { 2, 4, 1, 5, 3 }, *idx;
	double darr[] = { 2.1, 4.1, 1.1, 5.1, 3.1 };
	idx = argsort(iarr, 5, -1);
	print_vec(idx, 5);
	delete[] idx;
	idx = argsort(darr, 5, 1);
	print_vec(idx, 5);
	delete[] idx;
}

void test_gen_mat() {
	int *mat;
	mat = gen_imat(3, 3, 0, 10);
	print_mat(mat, 3, 3, "randomly generate a matrix");
	delete[] mat;
}

void gen_test_dataset() {
	int n = 2000000, m = 100;
	double *mat;
	FILE *fp = fopen("train.libfm", "r");
	mat = gen_dmat(n, m, 0, 100);
	for(int i = 0; i < n; i++) {
		fprintf(fp, "%d", m_random::getInstance().next_int(0,2));
		for(int j = 0; j < m; j++) {
			fprintf(fp, " %d:%lf", j+1, mat[i*m+j]);
		}
		fprintf(fp, "\n");
	}

	delete[] mat;
}

void test_max_min_mat() {
	int *mat, *max_vec, *min_vec;
	int rows = 4, cols = 5;
	mat = gen_imat(rows, cols, 0, 10);
	print_mat(mat, rows, cols, "randomly generate a matrix");

	max_vec = mat_max(mat, rows, cols, HORIZONTAL);
	print_vec(max_vec, rows, "max vector horizontal");
	delete[] max_vec;
	max_vec = mat_max(mat, rows, cols, VERTICAL);
	print_vec(max_vec, cols, "max vector vertical");
	delete[] max_vec;

	min_vec = mat_min(mat, rows, cols, HORIZONTAL);
	print_vec(min_vec, rows, "min vector horizontal");
	delete[] min_vec;
	min_vec = mat_min(mat, rows, cols, VERTICAL);
	print_vec(min_vec, cols, "min vector vertical");
	delete[] min_vec;

	delete[] mat;
	
}

void test_normalize() {
	double *mat, *new_mat;
	mat = gen_dmat(4, 5, 1, 10);
	print_mat(mat, 4, 5, "randomly generate a matrix");

	new_mat = mat_normalize(mat, 4, 5, false, HORIZONTAL);
	print_mat(new_mat, 4, 5, "after horizontal normalize");

	delete[] mat;
	delete[] new_mat;
}

void test_scale() {
	double *mat;
	mat = gen_dmat(4, 5, 0, 1000);
	print_mat(mat, 4, 5, "randomly generate a matrix");

	mat_scale(mat, 4, 5, true, 1, 10, HORIZONTAL);
	print_mat(mat, 4, 5, "after horizontal scale");

	delete[] mat;
}

void test_parallel_max() {
	int *mat, *max_vec1, *max_vec2;
	int rows = 5000000, cols = 50;
	mat = gen_imat(rows, cols, 0, 10);
	//print_mat(mat, 4, 5, "randomly generate a matrix");
	timer.tic();
	max_vec1 = mat_parallel_max(mat, rows, cols, HORIZONTAL);
	timer.toc("Parallel");
	timer.tic();
	max_vec2 = mat_max(mat, rows, cols, HORIZONTAL);
	timer.toc("Single thread");
//	print_vec(max_vec, 4000, "max vector horizontal");
	for (int i = 0; i < rows; i++) {
		if (max_vec1[i] != max_vec2[i]) {
			std::cout << "row " << i << " is wrong" << std::endl;
			break;
		}
	}
	delete[] mat;
	delete[] max_vec1;
	delete[] max_vec2;
}

void test_parallel_normalize() {
	double *mat, *new_mat, *accu_vec;
	int rows = 4, cols = 5;
	mat = gen_dmat(rows, cols, 1, 10);
	print_mat(mat, rows, cols, "randomly generate a matrix");

	new_mat = mat_normalize(mat, rows, cols, false, HORIZONTAL);
	print_mat(new_mat, rows, cols, "after horizontal normalize");
	accu_vec = mat_parallel_accumulate(new_mat, rows, cols, HORIZONTAL);
	print_mat(accu_vec, rows, 1, "accumulate result");
	delete[] accu_vec;
	delete[] new_mat;

	new_mat = mat_parallel_normalize(mat, rows, cols, false, HORIZONTAL);
	print_mat(new_mat, rows, cols, "after parallel horizontal normalize");
	accu_vec = mat_parallel_accumulate(new_mat, rows, cols, ALL);
	std::cout << *accu_vec << std::endl;
	delete accu_vec;
	delete[] new_mat;

	delete[] mat;
}
*/
int main(int argc, char** argv) {
	//test_argsort();
	//test_gen_mat();
	//test_max_min_mat();
	//test_normalize();
	//test_scale();
//	test_parallel_max();
//	test_parallel_normalize();
	//gen_test_dataset();

	return 0;
}

#ifndef _DISTANCE_H
#define _DISTANCE_H

/*
 * This header file contains many different distance measure
 *
 */

#include <cmath>
#include <algorithm>
#include <cstring>

int bit_count(int x);
int lcs(char *x, char *y);

int bit_count(int x) {
	int count = 0;
	while (x > 0 && ++count)
		x -= (x & -x);
	
	return count;
}

int lcs(char *x, char *y) {
	int len_x, len_y, **dp, ret, now;
	len_x = strlen(x);
	len_y = strlen(y);

	if (len_x == 0 || len_y == 0) return 0;

	dp = new int*[2];
	dp[0] = new int[2*len_y];
	dp[1] = dp[0] + len_y;
	
	for (int j = 0; j < len_y; j++) dp[0][j] = x[0] == y[j];
	
	now = 1;
	for (int i = 1; i < len_x; i++) {
		for (int j = 0; j < len_y; j++) {
			if (x[i] == y[j]) {
				dp[now][j] = j == 0 ? 0 : dp[now^1][j-1];
			} else {
				dp[now][j] = std::max(dp[now^1][j], j == 0 ? 0 : dp[now][j-1]);
			}
		}
		now = now ^ 1;
	}
	ret = dp[now^1][len_y-1];
	delete dp[0];
	delete dp;

	return ret;
}

template <class T>
double l1_norm(T *x, int dim = 2) {
	double norm = 0.0;
	if (dim < 1)
		throw "bad dimension value";

	for (int i = 0; i < dim; i++) {
		norm += fabs(1.0*x[i]);	
	}
	return norm;
}

template <class T>
double l2_norm(T *x, int dim = 2) {
	double norm = 0.0;
	if (dim < 1) 
		throw "bad dimension value";

	for (int i = 0; i < dim; i++)
		norm += 1.0*x[i]*x[i];
	
	return sqrt(norm);
}

template <class T>
double euclidean_dist(T *x, T *y, int dim = 2) {
	double dist = 0.0;
	if (dim < 1) 
		throw "bad dimension value";

	for (int i = 0; i < dim; i++)
		dist += 1.0*(x[i]-y[i])*(x[i]-y[i]);
	
	return sqrt(dist);
}

template <class T>
double cosine_dist(T *x, T* y, int dim = 2) {
	double dist = 0.0;
	if (dim < 1) 
		throw "bad dimension value";
	
	for (int i = 0; i < dim; i++)
		dist += 1.0*x[i]*y[i];
	
	return dist / (l2_norm(x, dim) * l2_norm(y, dim));
}

template <class T>
double manhattan_dist(T *x, T *y, int dim = 2) {
	double dist = 0.0;
	if (dim < 1)
		throw "bad dimension value";

	for (int i = 0; i < dim; i++)
		dist += fabs(1.0*(x[i]-y[i]));
		
	return dist;
}


template <class T>
double jaccard_distance(T *x, int dim_x, T *y, int dim_y) {
	int same, distinct, pt_x, pt_y;
	T biggest_current, now_x, now_y;
	if (dim_x < 0 || dim_y < 0) 
		throw "bad dimension value";
	std::sort(x, x+dim_x);
	std::sort(y, y+dim_y);

	biggest_current = std::min(x[0], y[0]) - 1;
	pt_x = pt_y = 0;
	same = distinct = 0;
	while (pt_x < dim_x && pt_y < dim_y) {
		now_x = x[pt_x]; now_y = y[pt_y];
		if (now_x < now_y) {
			// move x
			while (pt_x < dim_x && now_x == x[++pt_x]);
		} else if (now_x > now_y) {
			// move y
			while (pt_y < dim_y && now_y == y[++pt_y]);
		} else {
			// move x and y
			while (pt_x < dim_x && now_x == x[++pt_x]);
			while (pt_y < dim_y && now_y == y[++pt_y]);
			same++;
		}
		distinct++;
	}
	// finish x first in the while loop above
	if (pt_x >= dim_x) {
		while (pt_y < dim_y) {
			now_y = y[pt_y];
			distinct++;
			// move y
			while (pt_y < dim_y && now_y == y[++pt_y]);
		}
	} else {
		while (pt_x < dim_x) {
			now_x = x[pt_x];
			distinct++;
			// move x
			while (pt_x < dim_x && now_x == x[++pt_x]);
		}
	}

	return 1.0*same/distinct;
}


int edit_dist(char *x, char *y) {
	return 0;
}

int hamming_dist(int x, int y) {
	return bit_count(x^y);	
}
#endif

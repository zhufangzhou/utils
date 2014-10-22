#ifndef _DISTANCE_H
#define _DISTANCE_H

/*
 * This header file contains many different distance measure
 *
 */

#include <cmath>
#include <algorithm>
#include <cstring>
#include <string>

int bit_count(int x);
int lcs(std::string x, std::string y);
int edit_dist(std::string x, std::string y);
int hamming_dist(int x, int y);



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


#endif

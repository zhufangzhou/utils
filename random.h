#ifndef __RANDOM_H
#define __RANDOM_H


#include <iostream>
#include <cmath>



class m_random {
	public:
		m_random() {
			srand((unsigned int)time(NULL));
		}

		~m_random() { }

		static m_random getInstance() {
			static m_random r;
			return r;
		}
		
		// return a random integer
		int next_int() {
			return rand();
		}
		// return a random integer between `begin` and `end` ( [begin, end) )
		int next_int(int begin, int end) {
			if (begin > end) {
				throw "In `next_int`, `end` must larger than `begin`.";
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
				throw "In `next_int`, `end` must larger than `begin`.";
			}
			return (double)rand() / RAND_MAX * (end - begin) + begin;
		}

};

double draw_uniform() {
	double u;
	// return value must bigger than 0
	do {
		u = m_random::getInstance().next_int() / ((double)(RAND_MAX)+1);	
	} while (u == 0.0);
	return u;
}

// Ref. --> A Fast Normal Random Number Generator (JOSEPH L. LEV A)
double draw_gaussian() {
	double u, v, r, x, y, s, t, Q, a, b, r1, r2;
	r = 0.8578; s = 0.449871; t = -0.386595;
	a = 0.1960; b = 0.254720; r1 = 0.27597; r2 = 0.27846;

	while (true) {
		// step 1
		u = draw_uniform();
		v = draw_uniform();
		v = 1.7156*(v-0.5);
		// step 2
		x = u - s;
		y = fabs(v) - t;
		Q = x*x + y*(a*y-b*x);
		// step 3
		if (Q < r1) 
			break;	
		// step 4
		if (Q > r2)
			continue;
		// step 5
		if (v*v > -4*u*u*log(u))
			continue;
	}
	// step 6
	return v/u;
}

double draw_gaussian(double mu, double sigma) {
	if (sigma == 0)
		return mu;
	else
		return mu + draw_gaussian() * sigma;
}

// Ref. --> A simple method for generating gamma variables (Marsaglia and Tsang)
double draw_gamma(double alpha, double beta) {
	double d, c, z, u, v, x;
	if (alpha >= 1.0) {
		// step 1
		d = alpha - (1.0/3);
		c = 1.0 / sqrt(9*d);
		while (true) {
			// step 2
			z = draw_gaussian();
			u = draw_uniform();

			// step 3
			v = 1+c*z;
			v = v*v*v;
			if ((z > -1.0/c) && (u < 0.5*z*z+d-d*v+d*log(v))) {
				break;	
			}
		}
		return d*v / beta;
	} else if (alpha > 0 && alpha < 1) {
		x = draw_gamma(alpha+1, beta);	
		u = draw_uniform();
		return x*pow(u,1.0/alpha);
	} else {
		throw "bad alpha value";
	}
}

#endif

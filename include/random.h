#ifndef __RANDOM_H
#define __RANDOM_H

#include <iostream>
#include <cmath>

double draw_uniform();
// Ref. --> A Fast Normal Random Number Generator (JOSEPH L. LEV A)
double draw_gaussian();
double draw_gaussian(double mu, double sigma);
// Ref. --> A simple method for generating gamma variables (Marsaglia and Tsang)
double draw_gamma(double alpha, double beta);




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
		/**
		 *  return a random integer between `begin` and `end` ( [begin, end) )
		 */
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


#endif

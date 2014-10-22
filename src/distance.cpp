#include "distance.h"


int bit_count(int x) {
	int count = 0;
	while (x > 0 && ++count)
		x -= (x & -x);
	
	return count;
}


int lcs(std::string x, std::string y) {
	int len_x, len_y, **dp, ret, now;
	len_x = x.length();
	len_y = y.length();

	if (len_x == 0 || len_y == 0) return 0;

	dp = new int*[2];
	dp[0] = new int[2*len_y];
	dp[1] = dp[0] + len_y;
	
	for (int j = 0; j < len_y; j++) {
		dp[0][j] = (j==0?0:dp[0][j-1]) + (x[0] == y[j]);
	}
	
	now = 1;
	for (int i = 1; i < len_x; i++) {
		for (int j = 0; j < len_y; j++) {
			if (x[i] == y[j]) {
				dp[now][j] = j == 0 ? 1 : dp[now^1][j-1]+1;
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

int edit_dist(std::string x, std::string y) {
	return x.length() + y.length() - 2*lcs(x, y);
}

int hamming_dist(int x, int y) {
	return bit_count(x^y);	
}


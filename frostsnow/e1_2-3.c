#include <math.h>
#include <stdio.h>

/**
 * Solve the equation "n / log_2(n) < 8" via a binary search.
 */
int main(int argc, char* argv[]) {
	int n;
	int max;
	int min;
	float left;

	max = 100000;
	min = 0;

	left = 2.0f * log2f(100);
	while ((n = (min + max) / 2) < max) {
		float right;

		right = ((float)n) / (log2f((float)n));

		if (left < right) {
			max = n;
		} else {
			min = n + 1;
		}
		printf("n: %i, left: %f, right: %f, new max: %i, new min: %i.\n", n, left, right, max, min);
	}
	printf("Answer: n <= %i.\n", n);
	return 0;
}

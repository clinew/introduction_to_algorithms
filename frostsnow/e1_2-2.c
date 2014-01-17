#include <math.h>
#include <stdio.h>

/**
 * Solve the equation "n / log_2(n) < 8" via a binary search.
 */
int main(int argc, char* argv[]) {
	int n;
	int max;
	int min;

	max = 100000;
	min = 0;

	while ((n = (min + max + 1) / 2) > min) {
		float temp;

		temp = ((float)n) / (log2f((float)n));

		if (temp < 8.0f) {
			min = n;
		} else {
			max = n - 1;
		}
		printf("n: %i, temp: %f, new max: %i, new min: %i.\n", n, temp, max, min);
	}
	printf("Answer: n <= %i.\n", n);
	return 0;
}

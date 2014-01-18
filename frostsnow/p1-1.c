#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

void binary_search(double (*function)(uint64_t), uint64_t microseconds) {
	uint64_t max = UINT64_MAX - 2;
	uint64_t min = 0;
	uint64_t n;

	while ((n = (min + max + 1) / 2) > min) {
		double temp;

		temp = function(n);

		if (temp <= microseconds) {
			min = n;
		} else {
			max = n - 1;
		}
		//printf("n: %llu, temp: %f, new max: %llu, new min: %llu.\n", n, temp, max, min);
	}
	printf("Answer: n <= %llu for t = %llu.\n", n, microseconds);
}

double nlg_2n(uint64_t n) {
	return ((double) n * log2((double)n));
}

double squared(uint64_t n) {
	return ((double)n) * ((double)n);
}

double cubed(uint64_t n) {
	return ((double)n) * ((double)n) * ((double)n);
}

double exponential(uint64_t n) {
	return pow(2, (double)n);
}

double factorial(uint64_t n) {
	double temp;
	uint64_t i;

	/* My computer is not actually powerful enough to solve this within my lifetime.
	temp = 1.0f;
	for (i = 2; i <= n; i++) { // One of the few times I ever wrote a <= for loop.
		temp *= i;
	} */
	temp = DBL_MAX;

	return temp;
}

/**
 * Solve the equation "n / log_2(n) < 8" via a binary search.
 */
int main(int argc, char* argv[]) {
	uint64_t numbers[] = { 1000000ULL, 60000000ULL, 3600000000ULL, 86400000000ULL, 2592000000000ULL, 31100000000000ULL, 3110000000000000ULL };
	uint64_t n;
	int i;

	// n lg_2(n)
	printf("n lg_2(n):\n");
	for (i = 0; i < (sizeof(numbers) / sizeof(uint64_t)); i++) {
		binary_search(nlg_2n, numbers[i]);
	}
	// n^2
	printf("n^2:\n");
	for (i = 0; i < (sizeof(numbers) / sizeof(uint64_t)); i++) {
		binary_search(squared, numbers[i]);
	}
	// n^3
	printf("n^3:\n");
	for (i = 0; i < (sizeof(numbers) / sizeof(uint64_t)); i++) {
		binary_search(cubed, numbers[i]);
	}
	// 2^n
	printf("2^n:\n");
	for (i = 0; i < (sizeof(numbers) / sizeof(uint64_t)); i++) {
		binary_search(exponential, numbers[i]);
	}
	// n!
	/* Doesn't actually work due to... large numbers.
	printf("n!:\n");
	for (i = 0; i < (sizeof(numbers) / sizeof(uint64_t)); i++) {
		binary_search(factorial, numbers[i]);
	} */

	return 0;
}

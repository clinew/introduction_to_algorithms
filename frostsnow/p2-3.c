#include <stdio.h>

// Sum a polynominal with the specific coefficients.
double naiveity(double* coefficients, double x, size_t number_count) {
	int i;
	int j;
	double sum;

	for (i = 0; i < number_count; i++) {
		double product;

		// Calculate the x-portion.
		for (j = 0, product = 1.0f; j < i; j++) {
			product *= x;
		}

		// Finish calculating the sum.
		sum += product * x;
	}

	return sum;
}

int main(int argc, char* argv[]) {
	double coefficients[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.4f, 3.1f, 1.2f};
	double evaluation;

	printf("Evaluation yields: %f.\n", naiveity(coefficients, 1.3f, sizeof(coefficients)/sizeof(double)));
}


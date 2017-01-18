#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// c >= (1 / (log_2(4) - log_2(3)))
//const c = 2.41;
const c = 808; // High 'c' is necessary for Base Case.

double bigoh(int n);
double recurr(int n);

int main(int argc, char* argv[]) {
	int i = 0;

	for (i = 34; i < 102; i++) {
		fprintf(stdout, "n: %3i, T(n): %11f, f(n): %11f\n", i, recurr(i), bigoh(i));
	}
}

double bigoh(int n) {
	return (c * n * (log(n)/log(2))) - (34 * c * (log(n)/log(2))) - (3 * n * c);
}

double recurr(int n) {
	if (n < 34) {
		fprintf(stdout, "GG u suk\n");
		exit(EXIT_FAILURE);
	} else if (n == 34) {
		return 1.0;
	} else {
		return (2 * recurr((n / 2) + 17) + n);
	}
}

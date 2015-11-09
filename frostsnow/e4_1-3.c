#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Arguments to the program.
struct arguments {
	// Use brute-force rather than recursive method.
	int brute;
	// Array size.
	int count;
};

/**
 * Parse arguments into the specified structure.
 */
int arguments_parse(struct arguments* arguments, int argc, char* argv[]) {
	int ret;

	// Set default arguments.
	arguments->brute = 0;
	arguments->count = 32;

	// Parse arguments.
	while ((ret = getopt(argc, argv, "bn:")) != -1) {
		// Parsing error.
		if (ret == '?' || ret == ':') {
			return -1;
		}

		switch (ret) {
		case 'b':
			arguments->brute = 1;
			break;
		case 'n':
			arguments->count = atoi(optarg);
			if (arguments->count < 1) {
				fprintf(stderr, "Count must be greater than 1"
					", recieved: %i\n", arguments->count);
				return -1;
			}
			break;
		default:
			return -1;
		}
	}
	return 0;
}

/**
 * Print usage message and exit the program.
 */
void arguments_usage(char* message) {
	FILE* stream;
	// Print error message.
	if (message) {
		stream = stderr;
		fprintf(stream, "ERROR: %s\n\n", message);
	} else {
		stream = stdout;
	}

	// Print usage message.
	fprintf(stream, "USAGE: e4_1-3 [-b] [-n INT]\n\n");
	fprintf(stream, "  -b     Use brute-force rather than recursive algorithm\n");
	fprintf(stream, "  -n INT Use array of size INT (default: 32)\n");

	// Exit the program.
	if (message) {
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

/**
 * Brute-force implementation of the maximum subarray problem.
 */
int maxsubarray_brute(int* array, int array_count,
	int* out_low, int* out_high) {
	int high = -1;
	int low = -1;
	int sum_max = INT_MIN;
	int i;
	int j;

	for (i = 0; i < array_count; i++) {
		int sum_cur = 0;
		for (j = i; j < array_count; j++) {
			sum_cur += array[j];
			if (sum_cur > sum_max) {
				sum_max = sum_cur;
				high = j;
				low = i;
			}
		}
	}
	*out_high = high;
	*out_low = low;
	return sum_max;
}

/**
 * Find maximum crossing subarray. Private function.
 */
int maxsubarray_recur_crossing(int* array, int low, int mid, int high,
	int* out_low, int* out_high) {
	int i;
	int left_low = -1;
	int left_sum_cur;
	int left_sum_max = INT_MIN;
	int right_high = -1;
	int right_sum_cur;
	int right_sum_max = INT_MIN;

	// Get left sum.
	left_sum_cur = 0;
	for (i = mid; i >= low; i--) {
		left_sum_cur += array[i];
		if (left_sum_cur > left_sum_max) {
			left_low = i;
			left_sum_max = left_sum_cur;
		}
	}
	// Get right sum.
	right_sum_cur = 0;
	for (i = mid + 1; i <= high; i++) {
		right_sum_cur += array[i];
		if (right_sum_cur > right_sum_max) {
			right_high = i;
			right_sum_max = right_sum_cur;
		}
	}
	*out_low = left_low;
	*out_high = right_high;
	return (left_sum_max + right_sum_max);
}

/**
 * Recursive implementation of the maximum subarray problem.
 */
int maxsubarray_recur(int* array, int low, int high,
	int* out_low, int* out_high) {
	int mid;
	int cross_high;
	int cross_low;
	int cross_sum;
	int left_high;
	int left_low;
	int left_sum;
	int right_high;
	int right_low;
	int right_sum;

	// Base case.
	if (low == high) {
		*out_low = low;
		*out_high = high;
		return array[low];
	}

	// Recursive case.
	mid = (low + high) / 2;
	left_sum = maxsubarray_recur(array, low, mid,
		&left_low, &left_high);
	right_sum = maxsubarray_recur(array, mid + 1, high,
		&right_low, &right_high);
	cross_sum = maxsubarray_recur_crossing(array, low, mid, high,
		&cross_low, &cross_high);
	if (left_sum > right_sum && left_sum > cross_sum) {
		// Left-size largest.
		*out_low = left_low;
		*out_high = left_high;
		return left_sum;
	} else if (right_sum > left_sum && right_sum > cross_sum) {
		// Right-side largest.
		*out_low = right_low;
		*out_high = right_high;
		return right_sum;
	}
	*out_low = cross_low;
	*out_high = cross_high;
	return cross_sum;
}

int main(int argc, char* argv[]) {
	struct arguments arguments;
	int* array;
	int high;
	int i;
	int low;
	int sum;

	// TODO:
	// - Allow book's array.
	// - Print brute to Graphviz.
	// - Print recursive to Graphviz.
	// - Time run.
	// - Find cross-over point.
	// - Use modified implementation.
	// - Find new cross-over point.

	// Parse arguments.
	if (arguments_parse(&arguments, argc, argv) == -1) {
		arguments_usage("Parsing arguments");
	}

	// Generate array.
	array = malloc(sizeof(int) * arguments.count);
	if (!array) {
		fprintf(stderr, "Unable to allocate array.\n");
		exit(EXIT_FAILURE);
	}
	srand(0xDEADBEEF);
	for (i = 0; i < arguments.count; i++) {
		array[i] = (rand() % 64) - 32;
		printf("%i ", array[i]);
	}
	printf("\n");

	// Solve max-aubarray problem.
	if (arguments.brute) {
		// Brute-force.
		sum = maxsubarray_brute(array, arguments.count, &low, &high);
		printf("Brute: '%i','%i','%i', ", sum, low, high);
	} else {
		// Recursive.
		sum = maxsubarray_recur(array, 0, arguments.count - 1, &low, &high);
		printf("Recursive: '%i','%i','%i'.\n", sum, low, high);
	}

	// Exit the program.
	exit(EXIT_SUCCESS);
}

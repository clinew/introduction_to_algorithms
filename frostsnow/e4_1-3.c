#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Arguments to the program.
struct arguments {
	// Use the book's array.
	int book;
	// Use brute-force rather than recursive method.
	int brute;
	// Array size.
	int count;
	// Print graph.
	int print;
};

/**
 * Parse arguments into the specified structure.
 */
int arguments_parse(struct arguments* arguments, int argc, char* argv[]) {
	int ret;

	// Set default arguments.
	arguments->book = 0;
	arguments->brute = 0;
	arguments->count = 32;
	arguments->print = 0;

	// Parse arguments.
	while ((ret = getopt(argc, argv, "bkn:p")) != -1) {
		// Parsing error.
		if (ret == '?' || ret == ':') {
			return -1;
		}

		switch (ret) {
		case 'b':
			arguments->brute = 1;
			break;
		case 'k':
			arguments->book = 1;
			break;
		case 'n':
			arguments->count = atoi(optarg);
			if (arguments->count < 1) {
				fprintf(stderr, "Count must be greater than 1"
					", recieved: %i\n", arguments->count);
				return -1;
			}
			break;
		case 'p':
			arguments->print = 1;
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
	fprintf(stream, "USAGE: e4_1-3 [-b] [-k] [-n INT]\n\n");
	fprintf(stream, "  -b     Use brute-force rather than recursive algorithm\n");
	fprintf(stream, "  -k     Use book's subarray instead of generated one. Overrides '-n'.\n");
	fprintf(stream, "  -n INT Use array of size INT (default: 32).\n");
	fprintf(stream, "  -p     Print graph output.\n");

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
 * Brute-force implementation of the maximum subarray problem;
 * also writes graph output.
 */
int maxsubarray_brute_print(int* array, int array_count,
	int* out_low, int* out_high) {
	FILE* file;
	int high = -1;
	int low = -1;
	int sum_max = INT_MIN;
	int i;
	int j;

	// Open dot file.
	file = fopen("e4_1-3.dot", "w");
	if (!file) {
		fprintf(stderr, "Error opening output file");
		return -1;
	}
	fprintf(file, "graph \"\" {\n");
	fprintf(file, "\tnode [shape=record];\n");

	// Run sort.
	for (i = 0; i < array_count; i++) {
		int sum_cur = 0;
		int sum_max_local = INT_MIN;
		int sum_cur_local = 0;
		int high_local;
		int low_local;

		// Sort.
		for (j = i; j < array_count; j++) {
			sum_cur += array[j];
			if (sum_cur > sum_max) {
				sum_max = sum_cur;
				high = j;
				low = i;
			}
			sum_cur_local += array[j];
			if (sum_cur_local > sum_max_local) {
				sum_max_local = sum_cur_local;
				high_local = j;
				low_local = i;
			}
		}

		// Graph.
		fprintf(file, "\tstruct%i [label=\"", i, i);
		for (j = 0; j < array_count; j++) {
			fprintf(file, "<f%i> %i %s", j, array[j], j == (array_count - 1) ? "\"" : "|");
		}
		fprintf(file, "];\n");
		if (i) {
			fprintf(file, "\tstruct%i -- struct%i [color=white];\n", i - 1, i);
		}
	}
	*out_high = high;
	*out_low = low;

	// Close dot file.
	fprintf(file, "}\n");
	fclose(file);
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
	// + Print brute to Graphviz.
	//  - Label max-subarray using HTML-like labels
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
	if (arguments.book) {
		arguments.count = 16;
	}
	array = malloc(sizeof(int) * arguments.count);
	if (!array) {
		fprintf(stderr, "Unable to allocate array.\n");
		exit(EXIT_FAILURE);
	}
	srand(0xDEADBEEF);
	if (arguments.book) {
		// Use book's array.
		array[0] = 13; array[1] = -3; array[2] = -25; array[3] = 20;
		array[4] = -3; array[5] = -16; array[6] = -23; array[7] = 18;
		array[8] = 20; array[9] = -7; array[10] = 12; array[11] = -5;
		array[12] = -22; array[13] = 15; array[14] = -4; array[15] = 7;
	} else {
		// Randomly-generated array.
		for (i = 0; i < arguments.count; i++) {
			array[i] = (rand() % 64) - 32;
		}
	}
	for (i = 0; i < arguments.count; i++) {
		printf("%i ", array[i]);
	}
	printf("\n");

	// Solve max-aubarray problem.
	if (arguments.brute) {
		// Brute-force.
		if (arguments.print) {
			sum = maxsubarray_brute_print(array, arguments.count, &low, &high);
		} else {
			sum = maxsubarray_brute(array, arguments.count, &low, &high);
		}
		printf("Brute: '%i','%i','%i'.\n", sum, low, high);
	} else {
		// Recursive.
		sum = maxsubarray_recur(array, 0, arguments.count - 1, &low, &high);
		printf("Recursive: '%i','%i','%i'.\n", sum, low, high);
	}

	// Exit the program.
	exit(EXIT_SUCCESS);
}

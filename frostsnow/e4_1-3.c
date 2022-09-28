#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Arguments to the program.
struct arguments {
	// Use the book's array.
	int book;
	// Use brute-force rather than recursive method.
	int brute;
	// Array size.
	int count;
	// Use hybrid algorithm.
	int hybrid;
	// Use linear algorithm.
	int linear;
	// Print graph.
	int print;
	// Measure duration.
	int time;
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
	arguments->hybrid = 0;
	arguments->linear = 0;
	arguments->print = 0;
	arguments->time = 0;

	// Parse arguments.
	while ((ret = getopt(argc, argv, "bhkln:pt")) != -1) {
		// Parsing error.
		if (ret == '?' || ret == ':') {
			return -1;
		}

		switch (ret) {
		case 'b':
			arguments->brute = 1;
			break;
		case 'h':
			arguments->hybrid = 1;
			break;
		case 'k':
			arguments->book = 1;
			break;
		case 'l':
			arguments->linear = 1;
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
		case 't':
			arguments->time = 1;
			break;
		default:
			return -1;
		}
	}

	// Check for contradictory option.
	if (arguments->hybrid || arguments->linear) {
		if (arguments->print) {
			fprintf(stderr, "Cannot print neither hybrid nor "
				"linear algorithm\n");
			exit(EXIT_FAILURE);
		}
	}
	if (arguments->hybrid + arguments->linear + arguments->brute > 1) {
		fprintf(stderr, "Must select a single algorithm\n");
		exit(EXIT_FAILURE);
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
	fprintf(stream, "USAGE: e4_1-3 [-bhklt] [-n INT]\n\n");
	fprintf(stream, "  -b     Use brute-force rather than recursive algorithm.\n");
	fprintf(stream, "  -h     Use hybrid solution.\n");
	fprintf(stream, "  -k     Use book's subarray instead of generated one. Overrides '-n'.\n");
	fprintf(stream, "  -l     Use linear solution.\n");
	fprintf(stream, "  -n INT Use array of size INT (default: 32).\n");
	fprintf(stream, "  -p     Print graph output.\n");
	fprintf(stream, "  -t     Time algorithm run-time.\n");

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
	fprintf(file, "\tnode [shape=plaintext];\n");

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
		fprintf(file, "\tstruct%i [label=<<table border=\"0\" "
			"cellborder=\"1\" cellspacing=\"0\"><tr>", i);
		for (j = 0; j < array_count; j++) {
			if (j < i) {
				// Entry out of bounds.
				fprintf(file, "<td color=\"gray\">"
					"<font color=\"gray\">%i</font></td>",
					array[j]);
			} else if (j >= low_local && j <= high_local) {
				// Max-subarray entry.
				fprintf(file, "<td bgcolor=\"darkgreen\">"
					"<font color=\"white\">%i</font></td>",
					array[j]);
			} else {
				// Not max subarray.
				fprintf(file, "<td>%i</td>", array[j]);
			}
		}
		fprintf(file, "</tr><tr><td colspan=\"%i\" color=\"none\">"
			"Max: %i</td></tr></table>>];\n", array_count,
			sum_max_local);
		if (i) {
			fprintf(file, "\tstruct%i -- struct%i [color=white];\n",
				i - 1, i);
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
 * Hybrid implementation of the maximum subarray problem.
 */
int maxsubarray_hybrid(int* array, int low, int high,
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
	if (high - mid >= 32) {
		left_sum = maxsubarray_recur(array, low, mid,
			&left_low, &left_high);
		right_sum = maxsubarray_recur(array, mid + 1, high,
			&right_low, &right_high);
	} else {
		left_sum = maxsubarray_brute(&array[low], mid - low + 1,
			&left_low, &left_high);
		right_sum = maxsubarray_brute(&array[mid + 1], high - mid,
			&right_low, &right_high);
	}
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

/**
 * Find maximum crossing subarray in linear time.
 */
int maxsubarray_linear(int* array, int array_count,
	int* out_low, int* out_high) {
	int i;
	int j;
	int sum_max;
	int sum_cur;

	// Calculate max subarray.
	(*out_low) = (*out_high) = i = 0;
	sum_max = INT_MIN;
	sum_cur = 0;
	for (j = 0; j < array_count; j++) {
		// Merge elements.
		sum_cur += array[j];
		if (sum_cur >= sum_max) {
			// New max-crossing subarray.
			sum_max = sum_cur;
			(*out_low) = i;
			(*out_high) = j;
		}
		if (sum_cur < 0) {
			// New minimum point. Reset current array as it will
			// not become a larger maximum subarray before the
			// new one will.
			i = j + 1;
			sum_cur = 0;
		}
	}
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

int _maxsubarray_recur_print(int * array, int low, int high,
	int* out_low, int* out_high, FILE* file) {
	int cross_high;
	int cross_low;
	int cross_sum;
	int i;
	int left_high;
	int left_low;
	int left_sum;
	int mid;
	int right_high;
	int right_low;
	int right_sum;
	int sum;

	// Print top array node.
	fprintf(file, "\tstruct_t_%i_%i [label = <<table border=\"0\" "
		"cellborder=\"1\" cellspacing=\"0\"><tr>", low, high);
	for (i = low; i <= high; i++) {
		fprintf(file, "<td>%i</td>", array[i]);
	}
	fprintf(file, "</tr></table>>];\n");

	// Base case.
	if (low == high) {
		*out_low = low;
		*out_high = high;
		return array[low];
	}

	// Recursive case.
	mid = (low + high) / 2;
	left_sum = _maxsubarray_recur_print(array, low, mid,
		&left_low, &left_high, file);
	right_sum = _maxsubarray_recur_print(array, mid + 1, high,
		&right_low, &right_high, file);
	cross_sum = maxsubarray_recur_crossing(array, low, mid, high,
		&cross_low, &cross_high);
	if (left_sum > right_sum && left_sum > cross_sum) {
		// Left-size largest.
		*out_low = left_low;
		*out_high = left_high;
		sum = left_sum;
	} else if (right_sum > left_sum && right_sum > cross_sum) {
		// Right-side largest.
		*out_low = right_low;
		*out_high = right_high;
		sum = right_sum;
	} else {
		*out_low = cross_low;
		*out_high = cross_high;
		sum = cross_sum;
	}

	// Print bottom node.
	fprintf(file, "\tstruct_b_%i_%i [label = <<table border=\"0\" "
		"cellborder=\"1\" cellspacing=\"0\"><tr>", low, high);
	for (i = low; i <= high; i++) {
		if (i == mid + 1) {
			fprintf(file, "<td width=\"1\" color=\"none\"></td>");
		}
		fprintf(file, "<td%s>%i</td>",
			(i >= *out_low) && (i <= *out_high) ?
			" bgcolor=\"darkgreen\"" : "", array[i]);
	}
	fprintf(file, "</tr><tr><td color=\"none\" colspan=\"%i\">",
		high - low + 2);
	if (sum == left_sum) {
		fprintf(file, "Left");
	} else if (sum == right_sum) {
		fprintf(file, "Right");
	} else {
		fprintf(file, "Crossing");
	}
	fprintf(file, "</td></tr></table>>];\n");

	// Print links between top and bottom nodes.
	fprintf(file, "\tstruct_t_%i_%i -- struct_t_%i_%i;\n", low, high,
		low, mid);
	fprintf(file, "\tstruct_t_%i_%i -- struct_t_%i_%i;\n", low, high,
		mid + 1, high);
	fprintf(file, "\tstruct_%s_%i_%i -- struct_b_%i_%i;\n",
		low - mid == 0 ? "t" : "b", low, mid,
		low, high);
	fprintf(file, "\tstruct_%s_%i_%i -- struct_b_%i_%i;\n",
		mid + 1 - high == 0 ? "t" : "b", mid + 1, high,
		low, high);

	// Return current sum.
	return sum;
}
int maxsubarray_recur_print(int * array, int low, int high,
	int* out_low, int* out_high) {
	FILE* file;
	int sum;

	// Create graph file.
	file = fopen("e4_1-3.dot", "w");
	if (!file) {
		fprintf(stderr, "Error opening output file");
		return -1;
	}
	fprintf(file, "graph \"\" {\n");
	fprintf(file, "\tnode [shape=plaintext];\n");

	// Call function.
	sum = _maxsubarray_recur_print(array, low, high, out_low, out_high,
		file);

	// Close graph file.
	fprintf(file, "}\n");
	fclose(file);

	return sum;
}

int main(int argc, char* argv[]) {
	struct arguments arguments;
	int* array;
	int high;
	int i;
	int low;
	int sum;
	struct timespec time_start;
	struct timespec time_end;

	// TODO:
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

	// Start time measurement.
	if (arguments.time) {
		if (clock_gettime(CLOCK_MONOTONIC, &time_start) == -1) {
			perror("Unable to start timer");
			exit(EXIT_FAILURE);
		}
	}

	// Solve max-aubarray problem.
	if (arguments.hybrid) {
		// Hybrid approach.
		sum = maxsubarray_hybrid(array, 0, arguments.count - 1, &low,
			&high);
		printf("Hybrid: '%i','%i','%i'.\n", sum, low, high);
	} else if (arguments.brute) {
		// Brute-force.
		if (arguments.print) {
			sum = maxsubarray_brute_print(array, arguments.count,
				&low, &high);
		} else {
			sum = maxsubarray_brute(array, arguments.count, &low,
				&high);
		}
		printf("Brute: '%i','%i','%i'.\n", sum, low, high);
	} else if (arguments.linear) {
		// Linear.
		sum = maxsubarray_linear(array, arguments.count, &low, &high);
		printf("Linear: '%i','%i','%i'.\n", sum, low, high);
	} else {
		// Recursive.
		if (arguments.print) {
			sum = maxsubarray_recur_print(array, 0,
				arguments.count - 1, &low, &high);
		} else {
			sum = maxsubarray_recur(array, 0, arguments.count - 1,
				&low, &high);
		}
		printf("Recursive: '%i','%i','%i'.\n", sum, low, high);
	}

	// End time measurement.
	if (arguments.time) {
		if (clock_gettime(CLOCK_MONOTONIC, &time_end) == -1) {
			perror("Unable to end timer");
			exit(EXIT_FAILURE);
		}
	}

	// Print time difference.
	if (arguments.time) {
		struct timespec time_diff;

		// Calculate difference.
		time_diff.tv_sec = time_end.tv_sec - time_start.tv_sec;
		if ((time_diff.tv_nsec = time_end.tv_nsec - time_start.tv_nsec)
			< 0) {
			time_diff.tv_nsec += 1000000000;
			time_diff.tv_sec -= 1;
		}

		// Print difference.
		fprintf(stderr, "Elapsed time: %li.%010li\n", time_diff.tv_sec,
			time_diff.tv_nsec);
	}

	// Exit the program.
	exit(EXIT_SUCCESS);
}

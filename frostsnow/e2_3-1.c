/*
 * Copyright (C) 2014 Wade T. Cline
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct doge {
	char very[16];
	char much[16];
	char so[16];
	// All doges are created wowful, but some doges are more wowful than others.
	unsigned wow;
};

// Output file for graph. Global variables are bad...
FILE* file;
// Unique identifier for arrays. This allows 'graphviz' to uniquely identify value-equivalent arrays.
unsigned array_ids = 0;

// Prints the values of the specified array.
void merge_sort_print(void* array, size_t item_count, size_t item_size, void(*print)(void*)) {
	int i;

	// Print the values of the specified array.
	for (i = 0; i < item_count; i++) {
		print((void*)(((char*)array) + (i * item_size)));
		i + 1 < item_count ? fprintf(file, "|") : 0;
	}
}

void merge_sort(void* array, unsigned array_id, size_t item_count, size_t item_size, int(*comparator)(void*, void*), void(*print)(void*)) {
	void* array_left;
	void* array_right;
	size_t array_left_length;
	size_t array_right_length;
	unsigned array_left_id;
	unsigned array_right_id;
	int i;
	int j;
	int merge_index;
	int print_index;

	// Print array label declaration.
	fprintf(file, "\t\"%s%u\" [label=\"", item_count < 2 ? "sorted" : "unsorted", array_id);
	merge_sort_print(array, item_count, item_size, print);
	fprintf(file, "\"];\n");

	// Base case; do nothing.
	if (item_count < 2) {
		return;
	}

	// Break the array apart.
	array_right_length = item_count / 2;
	array_left_length = item_count - array_right_length;
	array_left = malloc(item_size * array_left_length);
	array_right = malloc(item_size * array_right_length);
	if (!array_left || !array_right) {
		array_left ? free(array_left) : 0;
		array_right ? free(array_right) : 0;
	}
	memcpy(array_left, array, array_left_length * item_size);
	memcpy(array_right, (void*)(((char*)array) + (array_left_length * item_size)),
	       array_right_length * item_size);
	array_left_id = array_ids++;
	array_right_id = array_ids++;
	
	// Print the broken array and its pieces.
	fprintf(file, "\t\"unsorted%u", array_id);
	fprintf(file, "\"-> {\"%s%u", array_left_length > 1 ? "unsorted" : "sorted", array_left_id);
	fprintf(file, "\";\"%s%u", array_right_length > 1 ? "unsorted" : "sorted", array_right_id);
	fprintf(file, "\"};\n");

	// Merge sort the broken arrays.
	merge_sort(array_left, array_left_id, array_left_length, item_size, comparator, print);
	merge_sort(array_right, array_right_id, array_right_length, item_size, comparator, print);

	// Merge the broken arrays.
	i = j = merge_index = 0;
	while (i < array_left_length || j < array_right_length) {
		if (i >= array_left_length) {
			// Merge right item.
			memcpy((void*)(((char*)array) + (merge_index++ * item_size)),
			       (void*)(((char*)array_right) + (j++ * item_size)),
			       item_size);
		} else if (j >= array_right_length) {
			// Merge left item.
			memcpy((void*)(((char*)array) + (merge_index++ * item_size)),
			       (void*)(((char*)array_left) + (i++ * item_size)),
			       item_size);
		} else {
			if (comparator((void*)(((char*)array_left) + (i * item_size)),
				       (void*)(((char*)array_right) + (j * item_size))) < 0) {
				// Merge left item.
				memcpy((void*)(((char*)array) + (merge_index++ * item_size)),
				       (void*)(((char*)array_left) + (i++ * item_size)),
				       item_size);
			} else {
				// Merge right item.
				memcpy((void*)(((char*)array) + (merge_index++ * item_size)),
				       (void*)(((char*)array_right) + (j++ * item_size)),
				       item_size);
			}
		}
	}

	// Print the broken arrays merging into the main array.
	fprintf(file, "\t\"sorted%u", array_id);
	fprintf(file, "\" [label=\"");
	merge_sort_print(array, item_count, item_size, print);
	fprintf(file, "\"];\n");
	fprintf(file, "\t\"sorted%u", array_left_id);
	fprintf(file, "\" -> \"sorted%u", array_id);
	fprintf(file, "\";\n\t\"sorted%u", array_right_id);
	fprintf(file, "\" -> \"sorted%u", array_id);
	fprintf(file, "\";\n");

	// Free the broken arrays.
	free(array_left);
	free(array_right);
}

int doge_comparator(void* x, void* y) {
	struct doge a = (*(struct doge*)x);
	struct doge b = (*(struct doge*)y);

	// Compare the doge's "wow" factor.
	if (a.wow < b.wow) {
		return -1;
	} else if (a.wow == b.wow) {
		return 0;
	} else {
		return 1;
	}
}

void doge_print(void* item) {
	struct doge* doge = (struct doge*)item;
	printf("much %s. very %s. so %s. wow. (%u)", doge->much, doge->very, doge->so, doge->wow);
}

int int_comparator(void* x, void* y) {
	int a = (*(int*)x);
	int b = (*(int*)y);

	if (a < b) {
		return -1;
	} else if (a == b) {
		return 0;
	} else {
		return 1;
	}
}

void int_print(void* item) {
	fprintf(file, "%i", *(int*)item);
}

int main(int argc, char* argv[]) {
	int i;
	int ints[] = {3, 41, 52, 26, 38, 57, 9, 49};

	// Create ints array the fun way (for testing fun :D)
	/*
	srand(24853);
	int_count = 100;
	ints = malloc(sizeof(int) * int_count);
	if (!ints) {
		fprintf(stderr, "Unable to create ints array.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < int_count; i++) {
		ints[i] = rand() % 512;
	} */

	// Open file for writing.
	file = fopen("testing.dot", "w");
	if (!file) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}

	// Print and sort ints.
	fprintf(file, "digraph stuff {\n\tnode [shape=record];\n\tsplines=false;\n");
	merge_sort(ints, array_ids++, sizeof(ints)/sizeof(int), sizeof(int), int_comparator, int_print);
	fprintf(file, "}\n");

	// Close file.
	close(file);

	// Print and sort doges.
	/*
	printf("\n");
	for (i = 0; i < (sizeof(doges)/sizeof(struct doge)); i++) {
		doge_print((void*)&doges[i]);
	}
	printf("\n");
	merge_sort(doges, sizeof(doges)/sizeof(struct doge), sizeof(struct doge), doge_comparator, doge_print);
	for (i = 0; i < (sizeof(doges)/sizeof(struct doge)); i++) {
		doge_print(&doges[i]);
	}
	printf("\n");
	*/
}

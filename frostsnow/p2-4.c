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

// AKA Merge Sort.
void inversions_fast(void* array, size_t item_count, size_t item_size, int(*comparator)(void*, void*), unsigned* inversions) {
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
		printf("Unable to allocate arrays\n");
		return;
	}
	memcpy(array_left, array, array_left_length * item_size);
	memcpy(array_right, (void*)(((char*)array) + (array_left_length * item_size)),
	       array_right_length * item_size);
	
	// Merge sort the broken arrays.
	inversions_fast(array_left, array_left_length, item_size, comparator, inversions);
	inversions_fast(array_right, array_right_length, item_size, comparator, inversions);

	// Merge the broken arrays.
	i = j = merge_index = 0;
	while (i < array_left_length || j < array_right_length) {
		if (i >= array_left_length) {
			//(*inversions += (array_left_length - i));
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
				       (void*)(((char*)array_right) + (j * item_size))) > 0) {
				// Merge right item.
				(*inversions += (array_left_length - i));
				memcpy((void*)(((char*)array) + (merge_index++ * item_size)),
				       (void*)(((char*)array_right) + (j++ * item_size)),
				       item_size);
			} else {
				// Merge left item.
				memcpy((void*)(((char*)array) + (merge_index++ * item_size)),
				       (void*)(((char*)array_left) + (i++ * item_size)),
				       item_size);
			}
		}
	}

	// Free the broken arrays.
	free(array_left);
	free(array_right);
}

void inversions_slow(int* array, size_t item_count, int(*comparator)(void*, void*), unsigned* inversions) {
	int i;
	int j;

	(*inversions) = 0;
	for (i = 0; i < item_count; i++) {
		for (j = i+1; j < item_count; j++) {
			if (comparator((void*)&array[i], (void*)&array[j]) > 0) {
				(*inversions)++;
			}
		}
	}
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
	printf("%i", *(int*)item);
}

int main(int argc, char* argv[]) {
	int i;
	//int ints[] = {2, 3, 8, 6, 1};
	//int ints[] = {53, 312, 34, 43, 643, 2312, 213, 75};
	int* ints;
	int int_count;
	unsigned inversions;

	// Create ints array the fun way (for testing fun :D)
	srand(24853);
	int_count = 484;
	ints = malloc(sizeof(int) * int_count);
	if (!ints) {
		fprintf(stderr, "Unable to create ints array.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < int_count; i++) {
		ints[i] = rand() % 512;
	}

	// Get inversions the slow way.
	printf("\n");
	for (i = 0; i < (sizeof(ints)/sizeof(int)); i++) {
		int_print((void*)&ints[i]);
	}
	printf("\n");
	//inversions_slow(ints, sizeof(ints)/sizeof(int), int_comparator, &inversions);
	inversions_slow(ints, int_count, int_comparator, &inversions);
	printf("\n");
	for (i = 0; i < (sizeof(ints)/sizeof(int)); i++) {
		int_print(&ints[i]);
	}
	printf("\n\nInversions: %u.\n", inversions);

	// Get inversions the fast way.
	printf("\n");
	for (i = 0; i < (sizeof(ints)/sizeof(int)); i++) {
		int_print((void*)&ints[i]);
	}
	printf("\n");
	inversions = 0;
	//inversions_fast(ints, sizeof(ints)/sizeof(int), sizeof(int), int_comparator, &inversions);
	inversions_fast(ints, int_count, sizeof(int), int_comparator, &inversions);
	printf("\n");
	for (i = 0; i < (sizeof(ints)/sizeof(int)); i++) {
		int_print(&ints[i]);
	}
	printf("\n\nInversions: %u.\n", inversions);

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
	printf("\n"); */
}

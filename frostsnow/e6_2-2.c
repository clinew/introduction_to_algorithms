/*
 * Copyright (C) 2015 Wade T. Cline.
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

struct heap {
	// Number of elements that the heap can support.
	int size;
	// Number of valid elements actually in the heap.
	int length;
	// Actual element data.
	char* data;
	// Size of a single element.
	size_t datum_size;
};

/**
 * Perform minsort on the heap specified by index 'i'; note that index 'i'
 * starts from 1, not 0, for the purposes of this exercise.
 */
void heap_minsort(struct heap* heap, int i, int(*comparator)(char*, char*)) {
	int left;
	int right;
	int smallest;

	// Get child nodes.
	left = i * 2;
	right = (i * 2) + 1;

	do {
		// Find smallest element.
		smallest = i;
		if (left <= heap->length && comparator(
			(heap->data + (heap->datum_size * (smallest - 1))),
			(heap->data + (heap->datum_size * (left - 1)))) > 0) {
			// Left child smaller than parent.
			smallest = left;
		}
		if (right <= heap->length && comparator(
			(heap->data + (heap->datum_size * (smallest - 1))),
			(heap->data + (heap->datum_size * (right - 1)))) > 0) {
			// Right child smallest element.
			smallest = right;
		}

		// Swap elemnents and recurse.
		if (smallest != i) {
			// Create buffer.
			char* buffer = malloc(heap->datum_size);
			if (!buffer) {
				perror("No memory :'(");
				exit(EXIT_FAILURE);
			}

			// Swap the elements.
			memcpy(buffer,
				heap->data + (heap->datum_size * (i - 1)),
				heap->datum_size);
			memcpy(heap->data + (heap->datum_size * (i - 1)),
				heap->data + (heap->datum_size * (smallest - 1)),
				heap->datum_size);
			memcpy(heap->data + (heap->datum_size * (smallest - 1)),
				buffer,
				heap->datum_size);

			// Free buffer.
			free(buffer);
		}
	} while (smallest != i);
}

/**
 * Initialize the specified heap to contain 'count' elements of size 'size'.
 */
int heap_init(struct heap* heap, int count, size_t size) {
	// Allocate space for items.
	heap->data = (char*)malloc(count * size);
	if (!heap->data) {
		perror("Heap alloc failed");
		return -1;
	}
	heap->length = 0;
	heap->size = count;
	heap->datum_size = size;
	return 0;
}

/**
 * Insert an element into the heap.
 */
int heap_insert(struct heap* heap, char* data, int(*comparator)(char*, char*)) {
	int i;

	// Check heap size constraint.
	if (heap->size == heap->length) {
		// Already full.
		return -1;
	}

	// Insert element.
	memcpy(heap->data + (heap->datum_size * heap->length),
		data,
		heap->datum_size);
	heap->length++;

	// Maintain heap property... evily.
	i = heap->length;
	while ((i /= 2) > 0) {
		heap_minsort(heap, i, comparator);
	};

	// Return success.
	return 0;
}

/**
 * Print the heap... hackishly.
 */
void heap_print(struct heap* heap) {
	int i;
	for (i = 0; i < heap->length; i++) {
		printf("%i ", *(int*)(heap->data + (i * heap->datum_size)));
		if (!(((unsigned)i+2) & (unsigned)(i+1))) {
			printf("\n");
		}
	}
	printf("\n");
}

/**
 * Comparator for a heap made up of 'int's. Use 'memcmp' semantics.
 */
int heap_int_comparator(char* a, char* b) {
	// Return integer comparison.
	if (*(int*)a > *(int*)b) {
		return 1;
	} else if (*(int*)a < *(int*)b) {
		return -1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	struct heap heap;
	int i;

	// Create integer heap.
	if (heap_init(&heap, 16, sizeof(int)) == -1) {
		fprintf(stderr, "Init heap fail\n");
		exit(EXIT_FAILURE);
	}
	srand(42); // Not cliche at all.
	for (i = 0; i < heap.size; i++) {
		int num = rand() % 100;
		printf("%i ", num);
		heap_insert(&heap, (char*)&num, heap_int_comparator);
	}
	printf("\n");
	heap_print(&heap);
}

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

/**
 * Performs a linear search on the specified array.
 */
void linear_search(void* items, void* target, size_t item_count, size_t item_size, int(*comparator)(void*, void*), int** index) {
	int i;

	// Allocate space for the return value (ew).
	(*index) = malloc(sizeof(int));
	if (!(*index)) {
		fprintf(stderr, "Error mallocing int.\n");
		exit(EXIT_FAILURE);
	}

	// Search each item in the array, 
	for (i = 0; i < item_count; i++) {
		if (!comparator((void*)(((char*)items) + (i * item_size)), target)) {
			(**index) = i;
			return;
		}
	}
	(*index) = NULL;
}

int int_comparator(void* x, void* y) {
	int a = *(int*)x;
	int b = *(int*)y;

	if (a < b) {
		return -1;
	} else if (a == b) {
		return 0;
	} else {
		return 1;
	}
}

int main(int argc, char* argv[]) {
	int* index;
	int ints[] = {1, 2, 3, 4, 6, 7, 8, 9, 10};
	int target;

	target = 5;
	linear_search((void*)ints, (void*)&target, sizeof(ints)/sizeof(int), sizeof(int), int_comparator, &index);
	if (index) {
		printf("Index: %i", *index);
	} else {
		printf("Not found");
	}
	printf(".\n");
}

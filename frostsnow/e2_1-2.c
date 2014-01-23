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

void insertion_sort(void* array, size_t item_count, size_t item_size, int(*comparator)(void*, void*)) {
	int i;
	void* temp;

	// Initialize temporary variable.
	temp = malloc(item_size);
	if (!temp) {
		fprintf(stderr, "Unable to malloc %u.\n", item_size);
	}

	// Perform the insertion sort.
	for (i = 0; i < item_count; i++) {
		int j;
		// Save current item in temporary variable.
		memcpy(temp, (void*)(((char*)array) + (i * item_size)), item_size);

		// Move smaller items to the left.
		j = i;
		while (j > 0 && comparator((void*)(((char*)array) + ((j - 1) * item_size)),
					   temp) < 0) {
			memcpy(((void*)((char*)array) + (j * item_size)),
			       ((void*)((char*)array) + ((j - 1) * item_size)), item_size);
			j--;
		}
		memcpy((void*)((char*)array) + (j * item_size), temp, item_size);
	}

	// Free the temporary variable.
	free(temp);
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

void doge_print(struct doge* doge) {
	printf("much %s. very %s. so %s. wow. (%u)\n", doge->much, doge->very, doge->so, doge->wow);
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

int main(int argc, char* argv[]) {
	struct doge doges[] = {{"test", "iffy", "syntax", 35}, {"C", "static type", "nitpick", 978},
			       {"generic", "void*", "pointer", 673}, {"algorithm", "O(n^2)", "insert", 76}};
	int i;
	int ints[] = {2, 4, 6, 4, 1, 84, 31, 532, 656732, 34};

	// Print and sort ints.
	for (i = 0; i < (sizeof(ints)/sizeof(int)); i++) {
		printf("%i ", ints[i]);
	}
	printf("\n");
	insertion_sort(ints, sizeof(ints)/sizeof(int), sizeof(int), int_comparator);
	for (i = 0; i < (sizeof(ints)/sizeof(int)); i++) {
		printf("%i ", ints[i]);
	}
	printf("\n");

	// Print and sort doges.
	printf("\n");
	for (i = 0; i < (sizeof(doges)/sizeof(struct doge)); i++) {
		doge_print(&doges[i]);
	}
	printf("\n");
	insertion_sort(doges, sizeof(doges)/sizeof(struct doge), sizeof(struct doge), doge_comparator);
	for (i = 0; i < (sizeof(doges)/sizeof(struct doge)); i++) {
		doge_print(&doges[i]);
	}
	printf("\n");
}

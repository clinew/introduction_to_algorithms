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

#include <math.h>
#include <stdio.h>

/**
 * Solve the equation "n / log_2(n) < 8" via a binary search.
 */
int main(int argc, char* argv[]) {
	int n;
	int max;
	int min;
	float left;

	max = 100000;
	min = 0;

	left = 2.0f * log2f(100);
	while ((n = (min + max) / 2) < max) {
		float right;

		right = ((float)n) / (log2f((float)n));

		if (left < right) {
			max = n;
		} else {
			min = n + 1;
		}
		printf("n: %i, left: %f, right: %f, new max: %i, new min: %i.\n", n, left, right, max, min);
	}
	printf("Answer: n <= %i.\n", n);
	return 0;
}

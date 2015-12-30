#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Add the specified matrices a and b into the specified matrix c.
 */
void matrix_add(int* a, int* b, int dim, int* c) {
	int i;
	int j;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			int index = i * dim + j;
			c[index] = a[index] + b[index];
		}
	}
}

/**
 * Joins the submatricies into the specified matrix.
 */
void matrix_join(int* matrix, int dim, int* a11, int* a12, int* a21,
	int* a22) {
	int i;
	int j;

	for (i = 0; i < dim / 2; i++) {
		for (j = 0; j < dim / 2; j++) {
			matrix[dim * i + j] = a11[(i * dim/2) + j];
		}
	}
	for (i = 0; i < dim / 2; i++) {
		for (j = dim / 2; j < dim; j++) {
			matrix[dim * i + j] = a12[(i * dim/2) + (j - dim / 2)];
		}
	}
	for (i = dim / 2; i < dim; i++) {
		for (j = 0; j < dim / 2; j++) {
			matrix[dim * i + j] = a21[((i - dim / 2) * dim/2) + j];
		}
	}
	for (i = dim / 2; i < dim; i++) {
		for (j = dim / 2; j < dim; j++) {
			matrix[dim * i + j] = a22[((i - dim / 2) * dim/2) + (j - dim/2)];
		}
	}
}

/**
 * Hacky print of matrix to stdout.
 */
void matrix_print(int* matrix, int dim) {
	int i;
	int j;

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			printf("%3i ", matrix[i * dim + j]);
		}
		printf("\n");
	}
}

/**
 * Creates submatricies for the specified square matrix. Submatrices must be
 * freed.
 */
void matrix_subs(int* matrix, int dim, int** a11, int** a12, int** a21,
	int** a22) {
	int i;
	int j;
	size_t size;

	// Allocate submatrices.
	size = sizeof(int) * dim * dim / 4;
	(*a11) = malloc(size);
	(*a12) = malloc(size);
	(*a21) = malloc(size);
	(*a22) = malloc(size);
	if (!(*a11) || !(*a12) || !(*a21) || !(*a22)) {
		fprintf(stderr, "Error allocating submatrices\n");
	}
	memset(*a11, 0, size);
	memset(*a12, 0, size);
	memset(*a21, 0, size);
	memset(*a22, 0, size);

	for (i = 0; i < dim / 2; i++) {
		for (j = 0; j < dim / 2; j++) {
			(*a11)[i * dim / 2 + j] = matrix[i * dim + j];
		}
	}
	for (i = 0; i < dim / 2; i++) {
		for (j = dim / 2; j < dim; j++) {
			(*a12)[(i * dim / 2) + (j - dim / 2)] = matrix[i * dim + j];
		}
	}
	for (i = dim / 2; i < dim; i++) {
		for (j = 0; j < dim /2; j++) {
			(*a21)[((i - dim / 2) * dim / 2) + j] = matrix[i * dim + j];
		}
	}
	for (i = dim / 2; i < dim; i++) {
		for (j = dim / 2; j < dim; j++) {
			(*a22)[((i - dim / 2) * dim / 2) + (j - dim / 2)] = matrix[i * dim + j];
		}
	}
}

/**
 * Subtract the specified matrices a and b into the specified matrix c.
 */
void matrix_subtract(int* a, int* b, int dim, int* c) {
	int i;
	int j;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			int index = i * dim + j;
			c[index] = a[index] - b[index];
		}
	}
}

/**
 * Implementation of Strassen's Matrix Multiplication method. Works only for
 * square matrices.
 */
void strassen(int* matrix_a, int* matrix_b, int dim, int* matrix_c) {
	int* a11, *a12, *a21, *a22;
	int* b11, *b12, *b21, *b22;
	int* c11, *c12, *c21, *c22;
	int i;
	char* s;
	int* s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10;
	size_t size;
	int subdim;
	char* p;
	int* p1, *p2, *p3, *p4, *p5, *p6, *p7;

	// No recursion for Base Case.
	if (dim == 1) {
		(*matrix_c) = (*matrix_a) * (*matrix_b);
		return;
	}

	// Allocate matrices.
	size = sizeof(int) * dim * dim / 4;
	s = malloc(size * 10);
	p = malloc(size * 7);
	if (!s || !p) {
		fprintf(stderr, "Unable to allocate matrices\n");
		exit(EXIT_FAILURE);
	}
	memset(s, 0, size * 10);
	memset(p, 0, size * 7);
	s1 = (int*)&s[size * 0]; s2 = (int*)&s[size * 1];
	s3 = (int*)&s[size * 2]; s4 = (int*)&s[size * 3];
	s5 = (int*)&s[size * 4]; s6 = (int*)&s[size * 5];
	s7 = (int*)&s[size * 6]; s8 = (int*)&s[size * 7];
	s9 = (int*)&s[size * 8]; s10 = (int*)&s[size * 9];
	p1 = (int*)&s[size * 0]; p2 = (int*)&s[size * 1];
	p3 = (int*)&s[size * 2]; p4 = (int*)&s[size * 3];
	p5 = (int*)&s[size * 4]; p6 = (int*)&s[size * 5]; 
	p7 = (int*)&s[size * 6];

	// Create submatrices.
	matrix_subs(matrix_a, dim, &a11, &a12, &a21, &a22);
	matrix_subs(matrix_b, dim, &b11, &b12, &b21, &b22);
	matrix_subs(matrix_c, dim, &c11, &c12, &c21, &c22);

	// Create S-matrices.
	subdim = dim / 2;
	matrix_subtract(b12, b22, subdim, s1);
	matrix_add(a11, a12, subdim, s2);
	matrix_add(a21, a22, subdim, s3);
	matrix_subtract(b21, b11, subdim, s4);
	matrix_add(a11, a22, subdim, s5);
	matrix_add(b11, b22, subdim, s6);
	matrix_subtract(a12, a22, subdim, s7);
	matrix_add(b21, b22, subdim, s8);
	matrix_subtract(a11, a21, subdim, s9);
	matrix_add(b11, b12, subdim, s10);

	// Create P-matrices.
	strassen(a11, s1, subdim, p1);
	strassen(s2, b22, subdim, p2);
	strassen(s3, b11, subdim, p3);
	strassen(a22, s4, subdim, p4);
	strassen(s5, s6, subdim, p5);
	strassen(s7, s8, subdim, p6);
	strassen(s9, s10, subdim, p7);

	// Create C-matrices.
	matrix_add(p5, p4, subdim, c11);
	matrix_subtract(c11, p2, subdim, c11);
	matrix_add(c11, p6, subdim, c11);
	matrix_add(p1, p2, subdim, c12);
	matrix_add(p3, p4, subdim, c21);
	matrix_add(p5, p1, subdim, c22);
	matrix_subtract(c22, p3, subdim, c22);
	matrix_subtract(c22, p7, subdim, c22);

	// Create C-matrix.
	matrix_join(matrix_c, dim, c11, c12, c21, c22);

	// Free matrices.
	free(s);
	free(p);
	free(a11); free(a12); free(a21); free(a22);
	free(b11); free(b12); free(b21); free(b22);
	free(c11); free(c12); free(c21); free(c22);
}

int main(int argc, char* argv[]) {
	int dim;
	int i;
	int j;
	int matrix_a[16];
	int matrix_b[16];
	int matrix_c[16];

	// Create matrices.
	dim = 4;
	srand(0xDEADBEEF);
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			int index = i * dim + j;
			matrix_a[index] = rand() % 10;
			matrix_b[index] = rand() % 10;
		}
	}
	memset(matrix_c, 0, sizeof(matrix_c));

	// Run matrix multiply.
	printf("A:\n");
	matrix_print(matrix_a, dim);
	printf("B:\n");
	matrix_print(matrix_b, dim);
	strassen(matrix_a, matrix_b, dim, matrix_c);
	printf("C:\n");
	matrix_print(matrix_c, dim);
}

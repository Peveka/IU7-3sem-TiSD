#ifndef DENSE_MATRIX_H
#define DENSE_MATRIX_H

#include "alloc.h"

void input_matrix_dimensions(int *rows, int *cols, const char *matrix_name);
error_t input_matrix_elements(dense_matrix_t *matrix);
error_t dense_matrix_full_input(dense_matrix_t **matrix);
void print_dense_matrix(const dense_matrix_t *matrix);

error_t dense_matrix_multiply(const dense_matrix_t *a, const dense_matrix_t *b, dense_matrix_t **result);
void dense_matrix_multiplication();

error_t dense_matrix_random(dense_matrix_t **matrix, int rows, int cols, double fill_percent);
error_t read_matrix_from_file(dense_matrix_t **matrix, const char *filename);

#endif
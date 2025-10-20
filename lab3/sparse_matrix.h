#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "alloc.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_menu();
int get_choice(int min, int max);
void clear_input_buffer();

void input_matrix_dimensions(int *rows, int *cols, const char *matrix_name);
error_t input_matrix_elements(dense_matrix_t *matrix);
void print_dense_matrix(const dense_matrix_t *matrix);
error_t dense_matrix_multiply(const dense_matrix_t *a, const dense_matrix_t *b, dense_matrix_t **result);
void dense_matrix_multiplication();

error_t dense_to_csr(const dense_matrix_t *dense, csr_matrix_t **csr);
error_t dense_to_csc(const dense_matrix_t *dense, csc_matrix_t **csc);
void print_csr_matrix(const csr_matrix_t *matrix);
void print_csc_matrix(const csc_matrix_t *matrix);
error_t csr_csc_multiply(const csr_matrix_t *csr, const csc_matrix_t *csc, csr_matrix_t **result);
void sparse_matrix_multiplication();
error_t input_csr_matrix(csr_matrix_t **csr);
error_t input_csc_matrix(csc_matrix_t **csc);
double read_double();

error_t dense_matrix_full_input(dense_matrix_t **matrix);
error_t dense_matrix_random(dense_matrix_t **matrix, int rows, int cols, double fill_percent);
error_t read_matrix_from_file(dense_matrix_t **matrix, const char *filename);
void compare_performance();
void measure_dense_multiply(const dense_matrix_t *a, const dense_matrix_t *b);
void measure_sparse_multiply(const csr_matrix_t *csr, const csc_matrix_t *csc);
double get_time();
size_t get_memory_usage();

#endif
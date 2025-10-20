#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "sparse_operations.h"

double get_time();
void measure_dense_multiply(const dense_matrix_t *a, const dense_matrix_t *b);
void measure_sparse_multiply(const csr_matrix_t *csr, const csc_matrix_t *csc);
void compare_performance();

#endif
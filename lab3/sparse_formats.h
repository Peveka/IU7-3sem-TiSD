#ifndef SPARSE_FORMATS_H
#define SPARSE_FORMATS_H

#include "alloc.h"

error_t dense_to_csr(const dense_matrix_t *dense, csr_matrix_t **csr);
error_t dense_to_csc(const dense_matrix_t *dense, csc_matrix_t **csc);

void print_csr_matrix(const csr_matrix_t *matrix);
void print_csc_matrix(const csc_matrix_t *matrix);

error_t input_csr_matrix(csr_matrix_t **csr);
error_t input_csc_matrix(csc_matrix_t **csc);

#endif
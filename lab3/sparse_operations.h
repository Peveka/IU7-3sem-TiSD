#ifndef SPARSE_OPERATIONS_H
#define SPARSE_OPERATIONS_H

#include "sparse_formats.h"

error_t csr_csc_multiply(const csr_matrix_t *csr, const csc_matrix_t *csc, csr_matrix_t **result);
void sparse_matrix_multiplication();

#endif
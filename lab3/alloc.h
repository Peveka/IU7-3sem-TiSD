#ifndef ALLOC_H
#define ALLOC_H

#include "errors.h"

typedef struct
{
    double **data;
    int rows;
    int cols;
} dense_matrix_t;

typedef struct
{
    double *A;
    int *JA;
    int *IA;
    int rows;
    int cols;
    int nnz;
} csr_matrix_t;

typedef struct
{
    double *A;
    int *IA;
    int *JA;
    int rows;
    int cols;
    int nnz;
} csc_matrix_t;

error_t dense_matrix_alloc(dense_matrix_t **matrix, int rows, int cols);
void dense_matrix_free(dense_matrix_t **matrix);

error_t csr_matrix_alloc(csr_matrix_t **matrix, int rows, int cols, int nnz);
void csr_matrix_free(csr_matrix_t **matrix);

error_t csc_matrix_alloc(csc_matrix_t **matrix, int rows, int cols, int nnz);
void csc_matrix_free(csc_matrix_t **matrix);

#endif
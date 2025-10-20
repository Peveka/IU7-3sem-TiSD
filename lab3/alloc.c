#include <stdlib.h>
#include "alloc.h"

void dense_matrix_free(dense_matrix_t** matrix)
{
    if (!matrix || !*matrix)
        return;

    if ((*matrix)->data)
    {
        for (int i = 0; i < (*matrix)->rows; i++)
        {
            free((*matrix)->data[i]);
        }
        free((*matrix)->data);
    }
    free(*matrix);
    *matrix = NULL;
}

error_t dense_matrix_alloc(dense_matrix_t** matrix, int rows, int cols)
{
    if (!matrix || rows <= 0 || cols <= 0)
        return ERR_INVALID_DATA;

    error_t rc = OK;
    
    *matrix = calloc(1, sizeof(dense_matrix_t));
    if (!(*matrix))
        return ERR_MEMORY_ALLOC;

    (*matrix)->rows = rows;
    (*matrix)->cols = cols;

    (*matrix)->data = calloc(rows, sizeof(double*));
    if (!(*matrix)->data)
    {
        free(*matrix);
        *matrix = NULL;
        return ERR_MEMORY_ALLOC;
    }

    for (int i = 0; i < rows && rc == OK; i++)
    {
        (*matrix)->data[i] = calloc(cols, sizeof(double));
        if (!(*matrix)->data[i])
        {
            dense_matrix_free(matrix);
            rc = ERR_MEMORY_ALLOC;
        }
    }

    return rc;
}

void csr_matrix_free(csr_matrix_t** matrix)
{
    if (!matrix || !*matrix)
        return;

    if ((*matrix)->A) free((*matrix)->A);
    if ((*matrix)->JA) free((*matrix)->JA);
    if ((*matrix)->IA) free((*matrix)->IA);
    
    free(*matrix);
    *matrix = NULL;
}

error_t csr_matrix_alloc(csr_matrix_t** matrix, int rows, int cols, int nnz)
{
    if (!matrix || rows <= 0 || cols <= 0 || nnz < 0)
        return ERR_INVALID_DATA;

    *matrix = calloc(1, sizeof(csr_matrix_t));
    if (!(*matrix))
        return ERR_MEMORY_ALLOC;

    (*matrix)->rows = rows;
    (*matrix)->cols = cols;
    (*matrix)->nnz = nnz;

    if (nnz > 0)
    {
        (*matrix)->A = malloc(nnz * sizeof(double));
        (*matrix)->JA = malloc(nnz * sizeof(int));
        if (!(*matrix)->A || !(*matrix)->JA)
        {
            csr_matrix_free(matrix);
            return ERR_MEMORY_ALLOC;
        }
    }

    (*matrix)->IA = calloc(rows + 1, sizeof(int));
    if (!(*matrix)->IA)
    {
        csr_matrix_free(matrix);
        return ERR_MEMORY_ALLOC;
    }

    return OK;
}

void csc_matrix_free(csc_matrix_t** matrix)
{
    if (!matrix || !*matrix)
        return;

    if ((*matrix)->A) free((*matrix)->A);
    if ((*matrix)->IA) free((*matrix)->IA);
    if ((*matrix)->JA) free((*matrix)->JA);
    
    free(*matrix);
    *matrix = NULL;
}

error_t csc_matrix_alloc(csc_matrix_t** matrix, int rows, int cols, int nnz)
{
    if (!matrix || rows <= 0 || cols <= 0 || nnz < 0)
        return ERR_INVALID_DATA;

    *matrix = calloc(1, sizeof(csc_matrix_t));
    if (!(*matrix))
        return ERR_MEMORY_ALLOC;

    (*matrix)->rows = rows;
    (*matrix)->cols = cols;
    (*matrix)->nnz = nnz;

    if (nnz > 0)
    {
        (*matrix)->A = malloc(nnz * sizeof(double));
        (*matrix)->IA = malloc(nnz * sizeof(int));
        if (!(*matrix)->A || !(*matrix)->IA)
        {
            csc_matrix_free(matrix);
            return ERR_MEMORY_ALLOC;
        }
    }

    (*matrix)->JA = calloc(cols + 1, sizeof(int));
    if (!(*matrix)->JA)
    {
        csc_matrix_free(matrix);
        return ERR_MEMORY_ALLOC;
    }

    return OK;
}
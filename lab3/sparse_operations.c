#include "sparse_operations.h"
#include "dense_matrix.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

error_t csr_csc_multiply(const csr_matrix_t *csr, const csc_matrix_t *csc, csr_matrix_t **result)
{
    if (!csr || !csc)
        return ERR_INVALID_DATA;

    if (csr->cols != csc->rows)
        return ERR_MATRIX_SIZE;

    dense_matrix_t *temp_result = NULL;
    error_t rc = dense_matrix_alloc(&temp_result, csr->rows, csc->cols);
    if (rc != OK)
        return rc;

    for (int i = 0; i < csr->rows; i++)
    {
        for (int j = 0; j < csc->cols; j++)
        {
            double sum = 0.0;

            int csr_start = csr->IA[i];
            int csr_end = csr->IA[i + 1];

            int csc_start = csc->JA[j];
            int csc_end = csc->JA[j + 1];

            int csr_idx = csr_start;
            int csc_idx = csc_start;

            while (csr_idx < csr_end && csc_idx < csc_end)
            {
                int csr_col = csr->JA[csr_idx];
                int csc_row = csc->IA[csc_idx];

                if (csr_col == csc_row)
                {
                    sum += csr->A[csr_idx] * csc->A[csc_idx];
                    csr_idx++;
                    csc_idx++;
                }
                else if (csr_col < csc_row)
                {
                    csr_idx++;
                }
                else
                {
                    csc_idx++;
                }
            }

            temp_result->data[i][j] = sum;
        }
    }

    rc = dense_to_csr(temp_result, result);

    dense_matrix_free(&temp_result);
    return rc;
}

void sparse_matrix_multiplication()
{
    printf("\n=== УМНОЖЕНИЕ CSR НА CSC ===\n");

    int input_method;
    printf("\nВыберите способ ввода:\n");
    printf("1. Координатный ввод (преобразуется в CSR/CSC)\n");
    printf("2. Прямой ввод CSR/CSC форматов\n");
    printf("Выберите опцию: ");
    input_method = get_choice(1, 2);

    csr_matrix_t *csr_a = NULL;
    csc_matrix_t *csc_b = NULL;
    csr_matrix_t *result = NULL;
    dense_matrix_t *dense_a = NULL;
    dense_matrix_t *dense_b = NULL;
    error_t rc = OK;

    if (input_method == 1)
    {
        printf("\n--- Матрица A (будет преобразована в CSR) ---\n");
        int rows_a, cols_a;
        input_matrix_dimensions(&rows_a, &cols_a, "A");

        rc = dense_matrix_alloc(&dense_a, rows_a, cols_a);
        if (rc != OK)
        {
            printf("Не удалось создать матрицу A! Код ошибки: %d\n", rc);
            return;
        }

        rc = input_matrix_elements(dense_a);
        if (rc != OK)
        {
            dense_matrix_free(&dense_a);
            return;
        }

        rc = dense_to_csr(dense_a, &csr_a);
        if (rc != OK)
        {
            printf("Ошибка преобразования A в CSR! Код ошибки: %d\n", rc);
            dense_matrix_free(&dense_a);
            return;
        }

        printf("\n--- Матрица B (будет преобразована в CSC) ---\n");
        int rows_b, cols_b;

        rows_b = cols_a;
        printf("Количество строк матрицы B установлено равным %d (по количеству столбцов A)\n", rows_b);

        while (1)
        {
            printf("Введите количество столбцов матрицы B: ");
            if (scanf("%d", &cols_b) == 1 && cols_b > 0)
                break;
            printf("Некорректный ввод! Количество столбцов должно быть положительным числом.\n");
            clear_input_buffer();
        }
        clear_input_buffer();

        rc = dense_matrix_alloc(&dense_b, rows_b, cols_b);
        if (rc != OK)
        {
            printf("Не удалось создать матрицу B! Код ошибки: %d\n", rc);
            dense_matrix_free(&dense_a);
            csr_matrix_free(&csr_a);
            return;
        }

        rc = input_matrix_elements(dense_b);
        if (rc != OK)
        {
            dense_matrix_free(&dense_a);
            dense_matrix_free(&dense_b);
            csr_matrix_free(&csr_a);
            return;
        }

        rc = dense_to_csc(dense_b, &csc_b);
        if (rc != OK)
        {
            printf("Ошибка преобразования B в CSC! Код ошибки: %d\n", rc);
            dense_matrix_free(&dense_a);
            dense_matrix_free(&dense_b);
            csr_matrix_free(&csr_a);
            return;
        }
    }
    else
    {
        printf("\n--- Матрица A (CSR формат) ---\n");
        rc = input_csr_matrix(&csr_a);
        if (rc != OK)
        {
            printf("Ошибка ввода CSR матрицы! Код ошибки: %d\n", rc);
            return;
        }

        printf("\n--- Матрица B (CSC формат) ---\n");
        printf("Количество строк матрицы B должно быть равно %d (столбцам A)\n", csr_a->cols);

        rc = input_csc_matrix(&csc_b);
        if (rc != OK)
        {
            printf("Ошибка ввода CSC матрицы! Код ошибки: %d\n", rc);
            csr_matrix_free(&csr_a);
            return;
        }

        if (csr_a->cols != csc_b->rows)
        {
            printf("Ошибка: размеры матриц несовместимы! A.cols(%d) != B.rows(%d)\n",
                   csr_a->cols, csc_b->rows);
            csr_matrix_free(&csr_a);
            csc_matrix_free(&csc_b);
            return;
        }
    }

    rc = csr_csc_multiply(csr_a, csc_b, &result);

    printf("\n--- Результаты ---\n");

    printf("\nМатрица A (CSR):");
    print_csr_matrix(csr_a);

    printf("\nМатрица B (CSC):");
    print_csc_matrix(csc_b);

    if (rc == OK)
    {
        printf("\nРезультат умножения A * B (CSR):");
        print_csr_matrix(result);

        if (input_method == 1)
        {
            printf("\nРезультат в обычном виде (для проверки):\n");
            dense_matrix_t *dense_result = NULL;
            dense_matrix_multiply(dense_a, dense_b, &dense_result);
            if (dense_result != NULL)
            {
                print_dense_matrix(dense_result);
                dense_matrix_free(&dense_result);
            }
        }
    }
    else
    {
        printf("\nОшибка умножения матриц! Код ошибки: %d\n", rc);
    }

    if (dense_a)
        dense_matrix_free(&dense_a);
    if (dense_b)
        dense_matrix_free(&dense_b);
    csr_matrix_free(&csr_a);
    csc_matrix_free(&csc_b);
    csr_matrix_free(&result);
}
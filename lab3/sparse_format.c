#include "sparse_formats.h"
#include "menu.h"

error_t dense_to_csr(const dense_matrix_t *dense, csr_matrix_t **csr)
{
    if (!dense || !dense->data)
        return ERR_INVALID_DATA;

    int nnz = 0;
    for (int i = 0; i < dense->rows; i++)
    {
        for (int j = 0; j < dense->cols; j++)
        {
            if (dense->data[i][j] != 0.0)
                nnz++;
        }
    }

    error_t rc = csr_matrix_alloc(csr, dense->rows, dense->cols, nnz);
    if (rc != OK)
        return rc;

    int idx = 0;
    (*csr)->IA[0] = 0;

    for (int i = 0; i < dense->rows; i++)
    {
        for (int j = 0; j < dense->cols; j++)
        {
            if (dense->data[i][j] != 0.0)
            {
                (*csr)->A[idx] = dense->data[i][j];
                (*csr)->JA[idx] = j;
                idx++;
            }
        }
        (*csr)->IA[i + 1] = idx;
    }

    return OK;
}

error_t dense_to_csc(const dense_matrix_t *dense, csc_matrix_t **csc)
{
    if (!dense || !dense->data)
        return ERR_INVALID_DATA;

    int nnz = 0;
    for (int j = 0; j < dense->cols; j++)
    {
        for (int i = 0; i < dense->rows; i++)
        {
            if (dense->data[i][j] != 0.0)
                nnz++;
        }
    }

    error_t rc = csc_matrix_alloc(csc, dense->rows, dense->cols, nnz);
    if (rc != OK)
        return rc;

    int idx = 0;
    (*csc)->JA[0] = 0;

    for (int j = 0; j < dense->cols; j++)
    {
        for (int i = 0; i < dense->rows; i++)
        {
            if (dense->data[i][j] != 0.0)
            {
                (*csc)->A[idx] = dense->data[i][j];
                (*csc)->IA[idx] = i;
                idx++;
            }
        }
        (*csc)->JA[j + 1] = idx;
    }

    return OK;
}

void print_csr_matrix(const csr_matrix_t *matrix)
{
    if (!matrix)
    {
        printf("CSR матрица не существует!\n");
        return;
    }

    printf("\nCSR матрица %dx%d (nnz=%d):\n", matrix->rows, matrix->cols, matrix->nnz);
    printf("A  (значения): ");
    for (int i = 0; i < matrix->nnz; i++)
        printf("%.2lf ", matrix->A[i]);
    printf("\n");

    printf("JA (столбцы): ");
    for (int i = 0; i < matrix->nnz; i++)
        printf("%d ", matrix->JA[i]);
    printf("\n");

    printf("IA (строки):  ");
    for (int i = 0; i <= matrix->rows; i++)
        printf("%d ", matrix->IA[i]);
    printf("\n");

    if (matrix->rows <= 10 && matrix->cols <= 10)
    {
        printf("В виде матрицы:\n");
        for (int i = 0; i < matrix->rows; i++)
        {
            double *row = calloc(matrix->cols, sizeof(double));

            for (int j = matrix->IA[i]; j < matrix->IA[i + 1]; j++)
            {
                int col = matrix->JA[j];
                row[col] = matrix->A[j];
            }

            for (int j = 0; j < matrix->cols; j++)
                printf("%8.2lf ", row[j]);
            printf("\n");

            free(row);
        }
    }
}

void print_csc_matrix(const csc_matrix_t *matrix)
{
    if (!matrix)
    {
        printf("CSC матрица не существует!\n");
        return;
    }

    printf("\nCSC матрица %dx%d (nnz=%d):\n", matrix->rows, matrix->cols, matrix->nnz);
    printf("A  (значения): ");
    for (int i = 0; i < matrix->nnz; i++)
        printf("%.2lf ", matrix->A[i]);
    printf("\n");

    printf("IA (строки):   ");
    for (int i = 0; i < matrix->nnz; i++)
        printf("%d ", matrix->IA[i]);
    printf("\n");

    printf("JA (столбцы):  ");
    for (int i = 0; i <= matrix->cols; i++)
        printf("%d ", matrix->JA[i]);
    printf("\n");

    if (matrix->rows <= 10 && matrix->cols <= 10)
    {
        printf("В виде матрицы:\n");
        double **dense_data = calloc(matrix->rows, sizeof(double *));
        for (int i = 0; i < matrix->rows; i++)
            dense_data[i] = calloc(matrix->cols, sizeof(double));

        for (int j = 0; j < matrix->cols; j++)
        {
            for (int idx = matrix->JA[j]; idx < matrix->JA[j + 1]; idx++)
            {
                int row = matrix->IA[idx];
                dense_data[row][j] = matrix->A[idx];
            }
        }

        for (int i = 0; i < matrix->rows; i++)
        {
            for (int j = 0; j < matrix->cols; j++)
                printf("%8.2lf ", dense_data[i][j]);
            printf("\n");

            free(dense_data[i]);
        }
        free(dense_data);
    }
}

error_t input_csr_matrix(csr_matrix_t **csr)
{
    printf("\n--- Прямой ввод CSR матрицы ---\n");

    int rows, cols, nnz;
    char buffer[1024];

    while (1)
    {
        printf("Введите количество строк: ");
        if (scanf("%d", &rows) == 1 && rows > 0)
            break;
        printf("Некорректный ввод!\n");
        clear_input_buffer();
    }

    while (1)
    {
        printf("Введите количество столбцов: ");
        if (scanf("%d", &cols) == 1 && cols > 0)
            break;
        printf("Некорректный ввод!\n");
        clear_input_buffer();
    }

    while (1)
    {
        printf("Введите количество ненулевых элементов (nnz): ");
        if (scanf("%d", &nnz) == 1 && nnz >= 0)
            break;
        printf("Некорректный ввод!\n");
        clear_input_buffer();
    }

    clear_input_buffer();

    error_t rc = csr_matrix_alloc(csr, rows, cols, nnz);
    if (rc != OK)
        return rc;

    if (nnz > 0)
    {
        printf("\nВведите массив A (%d значений через пробел): ", nnz);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            char *token = strtok(buffer, " \t\n");
            int count = 0;

            while (token != NULL && count < nnz)
            {
                if (sscanf(token, "%lf", &(*csr)->A[count]) == 1)
                {
                    count++;
                }
                token = strtok(NULL, " \t\n");
            }

            if (count != nnz)
            {
                printf("Ошибка: введено %d значений вместо %d!\n", count, nnz);
                csr_matrix_free(csr);
                return ERR_INPUT;
            }
        }

        printf("Введите массив JA (%d номеров столбцов через пробел): ", nnz);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            char *token = strtok(buffer, " \t\n");
            int count = 0;

            while (token != NULL && count < nnz)
            {
                if (sscanf(token, "%d", &(*csr)->JA[count]) == 1)
                {
                    if ((*csr)->JA[count] < 0 || (*csr)->JA[count] >= cols)
                    {
                        printf("Ошибка: столбец %d вне диапазона [0, %d]!\n",
                               (*csr)->JA[count], cols - 1);
                        csr_matrix_free(csr);
                        return ERR_INPUT;
                    }
                    count++;
                }
                token = strtok(NULL, " \t\n");
            }

            if (count != nnz)
            {
                printf("Ошибка: введено %d значений вместо %d!\n", count, nnz);
                csr_matrix_free(csr);
                return ERR_INPUT;
            }
        }
    }

    printf("Введите массив IA (%d указателей через пробел): ", rows + 1);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        char *token = strtok(buffer, " \t\n");
        int count = 0;

        while (token != NULL && count <= rows)
        {
            if (sscanf(token, "%d", &(*csr)->IA[count]) == 1)
            {
                if (count > 0 && (*csr)->IA[count] < (*csr)->IA[count - 1])
                {
                    printf("Ошибка: IA должен быть монотонно возрастающим!\n");
                    csr_matrix_free(csr);
                    return ERR_INPUT;
                }

                if ((*csr)->IA[count] < 0 || (*csr)->IA[count] > nnz)
                {
                    printf("Ошибка: указатель %d вне диапазона [0, %d]!\n",
                           (*csr)->IA[count], nnz);
                    csr_matrix_free(csr);
                    return ERR_INPUT;
                }

                count++;
            }
            token = strtok(NULL, " \t\n");
        }

        if (count != rows + 1)
        {
            printf("Ошибка: введено %d значений вместо %d!\n", count, rows + 1);
            csr_matrix_free(csr);
            return ERR_INPUT;
        }

        if ((*csr)->IA[rows] != nnz)
        {
            printf("Ошибка: IA[%d] должен быть равен %d (nnz)!\n", rows, nnz);
            csr_matrix_free(csr);
            return ERR_INPUT;
        }
    }

    return OK;
}

error_t input_csc_matrix(csc_matrix_t **csc)
{
    printf("\n--- Прямой ввод CSC матрицы ---\n");

    int rows, cols, nnz;
    char buffer[1024];

    while (1)
    {
        printf("Введите количество строк: ");
        if (scanf("%d", &rows) == 1 && rows > 0)
            break;
        printf("Некорректный ввод!\n");
        clear_input_buffer();
    }

    while (1)
    {
        printf("Введите количество столбцов: ");
        if (scanf("%d", &cols) == 1 && cols > 0)
            break;
        printf("Некорректный ввод!\n");
        clear_input_buffer();
    }

    while (1)
    {
        printf("Введите количество ненулевых элементов (nnz): ");
        if (scanf("%d", &nnz) == 1 && nnz >= 0)
            break;
        printf("Некорректный ввод!\n");
        clear_input_buffer();
    }

    clear_input_buffer();

    error_t rc = csc_matrix_alloc(csc, rows, cols, nnz);
    if (rc != OK)
        return rc;

    if (nnz > 0)
    {
        printf("\nВведите массив A (%d значений через пробел): ", nnz);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            char *token = strtok(buffer, " \t\n");
            int count = 0;

            while (token != NULL && count < nnz)
            {
                if (sscanf(token, "%lf", &(*csc)->A[count]) == 1)
                {
                    count++;
                }
                token = strtok(NULL, " \t\n");
            }

            if (count != nnz)
            {
                printf("Ошибка: введено %d значений вместо %d!\n", count, nnz);
                csc_matrix_free(csc);
                return ERR_INPUT;
            }
        }

        printf("Введите массив IA (%d номеров строк через пробел): ", nnz);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            char *token = strtok(buffer, " \t\n");
            int count = 0;

            while (token != NULL && count < nnz)
            {
                if (sscanf(token, "%d", &(*csc)->IA[count]) == 1)
                {
                    if ((*csc)->IA[count] < 0 || (*csc)->IA[count] >= rows)
                    {
                        printf("Ошибка: строка %d вне диапазона [0, %d]!\n",
                               (*csc)->IA[count], rows - 1);
                        csc_matrix_free(csc);
                        return ERR_INPUT;
                    }
                    count++;
                }
                token = strtok(NULL, " \t\n");
            }

            if (count != nnz)
            {
                printf("Ошибка: введено %d значений вместо %d!\n", count, nnz);
                csc_matrix_free(csc);
                return ERR_INPUT;
            }
        }
    }

    printf("Введите массив JA (%d указателей через пробел): ", cols + 1);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        char *token = strtok(buffer, " \t\n");
        int count = 0;

        while (token != NULL && count <= cols)
        {
            if (sscanf(token, "%d", &(*csc)->JA[count]) == 1)
            {
                if (count > 0 && (*csc)->JA[count] < (*csc)->JA[count - 1])
                {
                    printf("Ошибка: JA должен быть монотонно возрастающим!\n");
                    csc_matrix_free(csc);
                    return ERR_INPUT;
                }

                if ((*csc)->JA[count] < 0 || (*csc)->JA[count] > nnz)
                {
                    printf("Ошибка: указатель %d вне диапазона [0, %d]!\n",
                           (*csc)->JA[count], nnz);
                    csc_matrix_free(csc);
                    return ERR_INPUT;
                }

                count++;
            }
            token = strtok(NULL, " \t\n");
        }

        if (count != cols + 1)
        {
            printf("Ошибка: введено %d значений вместо %d!\n", count, cols + 1);
            csc_matrix_free(csc);
            return ERR_INPUT;
        }

        if ((*csc)->JA[cols] != nnz)
        {
            printf("Ошибка: JA[%d] должен быть равен %d (nnz)!\n", cols, nnz);
            csc_matrix_free(csc);
            return ERR_INPUT;
        }
    }

    return OK;
}
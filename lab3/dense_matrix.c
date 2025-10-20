#include "dense_matrix.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

error_t dense_matrix_multiply(const dense_matrix_t *a, const dense_matrix_t *b, dense_matrix_t **result)
{
    if (!a || !b || !a->data || !b->data)
        return ERR_INVALID_DATA;

    if (a->cols != b->rows)
        return ERR_MATRIX_SIZE;

    error_t rc = dense_matrix_alloc(result, a->rows, b->cols);
    if (rc != OK)
        return rc;

    for (int i = 0; i < a->rows; i++)
    {
        for (int j = 0; j < b->cols; j++)
        {
            (*result)->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++)
            {
                (*result)->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }

    return OK;
}

void dense_matrix_multiplication()
{
    printf("\n=== УМНОЖЕНИЕ ОБЫЧНЫХ МАТРИЦ ===\n");

    printf("\n--- Матрица A ---\n");
    int rows_a, cols_a;
    input_matrix_dimensions(&rows_a, &cols_a, "A");

    dense_matrix_t *matrix_a = NULL;
    error_t rc = dense_matrix_alloc(&matrix_a, rows_a, cols_a);
    if (rc != OK)
        return;

    rc = input_matrix_elements(matrix_a);
    if (rc != OK)
    {
        dense_matrix_free(&matrix_a);
        return;
    }

    printf("\n--- Матрица B ---\n");
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

    dense_matrix_t *matrix_b = NULL;
    rc = dense_matrix_alloc(&matrix_b, rows_b, cols_b);
    if (rc != OK)
    {
        dense_matrix_free(&matrix_a);
        return;
    }

    rc = input_matrix_elements(matrix_b);
    if (rc != OK)
    {
        dense_matrix_free(&matrix_a);
        dense_matrix_free(&matrix_b);
        return;
    }

    dense_matrix_t *result = NULL;
    rc = dense_matrix_multiply(matrix_a, matrix_b, &result);

    printf("\n--- Результаты ---\n");

    printf("\nМатрица A:\n");
    print_dense_matrix(matrix_a);

    printf("\nМатрица B:\n");
    print_dense_matrix(matrix_b);

    if (rc == OK)
    {
        printf("\nРезультат умножения A * B:\n");
        print_dense_matrix(result);
    }
    else
    {
        printf("\nОшибка умножения матриц! Код ошибки: %d\n", rc);
    }

    dense_matrix_free(&matrix_a);
    dense_matrix_free(&matrix_b);
    dense_matrix_free(&result);
}

void input_matrix_dimensions(int *rows, int *cols, const char *matrix_name)
{
    printf("\n--- Ввод размеров матрицы %s ---\n", matrix_name);

    while (1)
    {
        printf("Введите количество строк: ");
        if (scanf("%d", rows) == 1 && *rows > 0)
            break;
        printf("Некорректный ввод! Количество строк должно быть положительным числом.\n");
        clear_input_buffer();
    }

    while (1)
    {
        printf("Введите количество столбцов: ");
        if (scanf("%d", cols) == 1 && *cols > 0)
            break;
        printf("Некорректный ввод! Количество столбцов должно быть положительным числом.\n");
        clear_input_buffer();
    }

    clear_input_buffer();
}

error_t input_matrix_elements(dense_matrix_t *matrix)
{
    if (!matrix || !matrix->data)
        return ERR_INVALID_DATA;

    printf("\n--- Координатный ввод элементов ---\n");
    printf("Формат ввода: строка столбец значение\n");
    printf("Для завершения ввода введите -1 для любого параметра\n");
    printf("Диапазон строк: 0-%d, столбцов: 0-%d\n", matrix->rows - 1, matrix->cols - 1);

    int row, col;
    double value;
    int element_count = 0;

    while (1)
    {
        printf("Введите %d-ый элемент (строка столбец значение): ", element_count + 1);

        if (scanf("%d", &row) != 1)
        {
            printf("Некорректный ввод строки!\n");
            clear_input_buffer();
            continue;
        }

        if (row == -1)
        {
            printf("Завершение ввода.\n");
            clear_input_buffer();
            break;
        }

        if (scanf("%d", &col) != 1)
        {
            printf("Некорректный ввод столбца!\n");
            clear_input_buffer();
            continue;
        }

        if (col == -1)
        {
            printf("Завершение ввода.\n");
            clear_input_buffer();
            break;
        }

        if (scanf("%lf", &value) != 1)
        {
            printf("Некорректный ввод значения!\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer();

        if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols)
        {
            printf("Ошибка: координаты [%d,%d] выходят за границы матрицы [0-%d, 0-%d]!\n",
                   row, col, matrix->rows - 1, matrix->cols - 1);
            continue;
        }

        matrix->data[row][col] = value;
        element_count++;

        printf("Записан элемент matrix[%d][%d] = %lf\n", row, col, value);
    }

    printf("Введено %d ненулевых элементов.\n", element_count);
    return OK;
}

void print_dense_matrix(const dense_matrix_t *matrix)
{
    if (!matrix || !matrix->data)
    {
        printf("Матрица не существует!\n");
        return;
    }

    printf("\nМатрица %dx%d:\n", matrix->rows, matrix->cols);

    if (matrix->rows > 10 || matrix->cols > 10)
    {
        printf("(Матрица слишком большая для полного отображения)\n");
        printf("Первые 5x5 элементов:\n");

        int show_rows = (matrix->rows < 5) ? matrix->rows : 5;
        int show_cols = (matrix->cols < 5) ? matrix->cols : 5;

        for (int i = 0; i < show_rows; i++)
        {
            for (int j = 0; j < show_cols; j++)
            {
                printf("%8.2lf ", matrix->data[i][j]);
            }
            if (show_cols < matrix->cols)
                printf("...");
            printf("\n");
        }
        if (show_rows < matrix->rows)
            printf("...\n");
    }
    else
    {
        for (int i = 0; i < matrix->rows; i++)
        {
            for (int j = 0; j < matrix->cols; j++)
            {
                printf("%8.2lf ", matrix->data[i][j]);
            }
            printf("\n");
        }
    }
}

error_t dense_matrix_full_input(dense_matrix_t **matrix)
{
    int rows, cols;
    input_matrix_dimensions(&rows, &cols, "");

    error_t rc = dense_matrix_alloc(matrix, rows, cols);
    if (rc != OK)
        return rc;

    printf("\n--- Полный ввод матрицы %dx%d ---\n", rows, cols);
    printf("Введите все элементы построчно:\n");

    for (int i = 0; i < rows; i++)
    {
        printf("Строка %d (%d элементов через пробел): ", i, cols);

        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            char *token = strtok(buffer, " \t\n");
            int j = 0;

            while (token != NULL && j < cols)
            {
                if (sscanf(token, "%lf", &(*matrix)->data[i][j]) == 1)
                {
                    j++;
                }
                token = strtok(NULL, " \t\n");
            }

            if (j != cols)
            {
                printf("Ошибка: введено %d элементов вместо %d!\n", j, cols);
                dense_matrix_free(matrix);
                return ERR_INPUT;
            }
        }
    }

    return OK;
}

error_t dense_matrix_random(dense_matrix_t **matrix, int rows, int cols, double fill_percent)
{
    if (fill_percent < 0 || fill_percent > 100)
        return ERR_INVALID_DATA;

    error_t rc = dense_matrix_alloc(matrix, rows, cols);
    if (rc != OK)
        return rc;

    srand(time(NULL));
    int total_elements = rows * cols;
    int non_zero_elements = (int)(total_elements * fill_percent / 100.0);

    for (int i = 0; i < non_zero_elements; i++)
    {
        int row, col;
        do
        {
            row = rand() % rows;
            col = rand() % cols;
        } while ((*matrix)->data[row][col] != 0.0);

        (*matrix)->data[row][col] = (rand() % 1000) / 10.0;
    }

    return OK;
}

error_t read_matrix_from_file(dense_matrix_t **matrix, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return ERR_FILE_OPEN;

    int rows, cols;
    if (fscanf(file, "%d %d", &rows, &cols) != 2)
    {
        fclose(file);
        return ERR_FILE_READ;
    }

    if (rows <= 0 || cols <= 0)
    {
        fclose(file);
        return ERR_INVALID_DATA;
    }

    error_t rc = dense_matrix_alloc(matrix, rows, cols);
    if (rc != OK)
    {
        fclose(file);
        return rc;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (fscanf(file, "%lf", &(*matrix)->data[i][j]) != 1)
            {
                dense_matrix_free(matrix);
                fclose(file);
                return ERR_FILE_READ;
            }
        }
    }

    fclose(file);
    return OK;
}
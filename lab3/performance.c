#include "performance.h"
#include "dense_matrix.h"
#include "sparse_formats.h"
#include "menu.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

double get_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

void measure_dense_multiply(const dense_matrix_t *a, const dense_matrix_t *b)
{
    printf("Измерение времени для плотного умножения...\n");

    double start_time = get_time();

    dense_matrix_t *result = NULL;
    error_t rc = dense_matrix_multiply(a, b, &result);

    double end_time = get_time();
    double elapsed = end_time - start_time;

    if (rc == OK)
    {
        printf("Плотное умножение: %.6f секунд\n", elapsed);
        dense_matrix_free(&result);
    }
    else
    {
        printf("Ошибка плотного умножения: %d\n", rc);
    }
}

void measure_sparse_multiply(const csr_matrix_t *csr, const csc_matrix_t *csc)
{
    printf("Измерение времени для разреженного умножения...\n");

    double start_time = get_time();

    csr_matrix_t *result = NULL;
    error_t rc = csr_csc_multiply(csr, csc, &result);

    double end_time = get_time();
    double elapsed = end_time - start_time;

    if (rc == OK)
    {
        printf("Разреженное умножение: %.6f секунд\n", elapsed);
        csr_matrix_free(&result);
    }
    else
    {
        printf("Ошибка разреженного умножения: %d\n", rc);
    }
}

void compare_performance()
{
    printf("\n=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ===\n");

    int choice;
    printf("\nВыберите способ создания матриц:\n");
    printf("1. Ручной ввод\n");
    printf("2. Случайное заполнение\n");
    printf("3. Загрузка из файла\n");
    printf("Выберите опцию: ");
    choice = get_choice(1, 3);

    dense_matrix_t *dense_a = NULL;
    dense_matrix_t *dense_b = NULL;
    csr_matrix_t *csr_a = NULL;
    csc_matrix_t *csc_b = NULL;
    error_t rc = OK;

    if (choice == 1)
    {
        printf("\n--- Матрица A ---\n");
        rc = dense_matrix_full_input(&dense_a);
        if (rc != OK)
        {
            printf("Ошибка ввода матрицы A: %d\n", rc);
            return;
        }

        printf("\n--- Матрица B ---\n");
        printf("Количество строк должно быть равно %d\n", dense_a->cols);
        int rows_b = dense_a->cols;
        int cols_b;

        while (1)
        {
            printf("Введите количество столбцов матрицы B: ");
            if (scanf("%d", &cols_b) == 1 && cols_b > 0)
                break;
            printf("Некорректный ввод!\n");
            clear_input_buffer();
        }
        clear_input_buffer();

        dense_matrix_t *temp = NULL;
        rc = dense_matrix_alloc(&temp, rows_b, cols_b);
        if (rc != OK)
        {
            printf("Не удалось создать матрицу B: %d\n", rc);
            dense_matrix_free(&dense_a);
            return;
        }

        printf("Введите все элементы матрицы B %dx%d:\n", rows_b, cols_b);
        for (int i = 0; i < rows_b; i++)
        {
            printf("Строка %d (%d элементов): ", i, cols_b);
            for (int j = 0; j < cols_b; j++)
            {
                if (scanf("%lf", &temp->data[i][j]) != 1)
                {
                    printf("Ошибка ввода элемента [%d][%d]\n", i, j);
                    dense_matrix_free(&dense_a);
                    dense_matrix_free(&temp);
                    return;
                }
            }
        }
        dense_b = temp;
    }
    else if (choice == 2)
    {
        int rows_a, cols_a, rows_b, cols_b;
        double fill_percent;

        printf("\n--- Параметры матрицы A ---\n");
        input_matrix_dimensions(&rows_a, &cols_a, "A");

        printf("Введите процент заполнения (0-100): ");
        if (scanf("%lf", &fill_percent) != 1 || fill_percent < 0 || fill_percent > 100)
        {
            printf("Некорректный процент заполнения!\n");
            clear_input_buffer();
            return;
        }

        printf("\n--- Параметры матрицы B ---\n");
        rows_b = cols_a;
        printf("Количество строк B: %d\n", rows_b);

        while (1)
        {
            printf("Введите количество столбцов матрицы B: ");
            if (scanf("%d", &cols_b) == 1 && cols_b > 0)
                break;
            printf("Некорректный ввод!\n");
            clear_input_buffer();
        }

        printf("Введите процент заполнения (0-100): ");
        double fill_percent_b;
        if (scanf("%lf", &fill_percent_b) != 1 || fill_percent_b < 0 || fill_percent_b > 100)
        {
            printf("Некорректный процент заполнения!\n");
            clear_input_buffer();
            return;
        }

        clear_input_buffer();

        rc = dense_matrix_random(&dense_a, rows_a, cols_a, fill_percent);
        if (rc != OK)
        {
            printf("Ошибка создания случайной матрицы A: %d\n", rc);
            return;
        }

        rc = dense_matrix_random(&dense_b, rows_b, cols_b, fill_percent_b);
        if (rc != OK)
        {
            printf("Ошибка создания случайной матрицы B: %d\n", rc);
            dense_matrix_free(&dense_a);
            return;
        }

        printf("Созданы случайные матрицы:\n");
        printf("A: %dx%d, заполнение: %.1f%%\n", rows_a, cols_a, fill_percent);
        printf("B: %dx%d, заполнение: %.1f%%\n", rows_b, cols_b, fill_percent_b);
    }
    else
    {
        char filename_a[100], filename_b[100];

        printf("Введите имя файла для матрицы A: ");
        int res = scanf("%99s", filename_a);

        printf("Введите имя файла для матрицы B: ");
        res = scanf("%99s", filename_b);

        clear_input_buffer();

        rc = read_matrix_from_file(&dense_a, filename_a);
        if (rc != OK)
        {
            printf("Ошибка загрузки матрицы A из файла: %d\n", rc);
            return;
        }

        rc = read_matrix_from_file(&dense_b, filename_b);
        if (rc != OK)
        {
            printf("Ошибка загрузки матрицы B из файла: %d\n", rc);
            dense_matrix_free(&dense_a);
            return;
        }

        if (dense_a->cols != dense_b->rows)
        {
            printf("Ошибка: размеры матриц несовместимы! A.cols(%d) != B.rows(%d)\n",
                   dense_a->cols, dense_b->rows);
            dense_matrix_free(&dense_a);
            dense_matrix_free(&dense_b);
            return;
        }

        printf("Матрицы загружены из файлов:\n");
        printf("A: %dx%d\n", dense_a->rows, dense_a->cols);
        printf("B: %dx%d\n", dense_b->rows, dense_b->cols);
    }

    rc = dense_to_csr(dense_a, &csr_a);
    if (rc != OK)
    {
        printf("Ошибка преобразования A в CSR: %d\n", rc);
        dense_matrix_free(&dense_a);
        dense_matrix_free(&dense_b);
        csr_matrix_free(&csr_a);
        csc_matrix_free(&csc_b);
    }

    rc = dense_to_csc(dense_b, &csc_b);
    if (rc != OK)
    {
        printf("Ошибка преобразования B в CSC: %d\n", rc);
        dense_matrix_free(&dense_a);
        dense_matrix_free(&dense_b);
        csr_matrix_free(&csr_a);
        csc_matrix_free(&csc_b);
    }

    printf("\n--- РЕЗУЛЬТАТЫ ИЗМЕРЕНИЙ ---\n");

    measure_dense_multiply(dense_a, dense_b);
    measure_sparse_multiply(csr_a, csc_b);

    printf("\n--- СТАТИСТИКА ---\n");
    printf("Размеры: A(%dx%d) * B(%dx%d)\n",
           dense_a->rows, dense_a->cols, dense_b->rows, dense_b->cols);
    printf("Ненулевых элементов в A: %d\n", csr_a->nnz);
    printf("Ненулевых элементов в B: %d\n", csc_b->nnz);
    printf("Заполнение A: %.2f%%\n", (csr_a->nnz * 100.0) / (dense_a->rows * dense_a->cols));
    printf("Заполнение B: %.2f%%\n", (csc_b->nnz * 100.0) / (dense_b->rows * dense_b->cols));
}
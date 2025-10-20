#include "sparse_matrix.h"

void input_matrix_dimensions(int* rows, int* cols, const char* matrix_name)
{
    printf("\n--- Ввод размеров матрицы %s ---\n", matrix_name);
    
    while (1)
    {
        printf("Введите количество строк: ");
        if (scanf("%d", rows) == 1 && *rows > 0)
        {
            break;
        }
        printf("Некорректный ввод! Количество строк должно быть положительным числом.\n");
        clear_input_buffer();
    }
    
    while (1)
    {
        printf("Введите количество столбцов: ");
        if (scanf("%d", cols) == 1 && *cols > 0)
        {
            break;
        }
        printf("Некорректный ввод! Количество столбцов должно быть положительным числом.\n");
        clear_input_buffer();
    }
    
    clear_input_buffer();
}

error_t input_matrix_elements(dense_matrix_t* matrix)
{
    if (!matrix || !matrix->data)
        return ERR_INVALID_DATA;

    printf("\n--- Координатный ввод ненулевых элементов ---\n");
    printf("Формат ввода: строка столбец значение\n");
    printf("Для завершения ввода введите -1 для любого параметра\n");
    printf("Диапазон строк: 0-%d, столбцов: 0-%d\n", matrix->rows - 1, matrix->cols - 1);
    
    int row, col;
    double value;
    int element_count = 0;
    
    while (1)
    {
        printf("Введите элемент %d (строка столбец значение): ", element_count + 1);
        
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
        
        // Проверяем границы
        if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols)
        {
            printf("Ошибка: координаты [%d,%d] выходят за границы матрицы [0-%d, 0-%d]!\n", 
                   row, col, matrix->rows - 1, matrix->cols - 1);
            continue;
        }
        
        // Записываем значение
        matrix->data[row][col] = value;
        element_count++;
        
        printf("Записан элемент matrix[%d][%d] = %lf\n", row, col, value);
    }
    
    printf("Введено %d ненулевых элементов.\n", element_count);
    return OK;
}

void print_dense_matrix(const dense_matrix_t* matrix)
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
            if (show_cols < matrix->cols) printf("...");
            printf("\n");
        }
        if (show_rows < matrix->rows) printf("...\n");
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

// Функция для тестирования ввода обычной матрицы
void dense_matrix_input_test()
{
    printf("\n=== ТЕСТ ВВОДА ОБЫЧНОЙ МАТРИЦЫ ===\n");
    
    int rows, cols;
    input_matrix_dimensions(&rows, &cols, "A");
    
    dense_matrix_t* matrix = NULL;
    error_t rc = dense_matrix_alloc(&matrix, rows, cols);
    
    if (rc != OK)
    {
        printf("Не удалось создать матрицу! Код ошибки: %d\n", rc);
        return;
    }
    
    rc = input_matrix_elements(matrix);
    if (rc == OK)
    {
        print_dense_matrix(matrix);
    }
    
    dense_matrix_free(&matrix);
}
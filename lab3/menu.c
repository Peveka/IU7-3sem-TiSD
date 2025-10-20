#include "sparse_matrix.h"

void print_menu()
{
    printf("\n=== УМНОЖЕНИЕ МАТРИЦ ===\n");
    printf("1. Умножение обычных матриц\n");
    printf("2. Умножение CSR на CSC\n");
    printf("3. Сравнение производительности\n");
    printf("4. Выход\n");
    printf("Выберите опцию: ");
}

int get_choice(int min, int max)
{
    int choice;
    
    while (1)
    {
        if (scanf("%d", &choice) == 1)
        {
            if (choice >= min && choice <= max)
            {
                clear_input_buffer();
                return choice;
            }
        }
        clear_input_buffer();
        printf("Некорректный ввод! Введите число от %d до %d: ", min, max);
    }
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
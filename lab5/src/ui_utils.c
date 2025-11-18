#include "ui_utils.h"
#include <stdio.h>
void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void)
{
    printf("\n=====================================\n");
    printf("           МЕНЮ СИМУЛЯЦИИ\n");
    printf("=====================================\n");
    printf("1. Симуляция на списке (полный вывод)\n");
    printf("2. Симуляция на массиве (полный вывод)\n");
    printf("3. Замер времени работы\n");
    printf("4. Замер используемой памяти\n");
    printf("5. Анализ выделенных участков памяти\n");
    printf("6. Сменить параметры T1/T2 (текущие T1_max=%.2lf, T2_max=%.2lf)\n", g_t1_max, g_t2_max);
    printf("0. Выход\n");
    printf("=====================================\n");
}

int scan_choice(void)
{
    int choice;
    while (1) 
    {
        printf("Выберите действие: ");
        if (scanf("%d", &choice) == 1) 
        {
            clear_input(); 
            if (choice >= 0 && choice <= 6) 
            {
                return choice;
            } 
            else 
            {
                printf("Неверный выбор: введите число от 0 до 6.\n");
            }
        } 
        else 
        {
            printf("Ошибка ввода: введите целое число.\n");
            clear_input(); 
        }
    }
}
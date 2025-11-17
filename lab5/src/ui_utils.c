#include "ui_utils.h"
#include <stdio.h>
void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void)
{
    printf("=====================================\n");
    printf("1. Запустить симуляцию на Списке\n");
    printf("2. Запустить симуляцию на Массиве\n");
    printf("3. Сменить параметры T1/T2 (текущие T1_max=%.2lf, T2_max=%.2lf)\n", g_t1_max, g_t2_max);
    printf("0. Выход\n");
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
            if (choice >= 0 && choice <= 3) 
            {
                return choice;
            } 
            else 
            {
                printf("Неверный выбор: введите число от 0 до 3.\n");
            }
        } 
        else 
        {
            printf("Ошибка ввода: введите целое число.\n");
            clear_input(); 
        }
    }
}
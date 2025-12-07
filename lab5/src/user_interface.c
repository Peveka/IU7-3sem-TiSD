#include "../inc/user_interface.h"
#include <stdio.h>

void display_main_menu(const simulation_config_t *config)
{
    printf("\n");
    printf("=== СИСТЕМА АНАЛИЗА ОЧЕРЕДЕЙ (Вариант 7) ===\n");
    printf("Текущие настройки:\n");
    printf("  Время между заявками (T1): %.1f - %.1f\n", config->min_arrival_time, config->max_arrival_time);
    printf("  Время обработки (T2): %.1f - %.1f\n", config->min_processing_time, config->max_processing_time);
    printf("  Количество заявок: %d\n", config->total_requests);
    printf("  Циклов обработки: %d\n", config->processing_cycles);
    printf("\n");
    printf("1 - Запуск моделирования (массив)\n");
    printf("2 - Запуск моделирования (список)\n");
    printf("3 - Просмотр операций памяти\n");
    printf("4 - Тестирование скорости\n");
    printf("5 - Тестирование памяти\n");
    printf("6 - Настройка времени заявок (T1)\n");
    printf("7 - Настройка времени обработки (T2)\n");
    printf("8 - Сравнительный анализ\n");
    printf("9 - Сравнение производительности (память + время)\n");  // Новый пункт
    printf("0 - Выход\n");
    printf("-------------------------------------------\n");
}

int get_user_choice(int min_option, int max_option)
{
    int choice = -1;

    while (choice < min_option || choice > max_option)
    {
        printf("Выбор [%d-%d]: ", min_option, max_option);

        if (scanf("%d", &choice) != 1)
        {
            printf("Ошибка ввода. Введите число.\n");
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);
            choice = -1;
        }
        else if (choice < min_option || choice > max_option)
        {
            printf("Неверный выбор. Введите число от %d до %d.\n", min_option, max_option);
        }
    }

    return choice;
}

void modify_arrival_times(simulation_config_t *config)
{
    printf("\n--- Настройка времени между заявками (T1) ---\n");

    while (1)
    {
        printf("Введите мин и макс время (через пробел): ");

        if (scanf("%lf %lf", &config->min_arrival_time, &config->max_arrival_time) != 2)
        {
            printf("Ошибка формата.\n");
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);
            continue;
        }

        if (config->min_arrival_time >= config->max_arrival_time)
        {
            printf("Мин значение должно быть меньше макс.\n");
        }
        else if (config->min_arrival_time < 0 || config->max_arrival_time < 0)
        {
            printf("Значения должны быть положительными.\n");
        }
        else
        {
            printf("Установлено T1: от %.1f до %.1f\n", config->min_arrival_time, config->max_arrival_time);
            break;
        }
    }
}

void modify_processing_times(simulation_config_t *config)
{
    printf("\n--- Настройка времени обработки (T2) ---\n");

    while (1)
    {
        printf("Введите мин и макс время обработки (через пробел): ");

        if (scanf("%lf %lf", &config->min_processing_time, &config->max_processing_time) != 2)
        {
            printf("Ошибка формата.\n");
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);
            continue;
        }

        if (config->min_processing_time >= config->max_processing_time)
        {
            printf("Мин значение должно быть меньше макс.\n");
        }
        else if (config->min_processing_time < 0 || config->max_processing_time < 0)
        {
            printf("Значения должны быть положительными.\n");
        }
        else
        {
            printf("Установлено T2: от %.1f до %.1f\n", config->min_processing_time, config->max_processing_time);
            break;
        }
    }
}
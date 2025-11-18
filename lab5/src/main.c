#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "smol_simulator.h" 
#include "ui_utils.h"       

int main() 
{
    srand(time(NULL)); 

    int choice;
    
    do {
        print_menu();
        choice = scan_choice();

        switch (choice) {
            case 1:
                // Симуляция на списке с полным выводом
                printf("\n=== РЕЖИМ 1: СИМУЛЯЦИЯ НА СПИСКЕ ===\n");
                simulate_smol(1, 1, 0, 0, 0);
                break;
            case 2:
                // Симуляция на массиве с полным выводом
                printf("\n=== РЕЖИМ 2: СИМУЛЯЦИЯ НА МАССИВЕ ===\n");
                simulate_smol(2, 1, 0, 0, 0);
                break;
            case 3:
                // Замер времени работы
                printf("\n=== РЕЖИМ 3: ЗАМЕР ВРЕМЕНИ РАБОТЫ ===\n");
                printf("Тестирование на списке...\n");
                simulate_smol(1, 0, 1, 0, 0);
                printf("\nТестирование на массиве...\n");
                simulate_smol(2, 0, 1, 0, 0);
                break;
            case 4:
                // Замер используемой памяти
                printf("\n=== РЕЖИМ 4: ЗАМЕР ИСПОЛЬЗУЕМОЙ ПАМЯТИ ===\n");
                printf("Тестирование на списке...\n");
                simulate_smol(1, 0, 0, 1, 0);
                printf("\nТестирование на массиве...\n");
                simulate_smol(2, 0, 0, 1, 0);
                break;
            case 5:
                // Анализ выделенных участков памяти (только для списка)
                printf("\n=== РЕЖИМ 5: АНАЛИЗ ВЫДЕЛЕННЫХ УЧАСТКОВ ПАМЯТИ ===\n");
                simulate_smol(1, 0, 0, 0, 1);
                break;
            case 6:
                change_parameters();
                break;
            case 0:
                printf("Завершение программы.\n");
                break;
            default:
                printf("Неверный выбор!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
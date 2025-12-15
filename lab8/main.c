#include <stdio.h>
#include <stdlib.h>
#include "menu_handlers.h"

void print_menu() {
    printf("\n=== МЕНЮ УПРАВЛЕНИЯ ГРАФОМ ===\n");
    printf("1. Ввести новый граф (с клавиатуры)\n");
    printf("2. Добавить ребро в граф\n");
    printf("3. Удалить ребро из графа\n");
    printf("4. Очистить граф (удалить все ребра)\n");
    printf("5. Вывести граф на экран\n");
    printf("6. Визуализировать граф (Graphviz DOT)\n");
    printf("7. Решить задачу: вершины достижимые за расстояние <= A\n");
    printf("8. Выйти из программы\n");
    printf("Выберите действие (1-8): ");
}

int main() {
    Graph* graph = NULL;
    int choice;
    
    printf("Лабораторная работа №8. Графы.\n");
    printf("Задание: найти все вершины, достижимые из заданной за расстояние <= A.\n");
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода! Введите число от 1 до 8.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: handle_input_graph(&graph); break;
            case 2: handle_add_edge(graph); break;
            case 3: handle_remove_edge(graph); break;
            case 4: handle_clear_graph(graph); break;
            case 5: handle_print_graph(graph); break;
            case 6: handle_visualize_graph(graph); break;
            case 7: handle_solve_task(graph); break;
            case 8: 
                handle_exit(&graph);
                return 0;
            default:
                printf("Неверный выбор! Введите число от 1 до 8.\n");
                break;
        }
    }
    
    return 0;
}
#include "menu_handlers.h"
#include "graphviz.h"
#include "ford_bellman.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

void handle_input_graph(Graph** graph) {
    if (*graph) free_graph(*graph);
    error_t err = input_graph_from_console(graph);
    if (err == OK) {
        printf("Граф успешно создан.\n");
    } else {
        printf("Ошибка: %s\n", error_message(err));
        *graph = NULL;
    }
}

void handle_add_edge(Graph* graph) {
    if (!graph) {
        printf("Сначала создайте граф!\n");
        return;
    }
    
    int from, to;
    double weight;
    printf("Введите ребро в формате: from to weight\n");
    if (scanf("%d %d %lf", &from, &to, &weight) == 3) {
        error_t err = add_edge(graph, from, to, weight);
        if (err == OK) {
            printf("Ребро %d->%d добавлено", from, to);
            if (!graph->directed) {
                printf(", обратное ребро %d->%d также добавлено", to, from);
            }
            printf("\n");
        } else if (err == ERR_EDGE_EXISTS) {
            printf("Ребро %d->%d уже существовало, вес обновлен", from, to);
            if (!graph->directed) {
                printf(", вес обратного ребра также обновлен");
            }
            printf("\n");
        } else {
            printf("Ошибка: %s\n", error_message(err));
        }
    } else {
        printf("Неверный формат ввода!\n");
        while (getchar() != '\n');
    }
}

void handle_remove_edge(Graph* graph) {
    if (!graph) {
        printf("Сначала создайте граф!\n");
        return;
    }
    
    int from, to;
    printf("Введите ребро для удаления: from to\n");
    if (scanf("%d %d", &from, &to) == 2) {
        error_t err = remove_edge(graph, from, to);
        if (err == OK) {
            printf("Ребро %d->%d удалено", from, to);
            if (!graph->directed) {
                printf(", обратное ребро %d->%d также удалено", to, from);
            }
            printf("\n");
        } else {
            printf("Ошибка: %s\n", error_message(err));
        }
    } else {
        printf("Неверный формат ввода!\n");
        while (getchar() != '\n');
    }
}

void handle_clear_graph(Graph* graph) {
    if (!graph) {
        printf("Граф не создан!\n");
        return;
    }
    clear_graph(graph);
    printf("Все ребра удалены.\n");
}

void handle_print_graph(Graph* graph) {
    if (!graph) {
        printf("Граф не создан!\n");
        return;
    }
    print_graph(graph);
}

void handle_visualize_graph(Graph* graph) {
    if (!graph) {
        printf("Граф не создан!\n");
        return;
    }
    error_t err = export_to_dot(graph, "graph.dot");
    if (err == OK) {
        printf("Граф экспортирован в graph.dot\n");
        printf("Для визуализации: dot -Tpng graph.dot -o graph.png\n");
    } else {
        printf("Ошибка: %s\n", error_message(err));
    }
}

void handle_solve_task(Graph* graph) {
    if (!graph) {
        printf("Граф не создан!\n");
        return;
    }
    
    int start_vertex;
    double A;
    
    printf("Введите начальную вершину: ");
    if (scanf("%d", &start_vertex) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Введите максимальное расстояние A: ");
    if (scanf("%lf", &A) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n');
        return;
    }
    
    error_t err;
    double* distances = ford_bellman(graph, start_vertex, &err);
    
    if (err != OK) {
        printf("Ошибка: %s\n", error_message(err));
        if (distances) free(distances);
        return;
    }
    
    printf("\nРезультат (расстояния от вершины %d):\n", start_vertex);
    int found = 0;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (i == start_vertex) {
            continue;
        }
        
        if (isinf(distances[i]) && distances[i] < 0) {
            printf("Вершина %d: кратчайший путь не существует (отрицательный цикл)\n", i);
        }
        else if (isinf(distances[i])) {
            printf("Вершина %d: недостижима\n", i);
        }
        else if (distances[i] <= A) {
            printf("Вершина %d: %.2lf (удовлетворяет условию <= %.2lf)\n", i, distances[i], A);
            found++;
        }
        else {
            printf("Вершина %d: %.2lf (не удовлетворяет условию)\n", i, distances[i]);
        }
    }
    
    if (found == 0) {
        printf("\nНет вершин, удовлетворяющих условию (расстояние <= %.2lf).\n", A);
    } else {
        printf("\nВсего вершин, удовлетворяющих условию: %d\n", found);
    }
    
    free(distances);
}

void handle_exit(Graph** graph) {
    if (*graph) free_graph(*graph);
    printf("Программа завершена.\n");
}

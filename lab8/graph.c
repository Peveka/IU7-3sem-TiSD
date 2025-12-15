#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define MAX_STR_LEN 256

#include "graph.h"
#include "err_alloc.h"
#include <stdio.h>
#include <string.h>

#define INITIAL_CAPACITY 10

error_t create_graph(Graph** graph, int vertex_count, int directed) {
    if (vertex_count <= 0) 
        return ERR_INVALID_DATA;
    
    error_t err = OK;
    *graph = safe_malloc(sizeof(Graph), &err);
    if (err != OK) return err;
    
    (*graph)->edges = safe_malloc(sizeof(Edge) * INITIAL_CAPACITY, &err);
    if (err != OK) {
        free(*graph);
        *graph = NULL;
        return err;
    }
    
    (*graph)->vertex_count = vertex_count;
    (*graph)->edge_count = 0;
    (*graph)->capacity = INITIAL_CAPACITY;
    (*graph)->directed = directed;
    
    return OK;
}

error_t add_edge(Graph* graph, int from, int to, double weight) {
    if (!graph) return ERR_GRAPH_EMPTY;
    if (!vertex_exists(graph, from) || !vertex_exists(graph, to)) 
        return ERR_VERTEX_NOT_FOUND;
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].from == from && graph->edges[i].to == to) {
            graph->edges[i].weight = weight;
            if (!graph->directed) {
                for (int j = 0; j < graph->edge_count; j++) {
                    if (graph->edges[j].from == to && graph->edges[j].to == from) {
                        graph->edges[j].weight = weight;
                        break;
                    }
                }
            }
            return ERR_EDGE_EXISTS;
        }
    }
    
    if (graph->edge_count >= graph->capacity) {
        error_t err = OK;
        int new_capacity = graph->capacity * 2;
        Edge* new_edges = safe_realloc(graph->edges, sizeof(Edge) * new_capacity, &err);
        if (err != OK) return err;
        graph->edges = new_edges;
        graph->capacity = new_capacity;
    }
    
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    
    if (!graph->directed) {
        int reverse_exists = 0;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == to && graph->edges[i].to == from) {
                reverse_exists = 1;
                break;
            }
        }
        
        if (!reverse_exists) {
            if (graph->edge_count >= graph->capacity) {
                error_t err = OK;
                int new_capacity = graph->capacity * 2;
                Edge* new_edges = safe_realloc(graph->edges, sizeof(Edge) * new_capacity, &err);
                if (err != OK) {
                    graph->edge_count--;
                    return err;
                }
                graph->edges = new_edges;
                graph->capacity = new_capacity;
            }
            
            graph->edges[graph->edge_count].from = to;
            graph->edges[graph->edge_count].to = from;
            graph->edges[graph->edge_count].weight = weight;
            graph->edge_count++;
        }
    }
    
    return OK;
}

void free_graph(Graph* graph) {
    if (!graph) 
        return;
    if (graph->edges) 
        free(graph->edges);
    free(graph);
}

error_t remove_edge(Graph* graph, int from, int to) {
    if (!graph) return ERR_GRAPH_EMPTY;
    if (!vertex_exists(graph, from) || !vertex_exists(graph, to)) 
        return ERR_VERTEX_NOT_FOUND;
    
    int found = 0;
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].from == from && graph->edges[i].to == to) {
            for (int j = i; j < graph->edge_count - 1; j++) {
                graph->edges[j] = graph->edges[j + 1];
            }
            graph->edge_count--;
            found = 1;
            i--;
            
            if (!graph->directed) {
                for (int k = 0; k < graph->edge_count; k++) {
                    if (graph->edges[k].from == to && graph->edges[k].to == from) {
                        for (int j = k; j < graph->edge_count - 1; j++) {
                            graph->edges[j] = graph->edges[j + 1];
                        }
                        graph->edge_count--;
                        break;
                    }
                }
            }
        }
    }
    
    if (!found) return ERR_EDGE_NOT_FOUND;
    return OK;
}

void clear_graph(Graph* graph) {
    if (!graph) 
        return;
    graph->edge_count = 0;
}

int vertex_exists(const Graph* graph, int vertex) {
    return graph && vertex >= 0 && vertex < graph->vertex_count;
}

int get_max_vertex_id(const Graph* graph) {
    if (!graph || graph->vertex_count == 0) return -1;
    return graph->vertex_count - 1;
}

void print_graph(const Graph* graph) {
    if (!graph) {
        printf("Граф не существует!\n");
        return;
    }
    
    printf("\n=== Граф ===\n");
    printf("Вершин: %d, Ребер: %d, %s\n", 
           graph->vertex_count, graph->edge_count,
           graph->directed ? "Ориентированный" : "Неориентированный");
    
    printf("Ребра:\n");
    for (int i = 0; i < graph->edge_count; i++) {
        printf("  %d -> %d : %.2lf\n", 
               graph->edges[i].from, 
               graph->edges[i].to, 
               graph->edges[i].weight);
    }
}

error_t input_graph_from_console(Graph** graph) {
    int vertex_count, edge_count, directed;
    
    printf("Введите количество вершин: ");
    if (scanf("%d", &vertex_count) != 1 || vertex_count <= 0) {
        while (getchar() != '\n');
        return ERR_INVALID_DATA;
    }
    
    printf("Граф ориентированный? (1 - да, 0 - нет): ");
    if (scanf("%d", &directed) != 1) {
        while (getchar() != '\n');
        return ERR_INVALID_DATA;
    }
    
    error_t err = create_graph(graph, vertex_count, directed);
    if (err != OK) return err;
    
    printf("Введите количество ребер: ");
    if (scanf("%d", &edge_count) != 1 || edge_count < 0) {
        free_graph(*graph);
        *graph = NULL;
        while (getchar() != '\n');
        return ERR_INVALID_DATA;
    }
    
    printf("Введите ребра в формате: from to weight\n");
    printf("(вершины нумеруются от 0 до %d)\n", vertex_count - 1);
    
    for (int i = 0; i < edge_count; i++) {
        int from, to;
        double weight;
        
        printf("Ребро %d: ", i + 1);
        if (scanf("%d %d %lf", &from, &to, &weight) != 3) {
            printf("Ошибка ввода! Пропускаем это ребро.\n");
            while (getchar() != '\n');
            continue;
        }
        
        err = add_edge(*graph, from, to, weight);
        if (err == OK) {
            printf("Ребро %d->%d добавлено", from, to);
            if (!directed) {
                printf(", обратное ребро %d->%d также добавлено", to, from);
            }
            printf("\n");
        } else if (err == ERR_EDGE_EXISTS) {
            printf("Ребро %d->%d уже существовало, вес обновлен", from, to);
            if (!directed) {
                printf(", вес обратного ребра также обновлен");
            }
            printf("\n");
        } else {
            printf("Ошибка: %s\n", error_message(err));
        }
    }
    
    return OK;
}
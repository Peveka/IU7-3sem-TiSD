#include "ford_bellman.h"
#include "err_alloc.h"
#include <float.h>
#include <math.h>

double* ford_bellman(const Graph* graph, int start_vertex, error_t* error) {
    if (error) *error = OK;
    
    if (!graph) {
        if (error) 
            *error = ERR_GRAPH_EMPTY;
        return NULL;
    }
    
    if (!vertex_exists(graph, start_vertex)) {
        if (error) 
            *error = ERR_VERTEX_NOT_FOUND;
        return NULL;
    }
    
    error_t err = OK;
    double* dist = safe_malloc(graph->vertex_count * sizeof(double), &err);
    if (err != OK) 
    {
        if (error) *error = err;
        return NULL;
    }
    
    // Инициализация
    for (int i = 0; i < graph->vertex_count; i++) 
    {
        dist[i] = INFINITY;
    }
    dist[start_vertex] = 0;
    
    // Основные V-1 итераций
    for (int i = 0; i < graph->vertex_count - 1; i++) {
        int changed = 0;
        
        for (int j = 0; j < graph->edge_count; j++) {
            int u = graph->edges[j].from;
            int v = graph->edges[j].to;
            double weight = graph->edges[j].weight;
            
            if (dist[u] != INFINITY && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                changed = 1;
            }
        }
        
        if (!changed) break;
    }
    
    // Поиск вершин, достижимых через отрицательные циклы
    for (int i = 0; i < graph->vertex_count; i++) {
        for (int j = 0; j < graph->edge_count; j++) {
            int u = graph->edges[j].from;
            int v = graph->edges[j].to;
            double weight = graph->edges[j].weight;
            
            if (dist[u] != INFINITY && dist[u] + weight < dist[v]) {
                // Вершина достижима через отрицательный цикл
                dist[v] = NEG_INF;
            }
        }
    }
    
    // Распространение NEG_INF на все достижимые вершины
    for (int i = 0; i < graph->vertex_count; i++) {
        for (int j = 0; j < graph->edge_count; j++) {
            int u = graph->edges[j].from;
            int v = graph->edges[j].to;
            
            if (isinf(dist[u]) && dist[u] < 0 && !isinf(dist[v])) {
                dist[v] = NEG_INF;
            }
        }
    }
    
    return dist;
}
#ifndef FORD_BELLMAN_H
#define FORD_BELLMAN_H

#include "graph.h"
#define NEG_INF (-INFINITY)

double* ford_bellman(const Graph* graph, int start_vertex, error_t* error);
int* find_reachable_vertices(const Graph* graph, int start_vertex, 
                             double max_distance, int* count, error_t* error);

#endif
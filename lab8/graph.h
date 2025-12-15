#ifndef GRAPH_H
#define GRAPH_H

#include "errors.h"

typedef struct Edge {
    int from;
    int to;
    double weight;
} Edge;

typedef struct Graph {
    Edge* edges;
    int edge_count;
    int capacity;
    int vertex_count;
    int directed;
} Graph;

error_t create_graph(Graph** graph, int vertex_count, int directed);
void free_graph(Graph* graph);

error_t add_edge(Graph* graph, int from, int to, double weight);
error_t remove_edge(Graph* graph, int from, int to);
void clear_graph(Graph* graph);

error_t input_graph_from_console(Graph** graph);
void print_graph(const Graph* graph);

int get_max_vertex_id(const Graph* graph);
int vertex_exists(const Graph* graph, int vertex);

#endif
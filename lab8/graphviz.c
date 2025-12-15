#include "graphviz.h"
#include <stdio.h>

error_t export_to_dot(const Graph* graph, const char* filename) {
    if (!graph) return ERR_GRAPH_EMPTY;
    
    FILE* file = fopen(filename, "w");
    if (!file) return ERR_FILE_OPEN;
    
    if (graph->directed) {
        fprintf(file, "digraph G {\n");
    } else {
        fprintf(file, "graph G {\n");
    }
    
    fprintf(file, "  node [shape=circle];\n");
    
    for (int i = 0; i < graph->edge_count; i++) {
        if (graph->directed) {
            fprintf(file, "  %d -> %d [label=\"%.2lf\"];\n",
                   graph->edges[i].from,
                   graph->edges[i].to,
                   graph->edges[i].weight);
        } else {
            fprintf(file, "  %d -- %d [label=\"%.2lf\"];\n",
                   graph->edges[i].from,
                   graph->edges[i].to,
                   graph->edges[i].weight);
        }
    }
    
    fprintf(file, "}\n");
    fclose(file);
    
    return OK;
}
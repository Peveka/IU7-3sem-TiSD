#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "graph.h"

error_t export_to_dot(const Graph* graph, const char* filename);

#endif
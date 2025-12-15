#ifndef MENU_HANDLERS_H
#define MENU_HANDLERS_H

#include "graph.h"

void handle_input_graph(Graph** graph);
void handle_add_edge(Graph* graph);
void handle_remove_edge(Graph* graph);
void handle_clear_graph(Graph* graph);
void handle_print_graph(Graph* graph);
void handle_visualize_graph(Graph* graph);
void handle_solve_task(Graph* graph);
void handle_exit(Graph** graph);

#endif
#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct tree_node
{
    char data;
    int count;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

// Основные операции
tree_node *create_node(char data);
tree_node *insert_node(tree_node *root, char data);
tree_node *delete_node(tree_node *root, char data);
tree_node *find_node(tree_node *root, char data);
tree_node *build_tree_from_string(const char *input);
tree_node *remove_duplicates_from_tree(tree_node *root);
void free_tree(tree_node *root);

// Обходы дерева
void postorder_traversal(tree_node *root);

// Информация о дереве
int get_tree_height(tree_node *root);
int get_node_count(tree_node *root);
int get_leaf_count(tree_node *root);
int get_level_node_count(tree_node *root, int level);
int get_tree_balance_factor(tree_node *root);
bool is_bst(tree_node *root);

// Утилиты
void print_with_duplicates(tree_node *root);
tree_node *find_minimum(tree_node *root);
tree_node *find_maximum(tree_node *root);

// Функции визуализации
void print_tree_visual(tree_node *root);
void print_tree_statistics(tree_node *root);
void visualize_tree_graphviz(tree_node *root, const char *filename);

// Функции производительности
void compare_efficiency(const char *input_string);
void benchmark_sort_and_search(const char *input_string);
void benchmark_different_configs(void);
void benchmark_tree_operations_direct(tree_node *tree, int expected_size, const char *type);
#endif // BST_H
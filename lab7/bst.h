#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct tree_node {
    char data;
    int count;
    struct tree_node* left;
    struct tree_node* right;
} tree_node;

tree_node* create_node(char data);
tree_node* insert_node(tree_node* root, char data);
tree_node* delete_node(tree_node* root, char data);
tree_node* find_node(tree_node* root, char data);
tree_node* build_tree_from_string(const char* input);
void free_tree(tree_node* root);

void inorder_traversal(tree_node* root);
int get_tree_height(tree_node* root);
int get_node_count(tree_node* root);
int get_total_count(tree_node* root);
int get_leaf_count(tree_node* root);
int get_tree_balance_factor(tree_node* root);
void visualize_tree_graphviz(tree_node* root, const char* filename);
tree_node* find_node_with_comp(tree_node* root, char data, int* comp);

#endif 
#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct avl_node {
    char data;
    int count;
    int height;
    struct avl_node* left;
    struct avl_node* right;
} avl_node;

avl_node* avl_create_node(char data);
avl_node* avl_insert(avl_node* root, char data);
avl_node* avl_delete(avl_node* root, char data);
avl_node* avl_find(avl_node* root, char data);
avl_node* avl_build_from_string(const char* input);
void avl_free_tree(avl_node* root);

void avl_inorder_traversal(avl_node* root);
int avl_get_height(avl_node* root);
int avl_get_node_count(avl_node* root);
int avl_get_total_count(avl_node* root);
int avl_get_leaf_count(avl_node* root);
int avl_get_balance_factor(avl_node* root);
void avl_visualize_graphviz(avl_node* root, const char* filename);
avl_node* avl_find_with_comp(avl_node* root, char data, int* comp);

#endif
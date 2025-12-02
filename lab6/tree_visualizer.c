#include "bst.h"
#include <stdio.h>
#include <math.h>

static void print_tree_recursive(tree_node *root, int depth, char *prefix, int is_left)
{
    if (root == NULL)
    {
        return;
    }

    printf("%s", prefix);
    printf("%s", is_left ? "├──" : "└──");

    if (root->count > 1)
    {
        printf("\033[1;31m%c(%d)\033[0m\n", root->data, root->count);
    }
    else
    {
        printf("%c\n", root->data);
    }
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");
    print_tree_recursive(root->right, depth + 1, new_prefix, 1);
    print_tree_recursive(root->left, depth + 1, new_prefix, 0);
}

void print_tree_visual(tree_node *root)
{
    if (root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    printf("\nBinary Search Tree (root = %c):\n", root->data);
    //printf("Duplicates shown in red with count in parentheses\n");
    printf("=================================================\n");

    print_tree_recursive(root, 0, "", 0);
    printf("\n");
}

void print_tree_statistics(tree_node *root)
{
    if (root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    printf("\n=== TREE ANALYSIS ===\n");

    int height = get_tree_height(root);
    int total_nodes = get_node_count(root);
    int leaf_nodes = get_leaf_count(root);

    printf("Total nodes: %d\n", total_nodes);
    printf("Height: %d\n", height);
    printf("Leaf nodes: %d\n", leaf_nodes);

    if (total_nodes > 0)
    {
        double optimal_height = log2((double)total_nodes);
        printf("Optimal height (log₂(%d)): %.1f\n", total_nodes, optimal_height);

        if (height <= optimal_height + 1)
        {
            printf("Balance: Good (tree is reasonably balanced)\n");
            printf("Expected search time: O(log n) ≈ %.0f comparisons\n", optimal_height);
        }
        else if (height <= optimal_height + 3)
        {
            printf("Balance: Fair (slightly unbalanced)\n");
            printf("Expected search time: ~O(log n) to O(n)\n");
        }
        else
        {
            printf("Balance: Poor (tree is unbalanced)\n");
            printf("Expected search time: O(n) ≈ %d comparisons (worst case)\n", height);
        }
    }

    printf("=====================\n");
}

static void print_tree_horizontal(tree_node *root, int space)
{
    if (root == NULL)
    {
        return;
    }

    space += 5;

    print_tree_horizontal(root->right, space);

    printf("\n");
    for (int i = 5; i < space; i++)
    {
        printf(" ");
    }

    if (root->count > 1)
    {
        printf("\033[1;31m%c(%d)\033[0m", root->data, root->count);
    }
    else
    {
        printf("%c", root->data);
    }

    print_tree_horizontal(root->left, space);
}

void visualize_tree_graphviz(tree_node *root, const char *filename)
{
    if (root == NULL)
    {
        printf("Tree is empty, cannot create graph\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: Cannot open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "digraph BST {\n");
    fprintf(file, "  node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=\"#e0e0e0\"];\n");
    fprintf(file, "  edge [arrowhead=vee];\n");

    int null_count = 0;

    void export_tree(tree_node * node, FILE * f, int *null_cnt)
    {
        if (node == NULL)
            return;

        fprintf(f, "  \"%p\" [label=\"%c\\ncount: %d\"];\n",
                (void *)node, node->data, node->count);

        if (node->left != NULL)
        {
            fprintf(f, "  \"%p\" -> \"%p\";\n",
                    (void *)node, (void *)node->left);
            export_tree(node->left, f, null_cnt);
        }
        else
        {
            fprintf(f, "  null%d [shape=point];\n", *null_cnt);
            fprintf(f, "  \"%p\" -> null%d;\n",
                    (void *)node, (*null_cnt)++);
        }

        if (node->right != NULL)
        {
            fprintf(f, "  \"%p\" -> \"%p\";\n",
                    (void *)node, (void *)node->right);
            export_tree(node->right, f, null_cnt);
        }
        else
        {
            fprintf(f, "  null%d [shape=point];\n", *null_cnt);
            fprintf(f, "  \"%p\" -> null%d;\n",
                    (void *)node, (*null_cnt)++);
        }
    }

    export_tree(root, file, &null_count);

    fprintf(file, "}\n");
    fclose(file);

    printf("Graphviz file created: %s\n", filename);
    printf("To generate image, run: dot -Tpng %s -o tree.png\n", filename);
}
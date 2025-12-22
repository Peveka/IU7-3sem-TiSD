#include "bst.h"
#include "avl.h"
#include <stdio.h>

void visualize_tree_graphviz(tree_node* root, const char* filename) {
    if (root == NULL) {
        printf("Tree is empty, cannot create graph\n");
        return;
    }
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Cannot open file %s for writing\n", filename);
        return;
    }
    
    fprintf(file, "digraph BST {\n");
    fprintf(file, "  node [fontname=\"Arial\", shape=circle];\n");
    fprintf(file, "  edge [arrowhead=vee];\n");
    
    int null_count = 0;
    
    void export_tree(tree_node* node, FILE* f, int* null_cnt) {
        if (node == NULL) return;
        
        fprintf(f, "  \"%p\" [label=\"%c\\ncount: %d\"];\n",
                (void*)node, node->data, node->count);
        
        if (node->left != NULL) {
            fprintf(f, "  \"%p\" -> \"%p\" [label=\"L\"];\n",
                    (void*)node, (void*)node->left);
            export_tree(node->left, f, null_cnt);
        } else {
            fprintf(f, "  null%d [shape=point];\n", *null_cnt);
            fprintf(f, "  \"%p\" -> null%d [label=\"L\"];\n",
                    (void*)node, (*null_cnt)++);
        }
        
        if (node->right != NULL) {
            fprintf(f, "  \"%p\" -> \"%p\" [label=\"R\"];\n",
                    (void*)node, (void*)node->right);
            export_tree(node->right, f, null_cnt);
        } else {
            fprintf(f, "  null%d [shape=point];\n", *null_cnt);
            fprintf(f, "  \"%p\" -> null%d [label=\"R\"];\n",
                    (void*)node, (*null_cnt)++);
        }
    }
    
    export_tree(root, file, &null_count);
    
    fprintf(file, "}\n");
    fclose(file);
    
    printf("Graphviz file created: %s\n", filename);
    printf("To generate image, run: dot -Tpng %s -o tree.png\n", filename);
}

void avl_visualize_graphviz(avl_node* root, const char* filename) {
    if (!root) {
        printf("AVL Tree is empty, cannot create graph\n");
        return;
    }
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Cannot open file %s for writing\n", filename);
        return;
    }
    
    fprintf(file, "digraph AVL {\n");
    fprintf(file, "  node [fontname=\"Arial\", shape=circle];\n");
    fprintf(file, "  edge [arrowhead=vee];\n");
    
    int null_count = 0;
    
    void export_tree(avl_node* node, FILE* f, int* null_cnt) {
        if (!node) return;
        
        fprintf(f, "  \"%p\" [label=\"%c\\ncount: %d\\nh: %d\"];\n",
                (void*)node, node->data, node->count, node->height);
        
        if (node->left) {
            fprintf(f, "  \"%p\" -> \"%p\" [label=\"L\"];\n",
                    (void*)node, (void*)node->left);
            export_tree(node->left, f, null_cnt);
        } else {
            fprintf(f, "  null%d [shape=point];\n", *null_cnt);
            fprintf(f, "  \"%p\" -> null%d [label=\"L\"];\n",
                    (void*)node, (*null_cnt)++);
        }
        
        if (node->right) {
            fprintf(f, "  \"%p\" -> \"%p\" [label=\"R\"];\n",
                    (void*)node, (void*)node->right);
            export_tree(node->right, f, null_cnt);
        } else {
            fprintf(f, "  null%d [shape=point];\n", *null_cnt);
            fprintf(f, "  \"%p\" -> null%d [label=\"R\"];\n",
                    (void*)node, (*null_cnt)++);
        }
    }
    
    export_tree(root, file, &null_count);
    fprintf(file, "}\n");
    fclose(file);
    
    printf("AVL Graphviz file created: %s\n", filename);
    printf("To generate image, run: dot -Tpng %s -o avl_tree.png\n", filename);
}
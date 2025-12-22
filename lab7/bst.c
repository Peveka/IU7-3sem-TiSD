#include "bst.h"

tree_node* create_node(char data) {
    tree_node* new_node = (tree_node*)malloc(sizeof(tree_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    new_node->data = data;
    new_node->count = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

tree_node* insert_node(tree_node* root, char data) {
    if (root == NULL) {
        return create_node(data);
    }
    
    if (data < root->data) {
        root->left = insert_node(root->left, data);
    } else if (data > root->data) {
        root->right = insert_node(root->right, data);
    } else {
        root->count++;
    }
    
    return root;
}

tree_node* find_minimum(tree_node* root) {
    if (root == NULL) return NULL;
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

tree_node* delete_node(tree_node* root, char data) {
    if (root == NULL) return NULL;
    
    if (data < root->data) {
        root->left = delete_node(root->left, data);
    } else if (data > root->data) {
        root->right = delete_node(root->right, data);
    } else {
        if (root->left == NULL) {
            tree_node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            tree_node* temp = root->left;
            free(root);
            return temp;
        }
        
        tree_node* parent = NULL;
        tree_node* successor = root->right;
        
        while (successor->left != NULL) {
            parent = successor;
            successor = successor->left;
        }
        
        if (parent != NULL) {
            parent->left = successor->right;
            successor->right = root->right;
        }
        
        successor->left = root->left;
        free(root);
        
        return successor;
    }
    
    return root;
}

tree_node* find_node(tree_node* root, char data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return find_node(root->left, data);
    } else {
        return find_node(root->right, data);
    }
}

tree_node* build_tree_from_string(const char* input) {
    tree_node* root = NULL;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if ((unsigned char)input[i] != ' ') {
            root = insert_node(root, input[i]);
        }
    }
    
    return root;
}

void free_tree(tree_node* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void inorder_traversal(tree_node* root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        printf("%c(%d) ", root->data, root->count);
        inorder_traversal(root->right);
    }
}

int get_tree_height(tree_node* root) {
    if (root == NULL) return 0;
    
    int left_height = get_tree_height(root->left);
    int right_height = get_tree_height(root->right);
    
    return (left_height > right_height ? left_height : right_height) + 1;
}

int get_node_count(tree_node* root) {
    if (root == NULL) return 0;
    return 1 + get_node_count(root->left) + get_node_count(root->right);
}

int get_total_count(tree_node* root) {
    if (root == NULL) return 0;
    return root->count + get_total_count(root->left) + get_total_count(root->right);
}

int get_leaf_count(tree_node* root) {
    if (root == NULL) return 0;
    
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    
    return get_leaf_count(root->left) + get_leaf_count(root->right);
}

int get_tree_balance_factor(tree_node* root) {
    if (root == NULL) return 0;
    return get_tree_height(root->left) - get_tree_height(root->right);
}

tree_node* find_node_with_comp(tree_node* root, char data, int* comp) {
    (*comp)++;
    if (root == NULL || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return find_node_with_comp(root->left, data, comp);
    } else {
        return find_node_with_comp(root->right, data, comp);
    }
}
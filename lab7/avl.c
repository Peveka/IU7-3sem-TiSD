#include "avl.h"
#include <string.h>

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int get_node_height(avl_node* node) {
    return node ? node->height : 0;
}

static void update_height(avl_node* node) {
    if (node) {
        node->height = 1 + max(get_node_height(node->left), get_node_height(node->right));
    }
}

static int get_balance(avl_node* node) {
    return node ? get_node_height(node->left) - get_node_height(node->right) : 0;
}

static avl_node* right_rotate(avl_node* y) {
    avl_node* x = y->left;
    avl_node* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    update_height(y);
    update_height(x);
    
    return x;
}

static avl_node* left_rotate(avl_node* x) {
    avl_node* y = x->right;
    avl_node* T2 = y->left;
    y->left = x;
    x->right = T2;
    
    update_height(x);
    update_height(y);
    
    return y;
}

static avl_node* left_right_rotate(avl_node* z) {
    z->left = left_rotate(z->left);
    return right_rotate(z);
}

static avl_node* right_left_rotate(avl_node* z) {
    z->right = right_rotate(z->right);
    return left_rotate(z);
}

static avl_node* balance_node(avl_node* node) {
    if (!node) return node;
    
    update_height(node);
    int balance = get_balance(node);
    
    if (balance > 1) {
        int left_balance = get_balance(node->left);
        
        if (left_balance >= 0) {
            return right_rotate(node);
        } else {
            return left_right_rotate(node);
        }
    }   
    
    if (balance < -1) {
        int right_balance = get_balance(node->right);
        
        if (right_balance <= 0) {
            return left_rotate(node);
        } else {
            return right_left_rotate(node);
        }
    }
    
    return node;
}

avl_node* avl_create_node(char data) {
    avl_node* node = (avl_node*)malloc(sizeof(avl_node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for AVL node\n");
        return NULL;
    }
    
    node->data = data;
    node->count = 1;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

avl_node* avl_insert(avl_node* root, char data) {
    if (!root) {
        return avl_create_node(data);
    }
    
    if (data < root->data) {
        root->left = avl_insert(root->left, data);
    } else if (data > root->data) {
        root->right = avl_insert(root->right, data);
    } else {
        root->count++;
        return root;
    }
    
    return balance_node(root);
}

static avl_node* find_min(avl_node* node) {
    avl_node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

static avl_node* delete_node_helper(avl_node* node) {
    if (!node) return NULL;
    
    if (!node->left || !node->right) {
        avl_node* temp = node->left ? node->left : node->right;
        
        if (!temp) {
            free(node);
            return NULL;
        } else {
            avl_node* result = temp;
            free(node);
            return result;
        }
    }
    
    avl_node* parent = NULL;
    avl_node* successor = node->right;
    
    while (successor->left) {
        parent = successor;
        successor = successor->left;
    }
    
    if (parent) {
        parent->left = successor->right;
        successor->right = node->right;
    }
    
    successor->left = node->left;
    
    free(node);
    
    return successor;
}

avl_node* avl_delete(avl_node* root, char data) {
    if (!root) return NULL;
    
    if (data < root->data) {
        root->left = avl_delete(root->left, data);
    } else if (data > root->data) {
        root->right = avl_delete(root->right, data);
    } else {
        root = delete_node_helper(root);
    }
    
    if (!root) return NULL;
    
    return balance_node(root);
}

avl_node* avl_find(avl_node* root, char data) {
    if (!root || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return avl_find(root->left, data);
    } else {
        return avl_find(root->right, data);
    }
}

avl_node* avl_build_from_string(const char* input) {
    avl_node* root = NULL;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if ((unsigned char)input[i] != ' ') {
            root = avl_insert(root, input[i]);
        }
    }
    
    return root;
}

void avl_free_tree(avl_node* root) {
    if (root) {
        avl_free_tree(root->left);
        avl_free_tree(root->right);
        free(root);
    }
}

void avl_inorder_traversal(avl_node* root) {
    if (root) {
        avl_inorder_traversal(root->left);
        printf("%c(%d) ", root->data, root->count);
        avl_inorder_traversal(root->right);
    }
}

int avl_get_height(avl_node* root) {
    return get_node_height(root);
}

int avl_get_node_count(avl_node* root) {
    if (!root) return 0;
    return 1 + avl_get_node_count(root->left) + avl_get_node_count(root->right);
}

int avl_get_total_count(avl_node* root) {
    if (!root) return 0;
    return root->count + avl_get_total_count(root->left) + avl_get_total_count(root->right);
}

int avl_get_leaf_count(avl_node* root) {
    if (!root) return 0;
    
    if (!root->left && !root->right) {
        return 1;
    }
    
    return avl_get_leaf_count(root->left) + avl_get_leaf_count(root->right);
}

int avl_get_balance_factor(avl_node* root) {
    return get_balance(root);
}

avl_node* avl_find_with_comp(avl_node* root, char data, int* comp) {
    (*comp)++;
    if (!root || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return avl_find_with_comp(root->left, data, comp);
    } else {
        return avl_find_with_comp(root->right, data, comp);
    }
}
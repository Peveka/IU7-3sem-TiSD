#include "bst.h"

tree_node* create_node(char data)
{
    tree_node* new_node = (tree_node*)malloc(sizeof(tree_node));
    if (new_node == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    new_node->data = data;
    new_node->count = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

tree_node* insert_node(tree_node* root, char data)
{
    if (root == NULL) 
    {
        return create_node(data);
    }
    
    if (data < root->data) 
    {
        root->left = insert_node(root->left, data);
    } 
    else if (data > root->data) 
    {
        root->right = insert_node(root->right, data);
    } 
    else 
    {
        root->count++;
    }
    
    return root;
}

tree_node* find_minimum(tree_node* root)
{
    if (root == NULL) 
    {
        return NULL;
    }
    
    while (root->left != NULL) 
    {
        root = root->left;
    }
    
    return root;
}

tree_node* find_maximum(tree_node* root)
{
    if (root == NULL) 
    {
        return NULL;
    }
    
    while (root->right != NULL) 
    {
        root = root->right;
    }
    
    return root;
}

void inorder_traversal(tree_node *root)
{
    if (root != NULL)
    {
        inorder_traversal(root->left);
        printf("%c ", root->data);
        inorder_traversal(root->right);
    }
}

tree_node *delete_node(tree_node *root, char data)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (data < root->data)
    {
        root->left = delete_node(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = delete_node(root->right, data);
    }
    else
    {
        if (root->left == NULL)
        {
            tree_node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            tree_node *temp = root->left;
            free(root);
            return temp;
        }

        tree_node *current = root;
        tree_node *temp = root->right;
        //берем самый левый из правого (самый меньший из правого)
        while (temp->left != NULL)
        {
            current = temp;
            temp = temp->left;
        }

        if (current != root) // перестроение если не прямой прдк
        {
            current->left = temp->right;
            temp->right = root->right;
        }

        temp->left = root->left;

        free(root);

        return temp;
    }

    return root;
}

tree_node* find_node(tree_node* root, char data)
{
    if (root == NULL || root->data == data) 
    {
        return root;
    }
    
    if (data < root->data) 
    {
        return find_node(root->left, data);
    } 
    else 
    {
        return find_node(root->right, data);
    }
}

tree_node *build_tree_from_string(const char *input)
{
    tree_node *root = NULL;

    for (size_t i = 0; input[i] != '\0'; i++)
    {
        if (isalnum((unsigned char)input[i]))
        {
            root = insert_node(root, input[i]);
        }
    }

    return root;
}

tree_node *remove_duplicates_from_tree(tree_node *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    root->left = remove_duplicates_from_tree(root->left);
    root->right = remove_duplicates_from_tree(root->right);

    if (root->count > 1)
    {
        tree_node *result = delete_node(root, root->data);
        return result;
    }

    return root;
}

void free_tree(tree_node* root)
{
    if (root != NULL) 
    {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void postorder_traversal(tree_node* root)
{
    if (root != NULL) 
    {
        postorder_traversal(root->left);
        postorder_traversal(root->right);
        printf("%c ", root->data);
    }
}

int get_tree_height(tree_node* root)
{
    if (root == NULL) 
    {
        return 0;
    }
    
    int left_height = get_tree_height(root->left);
    int right_height = get_tree_height(root->right);
    
    return (left_height > right_height ? left_height : right_height) + 1;
}

int get_node_count(tree_node* root)
{
    if (root == NULL) 
    {
        return 0;
    }
    
    return get_node_count(root->left) + get_node_count(root->right) + 1;
}

int get_leaf_count(tree_node* root)
{
    if (root == NULL) 
    {
        return 0;
    }
    
    if (root->left == NULL && root->right == NULL) 
    {
        return 1;
    }
    
    return get_leaf_count(root->left) + get_leaf_count(root->right);
}

int get_level_node_count(tree_node* root, int level)
{
    if (root == NULL) 
    {
        return 0;
    }
    
    if (level == 0) 
    {
        return 1;
    }
    
    return get_level_node_count(root->left, level - 1) + 
           get_level_node_count(root->right, level - 1);
}

static bool is_bst_util(tree_node* root, tree_node* min, tree_node* max)
{
    if (root == NULL) 
    {
        return true;
    }
    
    if ((min != NULL && root->data <= min->data) || 
        (max != NULL && root->data >= max->data)) 
    {
        return false;
    }
    
    return is_bst_util(root->left, min, root) && 
           is_bst_util(root->right, root, max);
}

bool is_bst(tree_node* root)
{
    return is_bst_util(root, NULL, NULL);
}

int get_tree_balance_factor(tree_node* root)
{
    if (root == NULL) 
    {
        return 0;
    }
    
    return get_tree_height(root->left) - get_tree_height(root->right);
}

void print_with_duplicates(tree_node* root)
{
    if (root != NULL) 
    {
        print_with_duplicates(root->left);
        
        if (root->count > 1) 
        {
            printf("\033[1;31m%c(%d)\033[0m ", root->data, root->count);
        } 
        else 
        {
            printf("%c ", root->data);
        }
        
        print_with_duplicates(root->right);
    }
}
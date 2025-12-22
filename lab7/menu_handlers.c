#include "bst.h"
#include "avl.h"
#include "hash_table.h"
#include "hash_table_chain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compare_efficiency.h"

#define MAX_INPUT_SIZE 1024

tree_node* bst_root = NULL;
avl_node* avl_root = NULL;
hash_table_t* hash_table = NULL;
hash_table_chain_t* hash_table_chain = NULL;

//=======================================================================================
//=========== BST OPERATIONS ============================================================
//=======================================================================================
void bst_add_element_handler(void) {
    char element;
    printf("Enter character to add to BST: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    if (!isalnum((unsigned char)element)) {
        printf("Error: Only letters and digits are allowed\n");
        return;
    }
    
    bst_root = insert_node(bst_root, element);
    printf("Element '%c' added to BST\n", element);
}

void bst_remove_element_handler(void) {
    if (bst_root == NULL) {
        printf("BST is empty\n");
        return;
    }
    
    char element;
    printf("Enter character to remove from BST: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    tree_node* found = find_node(bst_root, element);
    if (found == NULL) {
        printf("Element '%c' not found in BST\n", element);
        return;
    }
    
    bst_root = delete_node(bst_root, element);
    printf("Element '%c' removed from BST\n", element);
}

void bst_search_element_handler(void) {
    char element;
    printf("Enter character to search in BST: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    tree_node* found = find_node(bst_root, element);
    if (found != NULL) {
        printf("Element '%c' found in BST (count: %d)\n", found->data, found->count);
    } else {
        printf("Element '%c' not found in BST\n", element);
    }
}

void bst_inorder_traversal_handler(void) {
    if (bst_root == NULL) {
        printf("BST is empty\n");
        return;
    }
    
    printf("BST In-order traversal: ");
    inorder_traversal(bst_root);
    printf("\n");
}

void bst_build_from_string_handler(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter string to build BST from: ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return;
    }
    
    if (bst_root != NULL) {
        free_tree(bst_root);
    }
    
    bst_root = build_tree_from_string(input);
    printf("BST built from string: %s\n", input);
    printf("BST created. Height: %d, Unique nodes: %d, Total elements: %d\n",
           get_tree_height(bst_root), get_node_count(bst_root), get_total_count(bst_root));
}

void bst_visualize_handler(void) {
    visualize_tree_graphviz(bst_root, "bst_tree.dot");
}

void bst_statistics_handler(void) {
    if (bst_root == NULL) {
        printf("BST is empty\n");
        return;
    }
    
    printf("\n=== BST STATISTICS ===\n");
    printf("Unique nodes: %d\n", get_node_count(bst_root));
    printf("Total elements (with duplicates): %d\n", get_total_count(bst_root));
    printf("Tree height: %d\n", get_tree_height(bst_root));
    printf("Leaf nodes: %d\n", get_leaf_count(bst_root));
    printf("Balance factor: %d\n", get_tree_balance_factor(bst_root));
    printf("=======================\n");
}

void bst_clear_handler(void) {
    free_tree(bst_root);
    bst_root = NULL;
    printf("BST cleared\n");
}

//=======================================================================================
//=========== AVL OPERATIONS ============================================================
//=======================================================================================

void avl_add_element_handler(void) {
    char element;
    printf("Enter character to add to AVL: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    if (!isalnum((unsigned char)element)) {
        printf("Error: Only letters and digits are allowed\n");
        return;
    }
    
    avl_root = avl_insert(avl_root, element);
    printf("Element '%c' added to AVL tree\n", element);
}

void avl_remove_element_handler(void) {
    if (avl_root == NULL) {
        printf("AVL Tree is empty\n");
        return;
    }
    
    char element;
    printf("Enter character to remove from AVL: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    avl_node* found = avl_find(avl_root, element);
    if (found == NULL) {
        printf("Element '%c' not found in AVL tree\n", element);
        return;
    }
    
    avl_root = avl_delete(avl_root, element);
    printf("Element '%c' removed from AVL tree\n", element);
}

void avl_search_element_handler(void) {
    char element;
    printf("Enter character to search in AVL: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    avl_node* found = avl_find(avl_root, element);
    if (found != NULL) {
        printf("Element '%c' found in AVL tree (count: %d, height: %d)\n",
               found->data, found->count, found->height);
    } else {
        printf("Element '%c' not found in AVL tree\n", element);
    }
}

void avl_inorder_traversal_handler(void) {
    if (avl_root == NULL) {
        printf("AVL Tree is empty\n");
        return;
    }
    
    printf("AVL In-order traversal: ");
    avl_inorder_traversal(avl_root);
    printf("\n");
}

void avl_build_from_string_handler(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter string to build AVL tree from: ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return;
    }
    
    if (avl_root != NULL) {
        avl_free_tree(avl_root);
    }
    
    avl_root = avl_build_from_string(input);
    printf("AVL tree built from string: %s\n", input);
    printf("AVL tree created. Height: %d, Unique nodes: %d, Total elements: %d\n",
           avl_get_height(avl_root), avl_get_node_count(avl_root), avl_get_total_count(avl_root));
}

void avl_visualize_handler(void) {
    avl_visualize_graphviz(avl_root, "avl_tree.dot");
}

void avl_statistics_handler(void) {
    if (avl_root == NULL) {
        printf("AVL Tree is empty\n");
        return;
    }
    
    printf("\n=== AVL TREE STATISTICS ===\n");
    printf("Unique nodes: %d\n", avl_get_node_count(avl_root));
    printf("Total elements (with duplicates): %d\n", avl_get_total_count(avl_root));
    printf("Tree height: %d\n", avl_get_height(avl_root));
    printf("Leaf nodes: %d\n", avl_get_leaf_count(avl_root));
    printf("Balance factor: %d\n", avl_get_balance_factor(avl_root));
    printf("============================\n");
}

void avl_clear_handler(void) {
    avl_free_tree(avl_root);
    avl_root = NULL;
    printf("AVL tree cleared\n");
}

//=======================================================================================
//=========== HASH TABLE OPEN ADDRESS ===================================================
//=======================================================================================

void hash_table_build_from_string_handler(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter string to build Hash Table from: ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return;
    }
    
    if (hash_table != NULL) {
        hash_table_destroy(hash_table);
    }
    
    hash_table = hash_table_create_from_string(input, HASH_DIVISION);
    
    if (hash_table) {
        printf("Hash Table built from string: %s\n", input);
        printf("Capacity: %d, Size: %d, Load factor: %.2f\n",
               hash_table->capacity, hash_table->size, hash_table->load_factor);
        printf("Collisions: %d\n", hash_table->collisions);
    } else {
        printf("Failed to create Hash Table\n");
    }
}

void hash_table_add_element_handler(void) {
    if (hash_table == NULL) {
        hash_table = hash_table_create(10, HASH_DIVISION);
    }
    
    char element;
    printf("Enter character to add to Hash Table: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    if (!isalnum((unsigned char)element)) {
        printf("Error: Only letters and digits are allowed\n");
        return;
    }
    
    if (hash_table_needs_resize(hash_table)) {
        hash_table_t* new_ht = hash_table_resize(hash_table);
        hash_table_destroy(hash_table);
        hash_table = new_ht;
    }
    
    if (hash_table_insert_with_count(hash_table, element)) {
        printf("Element '%c' added to Hash Table\n", element);
        
        if (hash_table_needs_rehash(hash_table)) {
            hash_function_type_t new_type = (hash_table->hash_func_type == HASH_DIVISION) 
                ? HASH_MULTIPLICATION 
                : HASH_DIVISION;
            hash_table_t* new_ht = hash_table_rehash(hash_table, new_type);
            hash_table_destroy(hash_table);
            hash_table = new_ht;
            printf("Auto-rehashed due to too many collisions\n");
        }
    } else {
        printf("Failed to add element '%c' to Hash Table\n", element);
    }
}

void hash_table_remove_element_handler(void) {
    if (hash_table == NULL || hash_table->size == 0) {
        printf("Hash Table is empty\n");
        return;
    }
    
    char element;
    printf("Enter character to remove from Hash Table: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    if (hash_table_remove(hash_table, element)) {
        printf("Element '%c' removed from Hash Table\n", element);
    } else {
        printf("Element '%c' not found in Hash Table\n", element);
    }
}

void hash_table_search_element_handler(void) {
    if (hash_table == NULL || hash_table->size == 0) {
        printf("Hash Table is empty\n");
        return;
    }
    
    char element;
    printf("Enter character to search in Hash Table: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    hash_table_entry_t* entry = hash_table_search(hash_table, element);
    if (entry != NULL) {
        printf("Element '%c' found in Hash Table (count: %d)\n", 
               entry->key, entry->value);
    } else {
        printf("Element '%c' not found in Hash Table\n", element);
    }
}

void hash_table_display_handler(void) {
    if (hash_table == NULL) {
        printf("Hash Table is empty\n");
        return;
    }
    
    hash_table_display(hash_table);
}

void hash_table_statistics_handler(void) {
    if (hash_table == NULL) {
        printf("Hash Table is empty\n");
        return;
    }
    
    printf("\n=== HASH TABLE STATISTICS ===\n");
    printf("Capacity: %d\n", hash_table->capacity);
    printf("Size: %d\n", hash_table->size);
    printf("Load factor: %.2f\n", hash_table->load_factor);
    printf("Collisions: %d\n", hash_table->collisions);
    printf("Hash function: %s\n", 
           hash_table->hash_func_type == HASH_DIVISION ? "Division" : "Multiplication");
    printf("Average comparisons for search: %d\n", 
           hash_table_get_average_comparisons(hash_table));
    printf("Needs resize: %s\n", 
           hash_table_needs_resize(hash_table) ? "YES" : "NO");
    printf("Needs rehash: %s\n", 
           hash_table_needs_rehash(hash_table) ? "YES" : "NO");
    printf("===============================\n");
}

void hash_table_rehash_handler(void) {
    if (hash_table == NULL) {
        printf("Hash Table is empty\n");
        return;
    }
    
    hash_function_type_t new_type;
    printf("Select new hash function:\n");
    printf("1. Division method\n");
    printf("2. Multiplication method (Golden ratio)\n");
    printf("Choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    if (choice == 1) {
        new_type = HASH_DIVISION;
    } else if (choice == 2) {
        new_type = HASH_MULTIPLICATION;
    } else {
        printf("Invalid choice\n");
        return;
    }
    
    hash_table_t* new_ht = hash_table_rehash(hash_table, new_type);
    if (new_ht) {
        hash_table_destroy(hash_table);
        hash_table = new_ht;
        printf("Hash Table rehashed successfully\n");
    }
}

void hash_table_clear_handler(void) {
    hash_table_destroy(hash_table);
    hash_table = NULL;
    printf("Hash Table cleared\n");
}

void hash_table_chain_build_from_string_handler(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter string to build Chaining Hash Table from: ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return;
    }
    
    if (hash_table_chain != NULL) {
        hash_table_chain_destroy(hash_table_chain);
    }
    
    hash_table_chain = hash_table_chain_create_from_string(input, HASH_CHAIN_DIVISION);
    
    if (hash_table_chain) {
        printf("Chaining Hash Table built from string: %s\n", input);
        printf("Capacity: %d, Size: %d, Load factor: %.2f\n",
               hash_table_chain->capacity, hash_table_chain->size, hash_table_chain->load_factor);
        printf("Collisions: %d\n", hash_table_chain->collisions);
    } else {
        printf("Failed to create Chaining Hash Table\n");
    }
}

void hash_table_chain_add_element_handler(void) {
    if (hash_table_chain == NULL) {
        hash_table_chain = hash_table_chain_create(10, HASH_CHAIN_DIVISION);
    }
    
    char element;
    printf("Enter character to add to Chaining Hash Table: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    if (!isalnum((unsigned char)element)) {
        printf("Error: Only letters and digits are allowed\n");
        return;
    }
    
    if (hash_table_chain_needs_resize(hash_table_chain)) {
        hash_table_chain_t* new_ht = hash_table_chain_resize(hash_table_chain);
        hash_table_chain_destroy(hash_table_chain);
        hash_table_chain = new_ht;
    }
    
    if (hash_table_chain_insert_with_count(hash_table_chain, element)) {
        printf("Element '%c' added to Chaining Hash Table\n", element);
        
        if (hash_table_chain_needs_rehash(hash_table_chain)) {
            hash_chain_function_type_t new_type = (hash_table_chain->hash_func_type == HASH_CHAIN_DIVISION) 
                ? HASH_CHAIN_MULTIPLICATION 
                : HASH_CHAIN_DIVISION;
            hash_table_chain_t* new_ht = hash_table_chain_rehash(hash_table_chain, new_type);
            hash_table_chain_destroy(hash_table_chain);
            hash_table_chain = new_ht;
            printf("Auto-rehashed due to too many collisions\n");
        }
    } else {
        printf("Failed to add element '%c' to Chaining Hash Table\n", element);
    }
}

void hash_table_chain_remove_element_handler(void) {
    if (hash_table_chain == NULL || hash_table_chain->size == 0) {
        printf("Chaining Hash Table is empty\n");
        return;
    }
    
    char element;
    printf("Enter character to remove from Chaining Hash Table: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    if (hash_table_chain_remove(hash_table_chain, element)) {
        printf("Element '%c' removed from Chaining Hash Table\n", element);
    } else {
        printf("Element '%c' not found in Chaining Hash Table\n", element);
    }
}

void hash_table_chain_search_element_handler(void) {
    if (hash_table_chain == NULL || hash_table_chain->size == 0) {
        printf("Chaining Hash Table is empty\n");
        return;
    }
    
    char element;
    printf("Enter character to search in Chaining Hash Table: ");
    if (scanf(" %c", &element) != 1) {
        printf("Error reading input\n");
        while (getchar() != '\n');
        return;
    }
    
    hash_chain_node_t* entry = hash_table_chain_search(hash_table_chain, element);
    if (entry != NULL) {
        printf("Element '%c' found in Chaining Hash Table (count: %d)\n", 
               entry->key, entry->value);
    } else {
        printf("Element '%c' not found in Chaining Hash Table\n", element);
    }
}

void hash_table_chain_display_handler(void) {
    if (hash_table_chain == NULL) {
        printf("Chaining Hash Table is empty\n");
        return;
    }
    
    hash_table_chain_display(hash_table_chain);
}

void hash_table_chain_statistics_handler(void) {
    if (hash_table_chain == NULL) {
        printf("Chaining Hash Table is empty\n");
        return;
    }
    
    printf("\n=== CHAINING HASH TABLE STATISTICS ===\n");
    printf("Capacity: %d\n", hash_table_chain->capacity);
    printf("Size: %d\n", hash_table_chain->size);
    printf("Load factor: %.2f\n", hash_table_chain->load_factor);
    printf("Collisions: %d\n", hash_table_chain->collisions);
    printf("Hash function: %s\n", 
           hash_table_chain->hash_func_type == HASH_CHAIN_DIVISION ? "Division" : "Multiplication");
    printf("Average comparisons for search: %d\n", 
           hash_table_chain_get_average_comparisons(hash_table_chain));
    printf("Needs resize: %s\n", 
           hash_table_chain_needs_resize(hash_table_chain) ? "YES" : "NO");
    printf("Needs rehash: %s\n", 
           hash_table_chain_needs_rehash(hash_table_chain) ? "YES" : "NO");
    printf("======================================\n");
}

void hash_table_chain_rehash_handler(void) {
    if (hash_table_chain == NULL) {
        printf("Chaining Hash Table is empty\n");
        return;
    }
    
    hash_chain_function_type_t new_type;
    printf("Select new hash function:\n");
    printf("1. Division method\n");
    printf("2. Multiplication method (Golden ratio)\n");
    printf("Choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    if (choice == 1) {
        new_type = HASH_CHAIN_DIVISION;
    } else if (choice == 2) {
        new_type = HASH_CHAIN_MULTIPLICATION;
    } else {
        printf("Invalid choice\n");
        return;
    }
    
    hash_table_chain_t* new_ht = hash_table_chain_rehash(hash_table_chain, new_type);
    if (new_ht) {
        hash_table_chain_destroy(hash_table_chain);
        hash_table_chain = new_ht;
        printf("Chaining Hash Table rehashed successfully\n");
    }
}

void hash_table_chain_clear_handler(void) {
    hash_table_chain_destroy(hash_table_chain);
    hash_table_chain = NULL;
    printf("Chaining Hash Table cleared\n");
}

void compare_efficiency_handler(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter string for comparison: ");
    while (getchar() != '\n');
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    input[strcspn(input, "\n")] = '\0';
    
    compare_efficiency(input);
}
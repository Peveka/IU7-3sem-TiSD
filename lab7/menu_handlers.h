#ifndef MENU_HANDLERS_H
#define MENU_HANDLERS_H

// BST 
void bst_add_element_handler(void);
void bst_remove_element_handler(void);
void bst_search_element_handler(void);
void bst_inorder_traversal_handler(void);
void bst_build_from_string_handler(void);
void bst_visualize_handler(void);
void bst_statistics_handler(void);
void bst_clear_handler(void);

// AVL 
void avl_add_element_handler(void);
void avl_remove_element_handler(void);
void avl_search_element_handler(void);
void avl_inorder_traversal_handler(void);
void avl_build_from_string_handler(void);
void avl_visualize_handler(void);
void avl_statistics_handler(void);
void avl_clear_handler(void);

// Hash table
void hash_table_build_from_string_handler(void);
void hash_table_add_element_handler(void);
void hash_table_remove_element_handler(void);
void hash_table_search_element_handler(void);
void hash_table_display_handler(void);
void hash_table_statistics_handler(void);
void hash_table_clear_handler(void);
void hash_table_rehash_handler(void);

// Hash Chain 
void hash_table_chain_build_from_string_handler(void);
void hash_table_chain_add_element_handler(void);
void hash_table_chain_remove_element_handler(void);
void hash_table_chain_search_element_handler(void);
void hash_table_chain_display_handler(void);
void hash_table_chain_statistics_handler(void);
void hash_table_chain_clear_handler(void);
void hash_table_chain_rehash_handler(void);

void compare_efficiency_handler(void);
#endif
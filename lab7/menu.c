#include "avl.h"
#include "bst.h"
#include "hash_table.h"
#include "hash_table_chain.h"
#include "menu_handlers.h"
#include <stdio.h>

static void display_menu(void) {
    printf("\n========== !Programm menu! ==========\n");
    printf("=== Binary Search Tree (BST) ===\n");
    printf("1. Add element to BST\n");
    printf("2. Remove element from BST\n");
    printf("3. Search element in BST\n");
    printf("4. Visualize BST (Graphviz)\n");
    printf("5. BST In-order traversal\n");
    printf("6. Build BST from string\n");
    printf("7. BST statistics\n");
    printf("8. Clear BST\n");
    
    printf("\n=== AVL Tree ===\n");
    printf("9. Add element to AVL\n");
    printf("10. Remove element from AVL\n");
    printf("11. Search element in AVL\n");
    printf("12. Visualize AVL (Graphviz)\n");
    printf("13. AVL In-order traversal\n");
    printf("14. Build AVL from string\n");
    printf("15. AVL statistics\n");
    printf("16. Clear AVL\n");
    
    printf("\n=== Hash Table (Open Addressing) ===\n");
    printf("17. Build Hash Table (Open) from string\n");
    printf("18. Add element to Hash Table (Open)\n");
    printf("19. Remove element from Hash Table (Open)\n");
    printf("20. Search element in Hash Table (Open)\n");
    printf("21. Display Hash Table (Open)\n");
    printf("22. Hash Table (Open) statistics\n");
    printf("23. Rehash Hash Table (Open)\n");
    printf("24. Clear Hash Table (Open)\n");
    
    printf("\n=== Hash Table (Chaining) ===\n");
    printf("25. Build Hash Table (Chain) from string\n");
    printf("26. Add element to Hash Table (Chain)\n");
    printf("27. Remove element from Hash Table (Chain)\n");
    printf("28. Search element in Hash Table (Chain)\n");
    printf("29. Display Hash Table (Chain)\n");
    printf("30. Hash Table (Chain) statistics\n");
    printf("31. Rehash Hash Table (Chain)\n");
    printf("32. Clear Hash Table (Chain)\n");
    
    printf("\n=== Comparison ===\n");
    printf("33. Compare efficiency\n");

    printf("\n=== Exit ===\n");
    printf("0. Exit program\n");
    printf("===========================================\n");
    printf("Enter your choice: ");
}

void main_menu(void) {
    int choice;
    
    display_menu();
    
    while (1) {
        printf("\n> ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 0:
                bst_clear_handler();
                avl_clear_handler();
                hash_table_clear_handler();
                hash_table_chain_clear_handler();
                printf("Exiting program.\n");
                return;
                
            // BST 
            case 1: bst_add_element_handler(); break;
            case 2: bst_remove_element_handler(); break;
            case 3: bst_search_element_handler(); break;
            case 4: bst_visualize_handler(); break;
            case 5: bst_inorder_traversal_handler(); break;
            case 6: bst_build_from_string_handler(); break;
            case 7: bst_statistics_handler(); break;
            case 8: bst_clear_handler(); break;
            
            // AVL   
            case 9: avl_add_element_handler(); break;
            case 10: avl_remove_element_handler(); break;
            case 11: avl_search_element_handler(); break;
            case 12: avl_visualize_handler(); break;
            case 13: avl_inorder_traversal_handler(); break;
            case 14: avl_build_from_string_handler(); break;
            case 15: avl_statistics_handler(); break;
            case 16: avl_clear_handler(); break;
            
            //hash table open
            case 17: hash_table_build_from_string_handler(); break;
            case 18: hash_table_add_element_handler(); break;
            case 19: hash_table_remove_element_handler(); break;
            case 20: hash_table_search_element_handler(); break;
            case 21: hash_table_display_handler(); break;
            case 22: hash_table_statistics_handler(); break;
            case 23: hash_table_rehash_handler(); break;
            case 24: hash_table_clear_handler(); break;
            
            //hash table chain
            case 25: hash_table_chain_build_from_string_handler(); break;
            case 26: hash_table_chain_add_element_handler(); break;
            case 27: hash_table_chain_remove_element_handler(); break;
            case 28: hash_table_chain_search_element_handler(); break;
            case 29: hash_table_chain_display_handler(); break;
            case 30: hash_table_chain_statistics_handler(); break;
            case 31: hash_table_chain_rehash_handler(); break;
            case 32: hash_table_chain_clear_handler(); break;
            
            //compare
            case 33: compare_efficiency_handler(); break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        
        if (choice != 0) {
            display_menu();
        }
    }
}
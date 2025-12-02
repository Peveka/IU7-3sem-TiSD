#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void compare_different_trees(void);

static void display_menu(void)
{
    printf("\n========== BINARY SEARCH TREE ==========\n");
    printf("1. Add element to tree\n");
    printf("2. Remove element from tree\n");
    printf("3. Search element in tree\n");
    printf("4. Visualize tree (console)\n");
    printf("5. Visualize tree (Graphviz)\n");
    printf("6. Postorder traversal\n");
    printf("7. Tree statistics\n");
    printf("8. Build tree from string\n");
    printf("9. Remove duplicates from tree\n");
    printf("10. Compare efficiency\n");
    printf("11. Compare different tree configurations\n");
    printf("12. Clear tree\n");
    printf("0. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

static void add_element_menu(tree_node **root)
{
    char element;
    printf("Enter character to add: ");
    if (scanf(" %c", &element) != 1)
    {
        printf("Error reading input\n");
        while (getchar() != '\n')
            ;
        return;
    }

    if (!isalnum((unsigned char)element))
    {
        printf("Error: Only letters and digits are allowed\n");
        return;
    }

    *root = insert_node(*root, element);
    printf("Element '%c' added to tree\n", element);
}

static void remove_element_menu(tree_node **root)
{
    if (*root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    char element;
    printf("Enter character to remove: ");
    if (scanf(" %c", &element) != 1)
    {
        printf("Error reading input\n");
        while (getchar() != '\n')
            ;
        return;
    }

    tree_node *found = find_node(*root, element);
    if (found == NULL)
    {
        printf("Element '%c' not found in tree\n", element);
        return;
    }

    *root = delete_node(*root, element);
    printf("Element '%c' removed from tree\n", element);
}

static void search_element_menu(tree_node *root)
{
    char element;
    printf("Enter character to search: ");
    if (scanf(" %c", &element) != 1)
    {
        printf("Error reading input\n");
        while (getchar() != '\n')
            ;
        return;
    }

    tree_node *found = find_node(root, element);
    if (found != NULL)
    {
        printf("Element '%c' found in tree (count: %d)\n",
               found->data, found->count);
    }
    else
    {
        printf("Element '%c' not found in tree\n", element);
    }
}

static void postorder_menu(tree_node *root)
{
    if (root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    printf("Postorder traversal: ");
    postorder_traversal(root);
    printf("\n");
}

static void statistics_menu(tree_node *root)
{
    if (root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    print_tree_statistics(root);
}

static void build_tree_from_string_menu(tree_node **root)
{
    char input[MAX_INPUT_SIZE];
    printf("Enter string to build tree from: ");

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Error reading input\n");
        return;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0)
    {
        printf("Empty input string\n");
        return;
    }

    printf("String length: %zu characters\n", strlen(input));

    if (*root != NULL)
    {
        free_tree(*root);
    }

    *root = build_tree_from_string(input);
    printf("Tree built from string: %s\n", input);

    printf("Letters with duplicates marked in red:\n");
    print_with_duplicates(*root);
    printf("\n");
}

static void compare_efficiency_menu(tree_node *root)
{
    char input[MAX_INPUT_SIZE];
    printf("Enter string for efficiency comparison: ");

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        printf("Error reading input\n");
        return;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0)
    {
        printf("Empty string entered\n");
        return;
    }

    printf("\nAnalyzing string of length %zu...\n", strlen(input));
    compare_efficiency(input);
}

void main_menu(void)
{
    tree_node *root = NULL;
    int choice;

    display_menu();

    while (1)
    {
        printf("\n> ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 0:
            free_tree(root);
            printf("Exiting program. Goodbye!\n");
            return;

        case 1:
            add_element_menu(&root);
            break;

        case 2:
            remove_element_menu(&root);
            break;

        case 3:
            search_element_menu(root);
            break;

        case 4:
            print_tree_visual(root);
            break;

        case 5:
            visualize_tree_graphviz(root, "tree.dot");
            break;

        case 6:
            postorder_menu(root);
            break;

        case 7:
            statistics_menu(root);
            break;

        case 8:
            build_tree_from_string_menu(&root);
            break;

        case 9:
            if (root == NULL)
            {
                printf("Tree is empty\n");
            }
            else
            {
                root = remove_duplicates_from_tree(root);
                printf("Duplicates removed from tree\n");
                print_tree_visual(root);
            }
            break;

        case 10:
            compare_efficiency_menu(root);
            break;

        case 11:
            compare_different_trees();
            break;

        case 12:
            free_tree(root);
            root = NULL;
            printf("Tree cleared\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
        display_menu();
    }
}
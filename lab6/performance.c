#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

static void remove_duplicates_from_string(char *str)
{
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = i + 1; j < len; j++)
        {
            if (str[i] == str[j])
            {
                for (size_t k = j; k < len; k++)
                {
                    str[k] = str[k + 1];
                }
                len--;
                j--;
            }
        }
    }
}

void compare_efficiency(const char *input_string)
{
    clock_t start_time, end_time;
    double tree_time_total = 0, string_time_total = 0;
    const int ITERATIONS = 100;

    size_t input_len = strlen(input_string);

    printf("\n=== COMPARING DUPLICATE REMOVAL ===\n");
    printf("Input string: %s\n", input_string);
    printf("Input length: %zu chars\n\n", input_len);

    // 1. Показываем начальное дерево
    tree_node *initial_tree = build_tree_from_string(input_string);
    printf("1. Initial tree:\n");
    if (initial_tree)
    {
        print_tree_visual(initial_tree);
        printf("In-order with duplicates: ");
        print_with_duplicates(initial_tree);
        printf("\n");
    }
    else
    {
        printf("(empty)\n");
    }

    // 2. Тестируем дерево
    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        tree_node *tree_copy = build_tree_from_string(input_string);

        start_time = clock();
        tree_node *result = remove_duplicates_from_tree(tree_copy);
        end_time = clock();

        tree_time_total += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        if (result)
            free_tree(result);
    }
    double tree_time_avg = tree_time_total / ITERATIONS;

    // 3. Тестируем строку
    size_t result_len = 0;
    char *final_string = NULL;

    for (int iter = 0; iter < ITERATIONS; iter++)
    {
        char *string_copy = (char *)malloc(input_len + 1);
        strcpy(string_copy, input_string);

        start_time = clock();
        remove_duplicates_from_string(string_copy);
        end_time = clock();

        string_time_total += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        if (iter == ITERATIONS - 1)
        {
            result_len = strlen(string_copy);
            final_string = (char *)malloc(result_len + 1);
            strcpy(final_string, string_copy);
        }

        free(string_copy);
    }
    double string_time_avg = string_time_total / ITERATIONS;

    // 4. Показываем конечное дерево
    tree_node *final_tree = build_tree_from_string(input_string);
    tree_node *result_tree = remove_duplicates_from_tree(final_tree);

    printf("\n2. Tree after removing duplicates:\n");
    if (result_tree)
    {
        print_tree_visual(result_tree);
        printf("Post-order traversal: ");
        postorder_traversal(result_tree);
        printf("\n");
    }
    else
    {
        printf("(empty tree)\n");
    }

    // 5. Показываем конечную строку
    printf("\n3. String after removing duplicates:\n");
    if (final_string)
    {
        printf("%s\n", final_string);
        free(final_string);
    }

    // 6. Статистика
    int unique_count = initial_tree ? get_node_count(initial_tree) : 0;
    int tree_height = initial_tree ? get_tree_height(initial_tree) : 0;
    int final_count = result_tree ? get_node_count(result_tree) : 0;

    printf("\n=== STATISTICS ===\n");
    printf("Initial length: %zu chars\n", input_len);
    printf("Final length: %zu chars\n", result_len);
    printf("Removed: %zu chars\n", input_len - result_len);
    printf("Unique elements in tree: %d\n", unique_count);
    printf("Elements in final tree: %d\n", final_count);
    printf("Tree height: %d\n", tree_height);

    // Баланс
    if (initial_tree)
    {
        int left_h = initial_tree->left ? get_tree_height(initial_tree->left) : 0;
        int right_h = initial_tree->right ? get_tree_height(initial_tree->right) : 0;
        printf("Balanced: %s (diff: %d)\n",
               abs(left_h - right_h) <= 1 ? "YES" : "NO",
               abs(left_h - right_h));
    }
    else
    {
        printf("Balanced: N/A\n");
    }

    printf("\n=== PERFORMANCE (avg of %d runs) ===\n", ITERATIONS);
    printf("Tree time:   %.8f sec\n", tree_time_avg);
    printf("String time: %.8f sec\n", string_time_avg);

    if (tree_time_avg < string_time_avg)
    {
        printf("Tree is %.2fx faster\n", string_time_avg / tree_time_avg);
    }
    else if (string_time_avg < tree_time_avg)
    {
        printf("String is %.2fx faster\n", tree_time_avg / string_time_avg);
    }
    else
    {
        printf("Same speed\n");
    }

    printf("====================================\n");

    // Очистка
    if (initial_tree)
        free_tree(initial_tree);
    if (result_tree)
        free_tree(result_tree);
}

void compare_different_trees(void)
{
    printf("\n=== COMPARING DIFFERENT TREE CONFIGURATIONS ===\n");

    const char *test_cases[] =
        {
            "ABCDEFGHIJ",
            "FACEBDGIHJK",
            "AAABBBCCCDDD",
            "ZYXWVUTSRQ",
            "MJNKBVCFXZAQWE",
            "ikissthegirlilikeitandstillrememberthisdaymyfriendsoitshardtobreath",
            "88888888asdadsa",
            "1234567890",
            "a1b2c3d4e5f6g7h8i9j0",
            "111222333444555666777888999000"};

    for (int i = 0; i < 10; i++)
    {
        printf("\nTest %d: %s\n", i + 1, test_cases[i]);
        compare_efficiency(test_cases[i]);
    }
}
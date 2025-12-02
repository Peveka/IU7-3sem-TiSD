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

    printf("\n3. String after removing duplicates:\n");
    if (final_string)
    {
        printf("%s\n", final_string);
        free(final_string);
    }

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

    if (initial_tree)
        free_tree(initial_tree);
    if (result_tree)
        free_tree(result_tree);
}

double measure_delete_time(tree_node *tree, char element, int iterations)
{
    if (tree == NULL)
        return 0.0;

    clock_t start_time, end_time;
    double total_time = 0;

    for (int i = 0; i < iterations; i++)
    {
        tree_node *copy = NULL;
        start_time = clock();
        delete_node(tree, element);
        end_time = clock();
        total_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    return total_time / iterations;
}

double measure_inorder_time(tree_node *tree, int iterations)
{
    if (tree == NULL)
        return 0.0;

    clock_t start_time, end_time;
    double total_time = 0;

    FILE *null_output = fopen("/dev/null", "w");
    FILE *original_stdout = stdout;

    for (int i = 0; i < iterations; i++)
    {
        stdout = null_output;

        start_time = clock();
        print_with_duplicates(tree);
        end_time = clock();

        stdout = original_stdout;

        total_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    if (null_output)
        fclose(null_output);
    stdout = original_stdout;

    return total_time / iterations;
}

void benchmark_sort_and_search(const char *input_string)
{
    printf("\n=== BENCHMARK SORT AND SEARCH ===\n");
    printf("Input string: %s\n", input_string);
    printf("String length: %zu\n\n", strlen(input_string));

    const int ITERATIONS = 10000;

    tree_node *tree = build_tree_from_string(input_string);
    if (tree == NULL)
    {
        printf("Cannot build tree\n");
        return;
    }

    visualize_tree_graphviz(tree, "benchmark_tree.dot");

    printf("Generating PNG image...\n");
    system("dot -Tpng benchmark_tree.dot -o benchmark_tree.png 2>/dev/null");
    printf("Tree saved as: benchmark_tree.png\n\n");

    printf("In-order traversal (once): ");
    inorder_traversal(tree);
    printf("\n\n");

    clock_t start_time, end_time;
    double total_inorder_time = 0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        start_time = clock();
        tree_node *stack[256];
        int top = -1;
        tree_node *current = tree;

        while (current != NULL || top >= 0)
        {
            while (current != NULL)
            {
                stack[++top] = current;
                current = current->left;
            }

            current = stack[top--];
            current = current->right;
        }
        end_time = clock();
        total_inorder_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    }

    double avg_inorder_time = total_inorder_time / ITERATIONS;

    double total_search_time = 0;
    int search_tests = 0;

    for (size_t i = 0; input_string[i] != '\0'; i++)
    {
        if (i % 2 == 0)
        {
            char element = input_string[i];
            if (!isalnum((unsigned char)element))
                continue;

            double search_time_sum = 0;

            for (int j = 0; j < ITERATIONS; j++)
            {
                start_time = clock();
                tree_node *found = find_node(tree, element);
                end_time = clock();
                search_time_sum += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            }

            total_search_time += search_time_sum / ITERATIONS;
            search_tests++;
        }
    }

    double avg_search_time = search_tests > 0 ? total_search_time / search_tests : 0;

    printf("=== RESULTS (avg of %d runs) ===\n", ITERATIONS);
    printf("In-order traversal time: %.10f sec\n", avg_inorder_time);
    printf("Average search time:     %.10f sec\n", avg_search_time);
    printf("Elements tested (even positions): %d\n", search_tests);

    printf("\n=== TREE INFO ===\n");
    printf("Total nodes: %d\n", get_node_count(tree));
    printf("Tree height: %d\n", get_tree_height(tree));
    printf("Unique chars in string: %d\n", get_node_count(tree));

    free_tree(tree);
    printf("==============================\n");
}

void benchmark_different_configs()
{
    printf("\n=== BENCHMARK DIFFERENT CONFIGURATIONS ===\n");

    const char *tests[] = {
        "ABCDEFGHIJ",
        "FACEBDGIHJK",
        "AAABBBCCCDDD",
        "ZYXWVUTSRQ",
        "MJNKBVCFXZAQWE",
        "1234567890"};

    for (int i = 0; i < 6; i++)
    {
        printf("\nTest %d: %s\n", i + 1, tests[i]);
        benchmark_sort_and_search(tests[i]);
    }
}

void benchmark_tree_operations(const char *input_string)
{
    printf("\n=== BENCHMARK TREE OPERATIONS ===\n");
    printf("Input string: %s\n", input_string);
    printf("String length: %zu\n", strlen(input_string));

    const int ITERATIONS = 1000;

    tree_node *tree = build_tree_from_string(input_string);
    if (tree == NULL)
    {
        printf("Error: Cannot build tree from string\n");
        return;
    }

    double inorder_time = measure_inorder_time(tree, ITERATIONS);

    char test_chars[256];
    int test_index = 0;
    for (int i = 0; i < strlen(input_string); ++i)
        if (i % 2 == 0)
            test_chars[test_index++] = input_string[i];
        
    int num_tests = sizeof(test_chars) / sizeof(test_chars[0]);

    printf("\n=== RESULTS (avg of %d iterations) ===\n", ITERATIONS);
    printf("In-order traversal time: %.10f sec\n", inorder_time);

    printf("\nDelete operation times:\n");
    printf("------------------------\n");

    for (int i = 0; i < num_tests; i++)
    {
        tree_node *found = find_node(tree, test_chars[i]);
        if (found)
        {
            double delete_time = measure_delete_time(tree, test_chars[i], ITERATIONS);
            printf("Delete '%c' (count: %d): %.10f sec\n",
                   test_chars[i], found->count, delete_time);
        }
    }

    printf("\nTree statistics:\n");
    printf("----------------\n");
    printf("Total nodes: %d\n", get_node_count(tree));
    printf("Tree height: %d\n", get_tree_height(tree));
    printf("Leaf nodes: %d\n", get_leaf_count(tree));

    int balance_factor = get_tree_balance_factor(tree);
    printf("\nBalance factor: %d\n", balance_factor);
    if (abs(balance_factor) <= 1)
    {
        printf("Tree is balanced\n");
    }
    else
    {
        printf("Tree is unbalanced\n");
    }

    printf("=====================================\n");

    free_tree(tree);
}

void benchmark_different_trees()
{
    printf("\n=== BENCHMARK DIFFERENT TREE CONFIGURATIONS ===\n");

    const char *test_cases[] = {
        // 1. Сбалансированное дерево (алфавит)
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",

        // 2. Несбалансированное дерево (отсортированная последовательность)
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",

        // 3. Случайные символы (скорее сбалансированное)
        "MJNKBVCFXZAQWE",

        // 4. Много дубликатов
        "AAAABBBBCCCCDDDDEEEEFFFF",

        // 5. Краткая строка
        "ABCD",

        // 6. Длинная строка со случайными символами
        "a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q7r8s9t0",

        // 7. Почти сбалансированное
        "FACEBDGIHJK",

        // 8. Крайне несбалансированное
        "ZYXWVUTSRQPONMLKJIHGFEDCBA"};

    const char *descriptions[] = {
        "1. Balanced tree (alphabet)",
        "2. Unbalanced tree (sorted sequence)",
        "3. Random chars (likely balanced)",
        "4. Many duplicates",
        "5. Short string",
        "6. Long random string",
        "7. Nearly balanced",
        "8. Extremely unbalanced"};

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_tests; i++)
    {
        printf("\n%s\n", descriptions[i]);
        printf("String: %s\n", test_cases[i]);
        benchmark_tree_operations(test_cases[i]);
    }

    printf("\n=== SUMMARY ===\n");
    printf("Balanced trees generally provide O(log n) performance.\n");
    printf("Unbalanced trees can degrade to O(n) performance.\n");
    printf("Duplicate handling affects memory usage but not search time.\n");
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

        tree_node *tree = build_tree_from_string(test_cases[i]);
        if (tree)
        {
            char filename_dot[50];
            char filename_png[50];

            snprintf(filename_dot, sizeof(filename_dot), "tree_test_%d.dot", i + 1);
            snprintf(filename_png, sizeof(filename_png), "tree_test_%d.png", i + 1);

            visualize_tree_graphviz(tree, filename_dot);

            char command[100];
            snprintf(command, sizeof(command), "dot -Tpng %s -o %s 2>/dev/null", filename_dot, filename_png);
            system(command);

            printf("Tree saved as: %s\n", filename_png);

            free_tree(tree);
        }

        compare_efficiency(test_cases[i]);
    }
}
#define _POSIX_C_SOURCE 199309L

#include "bst.h"
#include "avl.h"
#include "hash_table.h"
#include "hash_table_chain.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SEARCH_ITERATIONS 50000
static long long get_nanoseconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
}

static size_t calculate_bst_memory(tree_node *root)
{
    if (!root)
        return 0;
    return sizeof(tree_node) + calculate_bst_memory(root->left) + calculate_bst_memory(root->right);
}

static size_t calculate_avl_memory(avl_node *root)
{
    if (!root)
        return 0;
    return sizeof(avl_node) + calculate_avl_memory(root->left) + calculate_avl_memory(root->right);
}

static size_t calculate_hash_open_memory(hash_table_t *ht)
{
    if (!ht)
        return 0;
    return sizeof(hash_table_t) + sizeof(hash_table_entry_t) * ht->capacity;
}

static size_t calculate_hash_chain_memory(hash_table_chain_t *ht)
{
    if (!ht)
        return 0;
    size_t memory = sizeof(hash_table_chain_t) + sizeof(hash_chain_node_t *) * ht->capacity;
    for (int i = 0; i < ht->capacity; i++)
    {
        hash_chain_node_t *current = ht->buckets[i];
        while (current)
        {
            memory += sizeof(hash_chain_node_t);
            current = current->next;
        }
    }
    return memory;
}

static void collect_unique_chars_bst(tree_node *root, char *chars, int *index)
{
    if (root)
    {
        collect_unique_chars_bst(root->left, chars, index);
        chars[(*index)++] = root->data;
        collect_unique_chars_bst(root->right, chars, index);
    }
}

static volatile char g_dummy_result;

static double measure_avg_search_time_bst(tree_node *root, const char *unique_chars, int num_unique)
{
    if (!root || num_unique == 0)
        return 0.0;

    long long total_time_ns = 0;
    int dummy_counter = 0; 
    for (int i = 0; i < num_unique; i++)
    {
        char search_char = unique_chars[i];
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++)
        {
            long long start = get_nanoseconds();
            tree_node *found = find_node(root, search_char);
            long long end = get_nanoseconds();

            if (found)
            {
                g_dummy_result = found->data; 
                dummy_counter++;
            }

            total_time_ns += (end - start);
        }
    }

    {
        g_dummy_result = 'X';
    }

    double avg_time_us = (double)total_time_ns / (num_unique * SEARCH_ITERATIONS) / 1000.0;
    return avg_time_us;
}

static double measure_avg_search_time_avl(avl_node *root, const char *unique_chars, int num_unique)
{
    if (!root || num_unique == 0)
        return 0.0;

    long long total_time_ns = 0;
    int dummy_counter = 0;

    for (int i = 0; i < num_unique; i++)
    {
        char search_char = unique_chars[i];
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++)
        {
            long long start = get_nanoseconds();
            avl_node *found = avl_find(root, search_char);
            long long end = get_nanoseconds();

            if (found)
            {
                g_dummy_result = found->data;
                dummy_counter++;
            }

            total_time_ns += (end - start);
        }
    }

    if (dummy_counter == 0)
    {
        g_dummy_result = 'X';
    }

    double avg_time_us = (double)total_time_ns / (num_unique * SEARCH_ITERATIONS) / 1000.0;
    return avg_time_us;
}

static double measure_avg_search_time_hash_open(hash_table_t *ht, const char *unique_chars, int num_unique)
{
    if (!ht || num_unique == 0)
        return 0.0;

    long long total_time_ns = 0;
    int dummy_counter = 0;

    for (int i = 0; i < num_unique; i++)
    {
        char search_char = unique_chars[i];
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++)
        {
            long long start = get_nanoseconds();
            hash_table_entry_t *found = hash_table_search(ht, search_char);
            long long end = get_nanoseconds();

            if (found)
            {
                g_dummy_result = found->key;
                dummy_counter++;
            }

            total_time_ns += (end - start);
        }
    }

    if (dummy_counter == 0)
    {
        g_dummy_result = 'X';
    }

    double avg_time_us = (double)total_time_ns / (num_unique * SEARCH_ITERATIONS) / 1000.0;
    return avg_time_us;
}

static double measure_avg_search_time_hash_chain(hash_table_chain_t *ht, const char *unique_chars, int num_unique)
{
    if (!ht || num_unique == 0)
        return 0.0;

    long long total_time_ns = 0;
    int dummy_counter = 0;

    for (int i = 0; i < num_unique; i++)
    {
        char search_char = unique_chars[i];
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++)
        {
            long long start = get_nanoseconds();
            hash_chain_node_t *found = hash_table_chain_search(ht, search_char);
            long long end = get_nanoseconds();

            if (found)
            {
                g_dummy_result = found->key;
                dummy_counter++;
            }

            total_time_ns += (end - start);
        }
    }

    if (dummy_counter == 0)
    {
        g_dummy_result = 'X';
    }

    double avg_time_us = (double)total_time_ns / (num_unique * SEARCH_ITERATIONS) / 1000.0;
    return avg_time_us;
}

static double calculate_avg_comparisons_bst(tree_node *root, const char *unique_chars, int num_unique)
{
    if (!root || num_unique == 0)
        return 0.0;

    long long total_comp = 0;
    for (int i = 0; i < num_unique; i++)
    {
        int comp = 0;
        find_node_with_comp(root, unique_chars[i], &comp);
        total_comp += comp;
    }
    return (double)total_comp / num_unique;
}

static double calculate_avg_comparisons_avl(avl_node *root, const char *unique_chars, int num_unique)
{
    if (!root || num_unique == 0)
        return 0.0;

    long long total_comp = 0;
    for (int i = 0; i < num_unique; i++)
    {
        int comp = 0;
        avl_find_with_comp(root, unique_chars[i], &comp);
        total_comp += comp;
    }
    return (double)total_comp / num_unique;
}

void compare_efficiency(const char *input_string)
{
    if (!input_string || strlen(input_string) == 0)
    {
        printf("Empty input string.\n");
        return;
    }

    printf("\n=== Building data structures... ===\n");

    // Строим структуры
    tree_node *bst = build_tree_from_string(input_string);
    avl_node *avl = avl_build_from_string(input_string);
    hash_table_t *hash_open = hash_table_create_from_string(input_string, HASH_DIVISION);
    hash_table_chain_t *hash_chain = hash_table_chain_create_from_string(input_string, HASH_CHAIN_DIVISION);

    if (!bst || !avl || !hash_open || !hash_chain)
    {
        printf("Error building one or more data structures.\n");
        if (bst)
            free_tree(bst);
        if (avl)
            avl_free_tree(avl);
        if (hash_open)
            hash_table_destroy(hash_open);
        if (hash_chain)
            hash_table_chain_destroy(hash_chain);
        return;
    }

    // Получаем уникальные символы
    int num_unique = get_node_count(bst);
    if (num_unique == 0)
    {
        printf("No unique characters in string.\n");
        free_tree(bst);
        avl_free_tree(avl);
        hash_table_destroy(hash_open);
        hash_table_chain_destroy(hash_chain);
        return;
    }

    char *unique_chars = malloc(num_unique * sizeof(char));
    if (!unique_chars)
    {
        printf("Memory allocation failed.\n");
        free_tree(bst);
        avl_free_tree(avl);
        hash_table_destroy(hash_open);
        hash_table_chain_destroy(hash_chain);
        return;
    }

    int index = 0;
    collect_unique_chars_bst(bst, unique_chars, &index);

    printf("=== Measuring performance... (this may take a moment) ===\n");
    printf("Performing %d search iterations for each of %d unique characters...\n",
           SEARCH_ITERATIONS, num_unique);
    printf("Total searches: %d\n\n", SEARCH_ITERATIONS * num_unique);

    // Измеряем память
    size_t mem_bst = calculate_bst_memory(bst);
    size_t mem_avl = calculate_avl_memory(avl);
    size_t mem_hash_open = calculate_hash_open_memory(hash_open);
    size_t mem_hash_chain = calculate_hash_chain_memory(hash_chain);

    // Измеряем время поиска
    printf("Measuring BST search time...\n");
    double time_bst = measure_avg_search_time_bst(bst, unique_chars, num_unique);

    printf("Measuring AVL search time...\n");
    double time_avl = measure_avg_search_time_avl(avl, unique_chars, num_unique);

    printf("Measuring Hash Table (Open) search time...\n");
    double time_hash_open = measure_avg_search_time_hash_open(hash_open, unique_chars, num_unique);

    printf("Measuring Hash Table (Chain) search time...\n");
    double time_hash_chain = measure_avg_search_time_hash_chain(hash_chain, unique_chars, num_unique);

    // Измеряем среднее количество сравнений
    double comp_bst = calculate_avg_comparisons_bst(bst, unique_chars, num_unique);
    double comp_avl = calculate_avg_comparisons_avl(avl, unique_chars, num_unique);
    double comp_hash_open = hash_open ? (double)hash_table_get_average_comparisons(hash_open) : 0.0;
    double comp_hash_chain = hash_chain ? (double)hash_table_chain_get_average_comparisons(hash_chain) : 0.0;

    // Получаем дополнительные метрики
    int height_bst = get_tree_height(bst);
    int height_avl = avl_get_height(avl);
    int collisions_open = hash_open ? hash_open->collisions : 0;
    int collisions_chain = hash_chain ? hash_chain->collisions : 0;

    // Выводим результаты
    printf("\n=== Efficiency Comparison (Average per search) ===\n");
    printf("Input string: \"%s\"\n", input_string);
    printf("String length: %zu, Unique characters: %d\n\n", strlen(input_string), num_unique);

    printf("| Structure       | Memory (B) | Height | Collisions | Avg Time (µs) | Avg Comparisons |\n");
    printf("|-----------------|------------|--------|------------|---------------|-----------------|\n");
    printf("| BST             | %10zu | %6d |     -      | %13.3f | %15.1f |\n",
           mem_bst, height_bst, time_bst, comp_bst);
    printf("| AVL             | %10zu | %6d |     -      | %13.3f | %15.1f |\n",
           mem_avl, height_avl, time_avl, comp_avl);
    printf("| Hash Open       | %10zu |   -    | %10d | %13.3f | %15.1f |\n",
           mem_hash_open, collisions_open, time_hash_open, comp_hash_open);
    printf("| Hash Chain      | %10zu |   -    | %10d | %13.3f | %15.1f |\n",
           mem_hash_chain, collisions_chain, time_hash_chain, comp_hash_chain);
    printf("\n");

    // Дополнительный анализ
    printf("=== Performance Analysis ===\n");
    if (time_avl > 0)
    {
        printf("BST vs AVL time ratio: %.2fx\n", time_bst / time_avl);
    }
    if (comp_avl > 0)
    {
        printf("BST vs AVL comparison ratio: %.2fx\n", comp_bst / comp_avl);
    }

    printf("Best search time: ");
    double min_time = time_bst;
    const char *best_structure = "BST";

    if (time_avl > 0 && time_avl < min_time)
    {
        min_time = time_avl;
        best_structure = "AVL";
    }
    if (time_hash_open > 0 && time_hash_open < min_time)
    {
        min_time = time_hash_open;
        best_structure = "Hash Table (Open)";
    }
    if (time_hash_chain > 0 && time_hash_chain < min_time)
    {
        min_time = time_hash_chain;
        best_structure = "Hash Table (Chain)";
    }

    printf("%s (%.3f µs)\n", best_structure, min_time);
    printf("==================================================\n");

    free(unique_chars);
    free_tree(bst);
    avl_free_tree(avl);
    hash_table_destroy(hash_open);
    hash_table_chain_destroy(hash_chain);
}
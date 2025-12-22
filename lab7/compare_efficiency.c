#include "bst.h"
#include "avl.h"
#include "hash_table.h"
#include "hash_table_chain.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SEARCH_ITERATIONS 1000

static size_t calculate_bst_memory(tree_node* root) {
    if (!root) return 0;
    return sizeof(tree_node) + calculate_bst_memory(root->left) + calculate_bst_memory(root->right);
}

static size_t calculate_avl_memory(avl_node* root) {
    if (!root) return 0;
    return sizeof(avl_node) + calculate_avl_memory(root->left) + calculate_avl_memory(root->right);
}

static size_t calculate_hash_open_memory(hash_table_t* ht) {
    if (!ht) return 0;
    return sizeof(hash_table_t) + sizeof(hash_table_entry_t) * ht->capacity;
}

static size_t calculate_hash_chain_memory(hash_table_chain_t* ht) {
    if (!ht) return 0;
    size_t memory = sizeof(hash_table_chain_t) + sizeof(hash_chain_node_t*) * ht->capacity;
    for (int i = 0; i < ht->capacity; i++) {
        hash_chain_node_t* current = ht->buckets[i];
        while (current) {
            memory += sizeof(hash_chain_node_t);
            current = current->next;
        }
    }
    return memory;
}

static void collect_unique_chars_bst(tree_node* root, char* chars, int* index) {
    if (root) {
        collect_unique_chars_bst(root->left, chars, index);
        chars[(*index)++] = root->data;
        collect_unique_chars_bst(root->right, chars, index);
    }
}

static double measure_avg_search_time_bst(tree_node* root, const char* unique_chars, int num_unique) {
    clock_t total_time = 0;
    for (int i = 0; i < num_unique; i++) {
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++) {
            clock_t start = clock();
            find_node(root, unique_chars[i]);
            total_time += clock() - start;
        }
    }
    return (double)total_time / (num_unique * SEARCH_ITERATIONS) / CLOCKS_PER_SEC * 1e6;  // среднее в мкс
}

static double measure_avg_search_time_avl(avl_node* root, const char* unique_chars, int num_unique) {
    clock_t total_time = 0;
    for (int i = 0; i < num_unique; i++) {
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++) {
            clock_t start = clock();
            avl_find(root, unique_chars[i]);
            total_time += clock() - start;
        }
    }
    return (double)total_time / (num_unique * SEARCH_ITERATIONS) / CLOCKS_PER_SEC * 1e6;
}

static double measure_avg_search_time_hash_open(hash_table_t* ht, const char* unique_chars, int num_unique) {
    clock_t total_time = 0;
    for (int i = 0; i < num_unique; i++) {
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++) {
            clock_t start = clock();
            hash_table_search(ht, unique_chars[i]);
            total_time += clock() - start;
        }
    }
    return (double)total_time / (num_unique * SEARCH_ITERATIONS) / CLOCKS_PER_SEC * 1e6;
}

static double measure_avg_search_time_hash_chain(hash_table_chain_t* ht, const char* unique_chars, int num_unique) {
    clock_t total_time = 0;
    for (int i = 0; i < num_unique; i++) {
        for (int iter = 0; iter < SEARCH_ITERATIONS; iter++) {
            clock_t start = clock();
            hash_table_chain_search(ht, unique_chars[i]);
            total_time += clock() - start;
        }
    }
    return (double)total_time / (num_unique * SEARCH_ITERATIONS) / CLOCKS_PER_SEC * 1e6;
}

static double calculate_avg_comparisons_bst(tree_node* root, const char* unique_chars, int num_unique) {
    long long total_comp = 0;
    for (int i = 0; i < num_unique; i++) {
        int comp = 0;
        find_node_with_comp(root, unique_chars[i], &comp);
        total_comp += comp;
    }
    return (double)total_comp / num_unique;
}

static double calculate_avg_comparisons_avl(avl_node* root, const char* unique_chars, int num_unique) {
    long long total_comp = 0;
    for (int i = 0; i < num_unique; i++) {
        int comp = 0;
        avl_find_with_comp(root, unique_chars[i], &comp);
        total_comp += comp;
    }
    return (double)total_comp / num_unique;
}

void compare_efficiency(const char* input_string) {
    tree_node* bst = build_tree_from_string(input_string);
    avl_node* avl = avl_build_from_string(input_string);
    hash_table_t* hash_open = hash_table_create_from_string(input_string, HASH_DIVISION);
    hash_table_chain_t* hash_chain = hash_table_chain_create_from_string(input_string, HASH_CHAIN_DIVISION);

    int num_unique = avl ? avl_get_node_count(avl) : 0;
    if (num_unique == 0) {
        printf("No unique characters in string.\n");
        free_tree(bst);
        avl_free_tree(avl);
        if (hash_open) hash_table_destroy(hash_open);
        if (hash_chain) hash_table_chain_destroy(hash_chain);
        return;
    }

    char* unique_chars = malloc(num_unique * sizeof(char));
    int index = 0;
    collect_unique_chars_bst(bst, unique_chars, &index);

    size_t mem_bst = calculate_bst_memory(bst);
    size_t mem_avl = calculate_avl_memory(avl);
    size_t mem_hash_open = calculate_hash_open_memory(hash_open);
    size_t mem_hash_chain = calculate_hash_chain_memory(hash_chain);

    double time_bst = measure_avg_search_time_bst(bst, unique_chars, num_unique);
    double time_avl = measure_avg_search_time_avl(avl, unique_chars, num_unique);
    double time_hash_open = measure_avg_search_time_hash_open(hash_open, unique_chars, num_unique);
    double time_hash_chain = measure_avg_search_time_hash_chain(hash_chain, unique_chars, num_unique);

    double comp_bst = calculate_avg_comparisons_bst(bst, unique_chars, num_unique);
    double comp_avl = calculate_avg_comparisons_avl(avl, unique_chars, num_unique);
    double comp_hash_open = hash_open ? (double)hash_table_get_average_comparisons(hash_open) : 0.0;
    double comp_hash_chain = hash_chain ? (double)hash_table_chain_get_average_comparisons(hash_chain) : 0.0;

    int height_bst = get_tree_height(bst);
    int height_avl = avl_get_height(avl);
    int collisions_open = hash_open ? hash_open->collisions : 0;
    int collisions_chain = hash_chain ? hash_chain->collisions : 0;

    printf("\n=== Efficiency Comparison (1000 iterations per search) ===\n");
    printf("Input string: \"%s\"\n", input_string);
    printf("String length: %zu, Unique characters: %d\n\n", strlen(input_string), num_unique);

    printf("| Structure       | Memory (B) | Height | Collisions | Avg Time (ms) | Avg Comparisons |\n");
    printf("|-----------------|------------|--------|------------|---------------|-----------------|\n");
    printf("| BST             | %10zu | %6d |     -      | %13.3f | %15.2f |\n",
           mem_bst, height_bst, time_bst, comp_bst);
    printf("| AVL             | %10zu | %6d |     -      | %13.3f | %15.2f |\n",
           mem_avl, height_avl, time_avl, comp_avl);
    printf("| Hash Open       | %10zu |   -    | %10d | %13.3f | %15.2f |\n",
           mem_hash_open, collisions_open, time_hash_open, comp_hash_open);
    printf("| Hash Chain      | %10zu |   -    | %10d | %13.3f | %15.2f |\n",
           mem_hash_chain, collisions_chain, time_hash_chain, comp_hash_chain);
    printf("==================================================================================\n");

    free(unique_chars);
    free_tree(bst);
    avl_free_tree(avl);
    if (hash_open) hash_table_destroy(hash_open);
    if (hash_chain) hash_table_chain_destroy(hash_chain);
}
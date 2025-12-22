#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    HASH_DIVISION,
    HASH_MULTIPLICATION
} hash_function_type_t;

typedef struct hash_table_entry_t {
    char key;
    int value;
    bool is_occupied;
    bool is_deleted;
} hash_table_entry_t;

typedef struct hash_table_t {
    hash_table_entry_t* table;
    int capacity;
    int size;
    int collisions;
    hash_function_type_t hash_func_type;
    double load_factor;
} hash_table_t;

hash_table_t* hash_table_create(int initial_capacity, hash_function_type_t type);
void hash_table_destroy(hash_table_t* ht);
int hash_function(hash_table_t* ht, char key);
int division_hash(hash_table_t* ht, char key);
int multiplication_hash(hash_table_t* ht, char key);
bool hash_table_insert(hash_table_t* ht, char key, int value);
bool hash_table_insert_with_count(hash_table_t* ht, char key);
bool hash_table_remove(hash_table_t* ht, char key);
hash_table_entry_t* hash_table_search(hash_table_t* ht, char key);
void hash_table_display(hash_table_t* ht);
bool hash_table_needs_resize(hash_table_t* ht);
bool hash_table_needs_rehash(hash_table_t* ht);
hash_table_t* hash_table_resize(hash_table_t* ht);
hash_table_t* hash_table_rehash(hash_table_t* ht, hash_function_type_t new_type);
int hash_table_get_average_comparisons(hash_table_t* ht);
hash_table_t* hash_table_build_from_string(hash_table_t* ht, const char* str);
hash_table_t* hash_table_create_from_string(const char* str, hash_function_type_t type);
int linear_probe(hash_table_t* ht, char key, int start_index, bool for_insert, bool* had_collision);

#endif
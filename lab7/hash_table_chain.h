#ifndef HASH_TABLE_CHAIN_H
#define HASH_TABLE_CHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    HASH_CHAIN_DIVISION,
    HASH_CHAIN_MULTIPLICATION
} hash_chain_function_type_t;

typedef struct hash_chain_node_t {
    char key;                     
    int value;                    
    struct hash_chain_node_t* next;
} hash_chain_node_t;

typedef struct hash_table_chain_t {
    hash_chain_node_t** buckets;
    int capacity;                
    int size;                       
    int collisions;                 
    hash_chain_function_type_t hash_func_type;
    double load_factor;
} hash_table_chain_t;

hash_table_chain_t* hash_table_chain_create(int initial_capacity, hash_chain_function_type_t type);
void hash_table_chain_destroy(hash_table_chain_t* ht);
int hash_chain_function(hash_table_chain_t* ht, char key);
int hash_chain_division_hash(hash_table_chain_t* ht, char key);
int hash_chain_multiplication_hash(hash_table_chain_t* ht, char key);
hash_chain_node_t* hash_chain_create_node(char key, int value);
bool hash_table_chain_insert(hash_table_chain_t* ht, char key, int value);
bool hash_table_chain_insert_with_count(hash_table_chain_t* ht, char key);
bool hash_table_chain_remove(hash_table_chain_t* ht, char key);
hash_chain_node_t* hash_table_chain_search(hash_table_chain_t* ht, char key);
void hash_table_chain_display(hash_table_chain_t* ht);
bool hash_table_chain_needs_resize(hash_table_chain_t* ht);
bool hash_table_chain_needs_rehash(hash_table_chain_t* ht);
hash_table_chain_t* hash_table_chain_resize(hash_table_chain_t* ht);
hash_table_chain_t* hash_table_chain_rehash(hash_table_chain_t* ht, hash_chain_function_type_t new_type);
int hash_table_chain_get_average_comparisons(hash_table_chain_t* ht);
hash_table_chain_t* hash_table_chain_build_from_string(hash_table_chain_t* ht, const char* str);
hash_table_chain_t* hash_table_chain_create_from_string(const char* str, hash_chain_function_type_t type);

#endif
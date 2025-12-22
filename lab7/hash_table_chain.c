#include "hash_table_chain.h"
#include <math.h>

#define GOLDEN_RATIO 0.6180339887

static int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

static int next_prime(int n) {
    if (n <= 2) return 2;
    if (n % 2 == 0) n++;
    else if (n > 2) n += 2;
    while (!is_prime(n)) {
        n += 2;
    }
    return n;
}

hash_table_chain_t* hash_table_chain_create(int initial_capacity, hash_chain_function_type_t type) {
    hash_table_chain_t* ht = (hash_table_chain_t*)malloc(sizeof(hash_table_chain_t));
    if (!ht) return NULL;
    
    ht->capacity = initial_capacity;
    ht->size = 0;
    ht->collisions = 0;
    ht->hash_func_type = type;
    ht->load_factor = 0.0;
    
    ht->buckets = (hash_chain_node_t**)calloc(ht->capacity, sizeof(hash_chain_node_t*));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }
    
    return ht;
}

void hash_table_chain_destroy(hash_table_chain_t* ht) {
    if (ht) {
        for (int i = 0; i < ht->capacity; i++) {
            hash_chain_node_t* current = ht->buckets[i];
            while (current) {
                hash_chain_node_t* next = current->next;
                free(current);
                current = next;
            }
        }
        
        free(ht->buckets);
        free(ht);
    }
}

int hash_chain_division_hash(hash_table_chain_t* ht, char key) {
    return abs((int)key) % ht->capacity;
}

int hash_chain_multiplication_hash(hash_table_chain_t* ht, char key) {
    double a = GOLDEN_RATIO;
    double val = fabs((int)key * a);
    val = val - (int)val;
    return (int)(ht->capacity * val);
}

int hash_chain_function(hash_table_chain_t* ht, char key) {
    switch (ht->hash_func_type) {
        case HASH_CHAIN_DIVISION:
            return hash_chain_division_hash(ht, key);
        case HASH_CHAIN_MULTIPLICATION:
            return hash_chain_multiplication_hash(ht, key);
        default:
            return hash_chain_division_hash(ht, key);
    }
}

hash_chain_node_t* hash_chain_create_node(char key, int value) {
    hash_chain_node_t* new_node = (hash_chain_node_t*)malloc(sizeof(hash_chain_node_t));
    if (!new_node) return NULL;
    
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    
    return new_node;
}

bool hash_table_chain_insert(hash_table_chain_t* ht, char key, int value) {
    int bucket_index = hash_chain_function(ht, key);
    
    hash_chain_node_t* current = ht->buckets[bucket_index];
    while (current) {
        if (current->key == key) {
            current->value += value;
            return true;
        }
        current = current->next;
    }
    
    bool primary_collision = (ht->buckets[bucket_index] != NULL);
    
    hash_chain_node_t* new_node = hash_chain_create_node(key, value);
    if (!new_node) return false;
    
    new_node->next = ht->buckets[bucket_index];
    ht->buckets[bucket_index] = new_node;
    ht->size++;
    
    if (primary_collision) {
        ht->collisions++;
    }
    
    ht->load_factor = (double)ht->size / ht->capacity;
    
    return true;
}

bool hash_table_chain_insert_with_count(hash_table_chain_t* ht, char key) {
    int bucket_index = hash_chain_function(ht, key);
    
    hash_chain_node_t* current = ht->buckets[bucket_index];
    while (current) {
        if (current->key == key) {
            current->value++;
            return true;
        }
        current = current->next;
    }
    
    bool primary_collision = (ht->buckets[bucket_index] != NULL);
    
    hash_chain_node_t* new_node = hash_chain_create_node(key, 1);
    if (!new_node) return false;
    
    new_node->next = ht->buckets[bucket_index];
    ht->buckets[bucket_index] = new_node;
    ht->size++;
    
    if (primary_collision) {
        ht->collisions++;
    }
    
    ht->load_factor = (double)ht->size / ht->capacity;
    
    return true;
}

bool hash_table_chain_remove(hash_table_chain_t* ht, char key) {
    int bucket_index = hash_chain_function(ht, key);
    hash_chain_node_t* current = ht->buckets[bucket_index];
    hash_chain_node_t* prev = NULL;
    
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                ht->buckets[bucket_index] = current->next;
            }
            
            free(current);
            ht->size--;
            ht->load_factor = (double)ht->size / ht->capacity;
            return true;
        }
        
        prev = current;
        current = current->next;
    }
    
    return false;
}

hash_chain_node_t* hash_table_chain_search(hash_table_chain_t* ht, char key) {
    int bucket_index = hash_chain_function(ht, key);
    hash_chain_node_t* current = ht->buckets[bucket_index];
    
    while (current) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

void hash_table_chain_display(hash_table_chain_t* ht) {
    printf("\n=== HASH TABLE (Chaining Method) ===\n");
    printf("Capacity: %d, Size: %d, Load factor: %.2f\n", 
           ht->capacity, ht->size, ht->load_factor);
    printf("Collisions: %d, Hash function: %s\n", 
           ht->collisions,
           ht->hash_func_type == HASH_CHAIN_DIVISION ? "Division" : "Multiplication");
    printf("--------------------------------------\n");
    
    for (int i = 0; i < ht->capacity; i++) {
        printf("[%3d]: ", i);
        
        hash_chain_node_t* current = ht->buckets[i];
        if (current) {
            printf("'%c'->%d", current->key, current->value);
            current = current->next;
            
            while (current) {
                printf(" -> '%c'->%d", current->key, current->value);
                current = current->next;
            }
        } else {
            printf("[EMPTY]");
        }
        printf("\n");
    }
    printf("======================================\n");
}

bool hash_table_chain_needs_resize(hash_table_chain_t* ht) {
    return ht->load_factor > 0.7;
}

bool hash_table_chain_needs_rehash(hash_table_chain_t* ht) {
    return ht->collisions > 3;
}

hash_table_chain_t* hash_table_chain_resize(hash_table_chain_t* ht) {
    int old_capacity = ht->capacity;
    int candidate = (int)(old_capacity * 1.3);
    if (candidate <= old_capacity) {
        candidate = old_capacity * 2;
    }
    int new_capacity = next_prime(candidate);
    
    printf("Resizing chaining hash table: %d -> %d (load factor: %.2f)\n", 
           old_capacity, new_capacity, ht->load_factor);
    
    hash_table_chain_t* new_ht = hash_table_chain_create(new_capacity, ht->hash_func_type);
    if (!new_ht) return ht;
    
    for (int i = 0; i < old_capacity; i++) {
        hash_chain_node_t* current = ht->buckets[i];
        while (current) {
            hash_table_chain_insert(new_ht, current->key, current->value);
            current = current->next;
        }
    }
    
    hash_table_chain_destroy(ht);
    return new_ht;
}

hash_table_chain_t* hash_table_chain_rehash(hash_table_chain_t* ht, hash_chain_function_type_t new_type) {
    printf("Rehashing chaining hash table with new hash function: %s\n",
           new_type == HASH_CHAIN_DIVISION ? "Division" : "Multiplication");
    
    hash_table_chain_t* new_ht = hash_table_chain_create(ht->capacity, new_type);
    if (!new_ht) return ht;
    
    for (int i = 0; i < ht->capacity; i++) {
        hash_chain_node_t* current = ht->buckets[i];
        while (current) {
            hash_table_chain_insert(new_ht, current->key, current->value);
            current = current->next;
        }
    }
    
    hash_table_chain_destroy(ht);
    return new_ht;
}

int hash_table_chain_get_average_comparisons(hash_table_chain_t* ht) {
    if (ht->size == 0) return 0;
    
    int total_comparisons = 0;
    int searches = 0;
    
    for (int i = 0; i < ht->capacity; i++) {
        hash_chain_node_t* current = ht->buckets[i];
        int chain_length = 0;
        
        while (current) {
            chain_length++;
            current = current->next;
        }
        
        if (chain_length > 0) {
            int avg_for_bucket = (chain_length + 1) / 2;
            total_comparisons += avg_for_bucket * chain_length;
            searches += chain_length;
        }
    }
    
    return searches > 0 ? total_comparisons / searches : 0;
}

hash_table_chain_t* hash_table_chain_build_from_string(hash_table_chain_t* ht, const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            if (hash_table_chain_needs_resize(ht)) {
                hash_table_chain_t* new_ht = hash_table_chain_resize(ht);
                ht = new_ht;
            }
            
            if (!hash_table_chain_insert_with_count(ht, str[i])) {
                printf("Failed to insert character '%c'\n", str[i]);
            }
            
            if (hash_table_chain_needs_rehash(ht)) {
                hash_chain_function_type_t new_type = (ht->hash_func_type == HASH_CHAIN_DIVISION) 
                    ? HASH_CHAIN_MULTIPLICATION 
                    : HASH_CHAIN_DIVISION;
                hash_table_chain_t* new_ht = hash_table_chain_rehash(ht, new_type);
                ht = new_ht;
            }
        }
    }
    return ht;
}

hash_table_chain_t* hash_table_chain_create_from_string(const char* str, hash_chain_function_type_t type) {
    hash_table_chain_t* ht = hash_table_chain_create(10, type);
    if (ht) {
        ht = hash_table_chain_build_from_string(ht, str);
    }
    return ht;
}
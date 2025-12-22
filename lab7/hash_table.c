#include "hash_table.h"
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

hash_table_t* hash_table_create(int initial_capacity, hash_function_type_t type) {
    hash_table_t* ht = (hash_table_t*)malloc(sizeof(hash_table_t));
    if (!ht) return NULL;
    
    ht->capacity = initial_capacity;
    ht->size = 0;
    ht->collisions = 0;
    ht->hash_func_type = type;
    ht->load_factor = 0.0;
    
    ht->table = (hash_table_entry_t*)calloc(ht->capacity, sizeof(hash_table_entry_t));
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    
    return ht;
}

void hash_table_destroy(hash_table_t* ht) {
    if (ht) {
        if (ht->table) 
            free(ht->table);
        free(ht);
    }
}

int division_hash(hash_table_t* ht, char key) {
    return abs((int)key) % ht->capacity;
}

int multiplication_hash(hash_table_t* ht, char key) {
    double a = GOLDEN_RATIO;
    double val = fabs((int)key * a);
    val = val - (int)val;
    return (int)(ht->capacity * val);
}

int hash_function(hash_table_t* ht, char key) {
    switch (ht->hash_func_type) {
        case HASH_DIVISION:
            return division_hash(ht, key);
        case HASH_MULTIPLICATION:
            return multiplication_hash(ht, key);
        default:
            return division_hash(ht, key);
    }
}



int linear_probe(hash_table_t* ht, char key, int start_index, bool for_insert, bool* had_collision) {
    int index = start_index;
    int attempts = 0;
    
    *had_collision = false;
    
    while (attempts < ht->capacity) {
        if (!ht->table[index].is_occupied || 
            (for_insert && ht->table[index].is_deleted)) {
            return index;
        }
        
        if (!for_insert && ht->table[index].is_occupied && 
            !ht->table[index].is_deleted && 
            ht->table[index].key == key) {
            return index;
        }
        
        if (for_insert && attempts > 0) {
            *had_collision = true;
        }
        
        index = (index + 1) % ht->capacity;
        attempts++;
    }
    
    return -1;
}

bool hash_table_insert(hash_table_t* ht, char key, int value) {
    if (ht->size >= ht->capacity) {
        return false;
    }
    
    int original_index = hash_function(ht, key);
    bool primary_collision = false;
    
    if (ht->table[original_index].is_occupied && 
        !ht->table[original_index].is_deleted && 
        ht->table[original_index].key != key) {
        primary_collision = true;
    }
    
    bool had_collision = false;
    int probe_index = linear_probe(ht, key, original_index, true, &had_collision);
    
    if (probe_index == -1) {
        return false;
    }
    
    if (!ht->table[probe_index].is_occupied || ht->table[probe_index].is_deleted) {
        ht->table[probe_index].key = key;
        ht->table[probe_index].value = value;
        ht->table[probe_index].is_occupied = true;
        ht->table[probe_index].is_deleted = false;
        ht->size++;
        
        if (primary_collision) {
            ht->collisions++;
        }
        
        ht->load_factor = (double)ht->size / ht->capacity;
        return true;
    }
    
    if (ht->table[probe_index].key == key) {
        ht->table[probe_index].value += value;
        return true;
    }
    
    return false;
}

bool hash_table_insert_with_count(hash_table_t* ht, char key) {
    hash_table_entry_t* entry = hash_table_search(ht, key);
    if (entry) {
        entry->value++;
        return true;
    }
    
    int original_index = hash_function(ht, key);
    bool primary_collision = false;
    
    if (ht->table[original_index].is_occupied && 
        !ht->table[original_index].is_deleted && 
        ht->table[original_index].key != key) {
        primary_collision = true;
    }
    
    bool had_collision = false;
    int probe_index = linear_probe(ht, key, original_index, true, &had_collision);
    
    if (probe_index == -1) {
        return false;
    }
    
    ht->table[probe_index].key = key;
    ht->table[probe_index].value = 1;
    ht->table[probe_index].is_occupied = true;
    ht->table[probe_index].is_deleted = false;
    ht->size++;
    
    if (primary_collision) {
        ht->collisions++;
    }
    
    ht->load_factor = (double)ht->size / ht->capacity;
    return true;
}

hash_table_entry_t* hash_table_search(hash_table_t* ht, char key) {
    int index = hash_function(ht, key);
    bool had_collision;
    int probe_index = linear_probe(ht, key, index, false, &had_collision);
    
    if (probe_index == -1) {
        return NULL;
    }
    
    if (ht->table[probe_index].is_occupied && 
        !ht->table[probe_index].is_deleted && 
        ht->table[probe_index].key == key) {
        return &ht->table[probe_index];
    }
    
    return NULL;
}

bool hash_table_remove(hash_table_t* ht, char key) {
    int index = hash_function(ht, key);
    bool had_collision;
    int probe_index = linear_probe(ht, key, index, false, &had_collision);
    
    if (probe_index == -1) {
        return false;
    }
    
    if (ht->table[probe_index].is_occupied && 
        !ht->table[probe_index].is_deleted && 
        ht->table[probe_index].key == key) {
        ht->table[probe_index].is_deleted = true;
        ht->size--;
        ht->load_factor = (double)ht->size / ht->capacity;
        return true;
    }
    
    return false;
}

void hash_table_display(hash_table_t* ht) {
    printf("\n=== HASH TABLE (Open Addressing) ===\n");
    printf("Capacity: %d, Size: %d, Load factor: %.2f\n", 
           ht->capacity, ht->size, ht->load_factor);
    printf("Collisions: %d, Hash function: %s\n", 
           ht->collisions,
           ht->hash_func_type == HASH_DIVISION ? "Division" : "Multiplication");
    printf("------------------------------------\n");
    
    for (int i = 0; i < ht->capacity; i++) {
        printf("[%3d]: ", i);
        
        if (ht->table[i].is_occupied && !ht->table[i].is_deleted) {
            printf("'%c' -> %d", ht->table[i].key, ht->table[i].value);
        } else if (ht->table[i].is_deleted) {
            printf("[DELETED]");
        } else {
            printf("[EMPTY]");
        }
        
        if (ht->table[i].is_occupied && !ht->table[i].is_deleted) {
            int original_hash = hash_function(ht, ht->table[i].key);
            if (original_hash != i) {
                printf(" (hash: %d)", original_hash);
            }
        }
        
        printf("\n");
    }
    printf("====================================\n");
}

bool hash_table_needs_resize(hash_table_t* ht) {
    return ht->load_factor > 0.7;
}

bool hash_table_needs_rehash(hash_table_t* ht) {
    return ht->collisions > 3;
}

hash_table_t* hash_table_resize(hash_table_t* ht) {
    int old_capacity = ht->capacity;
    int candidate = (int)(old_capacity * 1.3);
    if (candidate <= old_capacity) {
        candidate = old_capacity * 2;
    }
    int new_capacity = next_prime(candidate);
    
    printf("Resizing hash table: %d -> %d (load factor: %.2f)\n", 
           old_capacity, new_capacity, ht->load_factor);
    
    hash_table_t* new_ht = hash_table_create(new_capacity, ht->hash_func_type);
    if (!new_ht) return ht;
    
    for (int i = 0; i < old_capacity; i++) {
        if (ht->table[i].is_occupied && !ht->table[i].is_deleted) {
            hash_table_insert(new_ht, ht->table[i].key, ht->table[i].value);
        }
    }
    
    hash_table_destroy(ht);
    return new_ht;
}

hash_table_t* hash_table_rehash(hash_table_t* ht, hash_function_type_t new_type) {
    printf("Rehashing with new hash function: %s\n",
           new_type == HASH_DIVISION ? "Division" : "Multiplication");
    
    hash_table_t* new_ht = hash_table_create(ht->capacity, new_type);
    if (!new_ht) return ht;
    
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->table[i].is_occupied && !ht->table[i].is_deleted) {
            hash_table_insert(new_ht, ht->table[i].key, ht->table[i].value);
        }
    }
    
    hash_table_destroy(ht);
    return new_ht;
}

int hash_table_get_average_comparisons(hash_table_t* ht) {
    if (ht->size == 0) return 0;
    
    int total_comparisons = 0;
    int searches = 0;
    
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->table[i].is_occupied && !ht->table[i].is_deleted) {
            char key = ht->table[i].key;
            int index = hash_function(ht, key);
            int comparisons = 1;
            
            while (ht->table[index].key != key) {
                index = (index + 1) % ht->capacity;
                comparisons++;
            }
            
            total_comparisons += comparisons;
            searches++;
        }
    }
    
    return searches > 0 ? total_comparisons / searches : 0;
}

hash_table_t* hash_table_build_from_string(hash_table_t* ht, const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            if (hash_table_needs_resize(ht)) {
                hash_table_t* new_ht = hash_table_resize(ht);
                ht = new_ht;
            }
            
            if (!hash_table_insert_with_count(ht, str[i])) {
                printf("Failed to insert character '%c'\n", str[i]);
            }
            
            if (hash_table_needs_rehash(ht)) {
                hash_function_type_t new_type = (ht->hash_func_type == HASH_DIVISION) 
                    ? HASH_MULTIPLICATION 
                    : HASH_DIVISION;
                hash_table_t* new_ht = hash_table_rehash(ht, new_type);
                ht = new_ht;
            }
        }
    }
    return ht;
}

hash_table_t* hash_table_create_from_string(const char* str, hash_function_type_t type) {
    hash_table_t* ht = hash_table_create(10, type);
    if (ht)
        ht = hash_table_build_from_string(ht, str);
    return ht;
}
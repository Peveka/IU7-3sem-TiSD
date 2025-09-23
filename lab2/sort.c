#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sort.h"
#include "struct.h"
#define ITERATIONS 10


// ===== SWAP FUNCTION =====
static void swap(void *a, void *b, void *temp, size_t size) {
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

// ===== UNIVERSAL BUBBLE SORT =====
void bubble_sort(void *base, int nmemb, int size, int (*compar)(const void*, const void*)) {
    if (nmemb <= 1) return;
    
    char *array = (char*)base;
    char temp[size];
    
    for (int i = 0; i < nmemb - 1; i++) {
        for (int j = 0; j < nmemb - i - 1; j++) {
            void *elem1 = array + j * size;
            void *elem2 = array + (j + 1) * size;
            
            if (compar(elem1, elem2) > 0) {
                swap(elem1, elem2, temp, size);
            }
        }
    }
}

// ===== COMPARISON FUNCTIONS =====
int compare_surname(const void *a, const void *b) {
    const subscriber_t *sub1 = (const subscriber_t*)a;
    const subscriber_t *sub2 = (const subscriber_t*)b;
    return strcmp(sub1->surname, sub2->surname);
}

int compare_keys(const void *a, const void *b) {
    const key_table_t *key1 = (const key_table_t*)a;
    const key_table_t *key2 = (const key_table_t*)b;
    return strcmp(key1->surname, key2->surname);
}

// ===== CREATE KEY TABLE =====
void create_key_table(subscriber_t *table, key_table_t *keys, int len) {
    for (int i = 0; i < len; i++) {
        keys[i].index = i;
        strcpy(keys[i].surname, table[i].surname);
    }
}

// ===== PRINT VIA KEYS =====
void print_via_keys(subscriber_t *table, key_table_t *keys, int len) {
    if (len == 0) {
        printf("Table is empty!\n");
        return;
    }
    
    printf("\nTable sorted by surname (showing first 20 records):\n");
    printf("Index   Surname          Name             Phone        Status\n");
    printf("-------------------------------------------------------------\n");
    
    int show_count = (len > 20) ? 20 : len;
    for (int i = 0; i < show_count; i++) {
        int idx = keys[i].index;
        const char *status = (table[idx].status == FRIEND) ? "Friend" : "Colleague";
        printf("%-7d %-16s %-16s %-12s %-10s\n", 
               idx, table[idx].surname, table[idx].name, table[idx].phone, status);
    }
}

// ===== CORRECTED PERFORMANCE MEASUREMENT =====
void measure_sorts(subscriber_t *table, int len) {
    if (len == 0) {
        printf("Table is empty!\n");
        return;
    }
    
    clock_t start, end;
    
    printf("Measuring sort performance for %d records (%d iterations)...\n", len, ITERATIONS);
    
    // Выделяем память для копий
    subscriber_t *table_copy1 = malloc(len * sizeof(subscriber_t));
    subscriber_t *table_copy2 = malloc(len * sizeof(subscriber_t));
    key_table_t *keys1 = malloc(len * sizeof(key_table_t));
    key_table_t *keys2 = malloc(len * sizeof(key_table_t));
    
    if (!table_copy1 || !table_copy2 || !keys1 || !keys2) {
        printf("Memory allocation failed!\n");
        free(table_copy1);
        free(table_copy2);
        free(keys1);
        free(keys2);
        return;
    }
    
    double total_table_bubble = 0, total_table_qsort = 0;
    double total_keys_bubble = 0, total_keys_qsort = 0;
    
    for (int iter = 0; iter < ITERATIONS; iter++) {
        memcpy(table_copy1, table, len * sizeof(subscriber_t));
        memcpy(table_copy2, table, len * sizeof(subscriber_t));
        create_key_table(table, keys1, len);
        create_key_table(table, keys2, len);
        
        // 1. Table bubble sort
        start = clock();
        bubble_sort(table_copy1, len, sizeof(subscriber_t), compare_surname);
        end = clock();
        total_table_bubble += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        // 2. Table qsort
        start = clock();
        qsort(table_copy2, len, sizeof(subscriber_t), compare_surname);
        end = clock();
        total_table_qsort += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        // 3. Keys bubble sort
        start = clock();
        bubble_sort(keys1, len, sizeof(key_table_t), compare_keys);
        end = clock();
        total_keys_bubble += ((double)(end - start)) / CLOCKS_PER_SEC;
        
        // 4. Keys qsort
        start = clock();
        qsort(keys2, len, sizeof(key_table_t), compare_keys);
        end = clock();
        total_keys_qsort += ((double)(end - start)) / CLOCKS_PER_SEC;
    }
    
    // Вычисляем среднее время
    double time_table_bubble = total_table_bubble / ITERATIONS;
    double time_table_qsort = total_table_qsort / ITERATIONS;
    double time_keys_bubble = total_keys_bubble / ITERATIONS;
    double time_keys_qsort = total_keys_qsort / ITERATIONS;
    
    // рассчитываем эффективность
    double speedup_table_qsort = time_table_bubble / time_table_qsort;
    double speedup_keys_bubble = time_table_bubble / time_keys_bubble;
    double speedup_keys_qsort = time_table_bubble / time_keys_qsort;
    
    // Output results
    printf("\nSORT PERFORMANCE COMPARISON (%d iterations average)\n", ITERATIONS);
    printf("==================================================\n");
    printf("Sort Method               Time (sec)    Speedup Factor\n");
    printf("------------------------- ------------- --------------\n");
    printf("Table (bubble sort)       %13.6f     1.00x (base)\n", time_table_bubble);
    printf("Table (qsort)             %13.6f     %.2fx\n", 
           time_table_qsort, speedup_table_qsort);
    printf("Keys (bubble sort)        %13.6f     %.2fx\n", 
           time_keys_bubble, speedup_keys_bubble);
    printf("Keys (qsort)              %13.6f     %.2fx\n", 
           time_keys_qsort, speedup_keys_qsort);
    printf("------------------------- ------------- --------------\n");
    
    printf("\nPercentage improvement compared to base:\n");
    printf("Table (qsort):   %.1f%% faster\n", (speedup_table_qsort - 1) * 100);
    printf("Keys (bubble):   %.1f%% faster\n", (speedup_keys_bubble - 1) * 100);
    printf("Keys (qsort):    %.1f%% faster\n", (speedup_keys_qsort - 1) * 100);
    
    // Memory usage comparison
    size_t table_size = len * sizeof(subscriber_t);
    size_t keys_size = len * sizeof(key_table_t);
    printf("\nMemory usage comparison:\n");
    printf("- Table size: %zu bytes (%d records x %zu bytes each)\n", 
           table_size, len, sizeof(subscriber_t));
    printf("- Keys size:  %zu bytes (%d records x %zu bytes each)\n", 
           keys_size, len, sizeof(key_table_t));
    printf("- Keys use %.1f%% less memory than table\n", 
           (1.0 - (double)keys_size/table_size) * 100);
    
    free(table_copy1);
    free(table_copy2);
    free(keys1);
    free(keys2);
}
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sort.h"

void swap(void *a, void *b, void *temp, size_t size) {
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

void bubble_sort(void *base, int nmemb, int size, int (*compar)(const void*, const void*)) {
    if (nmemb <= 1) 
        return; //Empty arr don't sort
    
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

void create_key_table(subscriber_t *table, key_table_t *keys, int len) {
    for (int i = 0; i < len; i++) {
        keys[i].index = i;
        strcpy(keys[i].surname, table[i].surname);
    }
}

void print_via_keys(subscriber_t *table, key_table_t *keys, int len) {
    if (len == 0) {
        printf("Table is empty!\n");
        return;
    }
    
    printf("\nTable using keys:\n");
    for (int i = 0; i < len; i++) {
        int idx = keys[i].index;
        printf("%s %s %s\n", table[idx].surname, table[idx].name, table[idx].phone);
    }
}

void measure_sorts(subscriber_t *table, int len) {
    if (len == 0) {
        printf("Table is empty!\n");
        return;
    }
    
    clock_t start, end;
    subscriber_t table_copy1[MAX_FILE_LEN], table_copy2[MAX_FILE_LEN];
    key_table_t keys1[MAX_FILE_LEN], keys2[MAX_FILE_LEN];
    
    memcpy(table_copy1, table, len * sizeof(subscriber_t));
    memcpy(table_copy2, table, len * sizeof(subscriber_t));
    
    printf("Starting compare \n");
    
    // 1. Таблица bubble sort
    start = clock();
    bubble_sort(table_copy1, len, sizeof(subscriber_t), compare_surname);
    end = clock();
    double time_table_bubble = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 2. Таблица qsort
    start = clock();
    qsort(table_copy2, len, sizeof(subscriber_t), compare_surname);
    end = clock();
    double time_table_qsort = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 3. Ключи bubble sort
    create_key_table(table, keys1, len);
    start = clock();
    bubble_sort(keys1, len, sizeof(key_table_t), compare_keys);
    end = clock();
    double time_keys_bubble = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 4. Ключи qsort
    create_key_table(table, keys2, len);
    start = clock();
    qsort(keys2, len, sizeof(key_table_t), compare_keys);
    end = clock();
    double time_keys_qsort = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Вывод результатов
    printf("\nРезультаты для %d записей:\n", len);
    printf("Таблица (bubble): %.6f сек\n", time_table_bubble);
    printf("Таблица (qsort):  %.6f сек\n", time_table_qsort);
    printf("Ключи (bubble):   %.6f сек\n", time_keys_bubble);
    printf("Ключи (qsort):    %.6f сек\n", time_keys_qsort);
    
    printf("\nЭффективность относительно таблицы bubble sort:\n");
    printf("Таблица (qsort):  %.2f%%\n", (time_table_bubble - time_table_qsort) / time_table_bubble * 100);
    printf("Ключи (bubble):   %.2f%%\n", (time_table_bubble - time_keys_bubble) / time_table_bubble * 100);
    printf("Ключи (qsort):    %.2f%%\n", (time_table_bubble - time_keys_qsort) / time_table_bubble * 100);
}

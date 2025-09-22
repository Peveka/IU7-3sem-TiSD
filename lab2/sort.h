#ifndef SORT_H__
#define SORT_H__

#include "struct.h"

void bubble_sort(void *base, int nmemb, int size, int (*compar)(const void*, const void*));

int compare_surname(const void *a, const void *b);
int compare_keys(const void *a, const void *b);

void create_key_table(subscriber_t *table, key_table_t *keys, int len);

void print_via_keys(subscriber_t *table, key_table_t *keys, int len);

void measure_sorts(subscriber_t *table, int len);

#endif

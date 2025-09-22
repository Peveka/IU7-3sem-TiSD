#ifndef MENU_H__
#define MENU_H__
#include <stdio.h>
#include "struct.h"

typedef enum
{
    NO_MODE = 0,
    FILE_INPUT = 1,
    STDIN_INPUT = 2,
    FILE_OUTPUT = 3, 
    STDIN_OUTPUT = 4,
    SEARCH_DATA = 5,
    SORT_TABLE_BUBBLE = 6,
    SORT_TABLE_QSORT = 7,
    SORT_KEYS_BUBBLE = 8,
    SORT_KEYS_QSORT = 9,
    COMPARE_SORTS = 10,
    EXIT = 11
} mode_t;

int menu_handler(subscriber_t *subscribers);

#endif

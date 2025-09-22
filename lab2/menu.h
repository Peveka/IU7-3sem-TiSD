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
    EXIT = 9
} mode_t;

int menu_handler(subscriber_t *subscribers);

#endif
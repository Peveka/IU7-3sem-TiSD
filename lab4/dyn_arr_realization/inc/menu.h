#ifndef MENU_H__
#define MENU_H__
#include "errors.h"
#include "data.h"

typedef enum
{
    no_mode = 0,
    add_mode = 1,
    pop_mode = 2,
    peek_mode = 3,
    stack_status_mode = 4,
    palindrome_mode = 5,
    clear_mode = 6,
    exit_mode = 7
} user_mode_t;

void menu_handler(stack_t *stack);
void clear_input(void);

#endif
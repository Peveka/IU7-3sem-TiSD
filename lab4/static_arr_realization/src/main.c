#include <stdio.h>
#include "data.h"
#include "menu.h"

int main(void)
{
    stack_t stack = {.max_size = MAX_ARR_LEN, .size = 0};
    menu_handler(&stack);
    
    return 0;
}
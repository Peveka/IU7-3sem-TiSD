#include <stdio.h>
#include "list.h"
#include "menu.h"

int main(void)
{
    stack_list_t stack;
    init_stack(&stack);

    menu_handler(&stack);

    free_stack(&stack);
    
    return 0;
}
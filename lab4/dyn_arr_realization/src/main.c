#include <stdio.h>
#include "data.h"
#include "menu.h"

int main(void)
{
    stack_t stack;
    
    if (stack_init(&stack) != OK) 
    {
        printf("Error: Could not initialize stack!\n");
        return 1;
    }
    
    printf("Dynamic Stack Program Started\n");
    menu_handler(&stack);
    
    stack_free(&stack);
    printf("Program finished. Memory freed.\n");
    
    return 0;
}
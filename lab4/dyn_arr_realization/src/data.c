#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "errors.h"

error_t stack_init(stack_t *stack)
{
    stack->data = (stack_elem_t*)malloc(INITIAL_CAPACITY * sizeof(stack_elem_t));
    if (stack->data == NULL)
        return ERR_MEMORY;
    
    stack->capacity = INITIAL_CAPACITY;
    stack->size = 0;
    return OK;
}

void stack_free(stack_t *stack)
{
    if (stack->data != NULL)
    {
        free(stack->data);
        stack->data = NULL;
    }
    stack->capacity = 0;
    stack->size = 0;
}

error_t stack_resize(stack_t *stack, int new_capacity)
{
    stack_elem_t *new_data = (stack_elem_t*)realloc(stack->data, new_capacity * sizeof(stack_elem_t));
    if (new_data == NULL)
        return ERR_MEMORY;
    
    stack->data = new_data;
    stack->capacity = new_capacity;
    return OK;
}
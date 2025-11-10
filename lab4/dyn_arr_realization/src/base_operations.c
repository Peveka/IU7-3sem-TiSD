#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "errors.h"

error_t add_elem_to_stack(stack_t *stack, char data)
{
    if (stack->size >= stack->capacity)
    {
        error_t rc = stack_resize(stack, stack->capacity * GROWTH_FACTOR);
        if (rc != OK)
            return rc;
    }
    
    for (int i = stack->size; i > 0; --i)
        stack->data[i] = stack->data[i-1];

    //stack->data[0].data = NULL;
    stack->data[0].letter = data;
    stack->size++;
    
    return OK;
}

static void elem_cpy(stack_elem_t *dst, stack_elem_t *src)
{
    //dst->data = src->data;
    dst->letter = src->letter;
}

void print_elem(stack_elem_t elem)
{
    printf("letter (char): %c\n", elem.letter);
}

error_t pop_elem_from_stack(stack_t *stack, stack_elem_t *pop_elem)
{
    if (stack->size == 0)
        return ERR_STACK_EMPTY;
    
    elem_cpy(pop_elem, &stack->data[0]);
    
    for (int i = 1; i < stack->size; ++i)
        stack->data[i-1] = stack->data[i];
    
    stack->size--;
    
    // Опционально: уменьшаем capacity если слишком много свободного места
    if (stack->size > 0 && stack->size < stack->capacity / (GROWTH_FACTOR * 2))
    {
        stack_resize(stack, stack->capacity / GROWTH_FACTOR);
    }
    
    return OK;
}

void stack_status(stack_t *stack)
{
    if (stack->size == 0)
        printf("Stack is empty!\n");
    else 
        printf("Stack has %d elements (capacity: %d)\n", stack->size, stack->capacity);
    
    if (stack->size != 0)
    {
        for (int i = 0; i < stack->size; ++i)
        {
            printf("Elem number %d: ", i+1);
            print_elem(stack->data[i]);
        }
    }
}

void clear_all_stack(stack_t *stack)
{
    stack->size = 0;
    stack_resize(stack, INITIAL_CAPACITY);
}
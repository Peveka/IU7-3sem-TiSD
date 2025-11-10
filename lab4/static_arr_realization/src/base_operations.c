#include <stdio.h>
#include "data.h"
#include "errors.h"

void add_elem_to_stack(stack_t *stack, char data)
{
    if (stack->size < stack->max_size)
    {
        for (int i = stack->size; i >= 0; --i)
            (stack->data)[i+1] = (stack->data)[i];

        //stack->data[0].data = NULL;
        stack->data[0].letter = data;
        (stack->size)++;
    }
    else
        printf("Stack is full!\n");
}

static void elem_cpy(stack_elem_t *src, stack_elem_t *dst)
{
    //src->data = dst->data;
    src->letter = dst->letter;
}

void print_elem(stack_elem_t elem)
{
    printf("letter (char): %c\n", elem.letter);
}

void pop_elem_from_stack(stack_t *stack, stack_elem_t *pop_elem)
{
    if (stack->size != 0)
    {
        elem_cpy(pop_elem, &(stack->data)[0]);
        for(int i = 1; i < stack->size; ++i)
            (stack->data)[i-1] = (stack->data)[i];
        (stack->size)--;
    }
}

void stack_status(stack_t *stack)
{
    if (stack->size == 0)
        printf("Stack is empty!\n");
    else if (stack->size == stack->max_size)
        printf("Stack is full!\n");
    else 
        printf("Stack has only %d elem from max %d\n", stack->size, stack->max_size);
    if (stack->size != 0)
        for (int i = 0; i < stack->size; ++i)
        {
            printf("Elem number: %d\n", i+1);
            print_elem((stack->data)[i]);
        }
}

void clear_all_stack(stack_t *stack)
{
    for(int i = 0; i < (stack->size)+1; ++i)
    {
        stack_elem_t elem;
        pop_elem_from_stack(stack, &elem);
    }

    stack->size = 0;
}
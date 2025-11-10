#ifndef DATA_H__
#define DATA_H__
#include "errors.h"
#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

typedef struct stack_elem_t
{
    char letter;
} stack_elem_t;

typedef struct stack_t
{
    stack_elem_t *data;        
    int capacity;              
    int size;                  
} stack_t;

error_t stack_init(stack_t *stack);
void stack_free(stack_t *stack);
error_t stack_resize(stack_t *stack, int new_capacity);

#endif
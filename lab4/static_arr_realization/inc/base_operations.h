#ifndef OPER_H__
#define OPER_H__
#include "errors.h"
#include "data.h"

void add_elem_to_stack(stack_t *stack, char data);
void pop_elem_from_stack(stack_t *stack, stack_elem_t *pop_elem);
void stack_status(stack_t *stack);
void print_elem(stack_elem_t elem);
void clear_all_stack(stack_t *stack);
#endif
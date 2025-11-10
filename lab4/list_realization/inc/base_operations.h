#ifndef BASE_OP_H__
#define BASE_OP_H__
#include "list.h"
void peek_stack_elem(stack_list_t *stack);
void stack_status(stack_list_t *stack);
error_t add_elem_to_stack(stack_list_t *stack, char data);
list_node_t *pop_elem_from_stack(stack_list_t *stack);
//void check_mem_fragmentation(stack_list_t *stack);

#endif
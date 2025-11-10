#include <stdlib.h>
#include "list.h"

void init_stack(stack_list_t *stack)
{
    stack->size = 0;
    stack->head = NULL;
}

void free_stack(stack_list_t *stack)
{
    list_node_t *cur_elem = stack->head;
    list_node_t *next_elem = NULL;
    
    while (cur_elem != NULL)
    {
        next_elem = cur_elem->next;
        free(cur_elem);
        cur_elem = next_elem;
    }

    stack->head = NULL;
    stack->size = 0;
}
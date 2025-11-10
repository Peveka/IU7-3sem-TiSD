#ifndef LIST_H__
#define LIST_H__
#include "errors.h" 

typedef struct list_node
{
    struct list_node *next;
    char data;
} list_node_t;

typedef struct 
{
    list_node_t *head;
    int size;
} stack_list_t;

void init_stack(stack_list_t *stack);
void free_stack(stack_list_t *stack);

#endif
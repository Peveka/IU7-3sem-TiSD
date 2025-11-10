#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "errors.h"

static list_node_t *allocate_stack_elem(void)
{
    list_node_t *elem = malloc(sizeof(list_node_t));
    return elem;
}

void peek_stack_elem(stack_list_t *stack)
{
    if (stack->size != 0)
        printf("Peek stack elem data: %c address: %p\n", stack->head->data, (void*)stack->head);
    else
        printf("Stack is empty\n");
}

// void check_mem_fragmentation(stack_list_t *stack)
// {
//     if (stack->size < 2) {
//         printf("Too few elements to check fragmentation\n");
//         return;
//     }
    
//     list_node_t *cur = stack->head;
//     int sequential_blocks = 0;
//     int total_gaps = 0;
//     size_t total_gap_size = 0;
    
//     while (cur != NULL && cur->next != NULL)
//     {
//         size_t actual_gap = (size_t)cur->next - (size_t)cur;
//         size_t expected_gap = sizeof(list_node_t);
        
//         if (actual_gap == expected_gap) {
//             sequential_blocks++;
//         } else {
//             total_gaps++;
//             total_gap_size += (actual_gap > expected_gap) ? 
//                              actual_gap - expected_gap : 0;
//         }
        
//         cur = cur->next;
//     }
    
//     double fragmentation_ratio = (double)sequential_blocks / (stack->size - 1);
    
//     printf("Memory layout analysis:\n");
//     printf("  Total elements: %d\n", stack->size);
//     printf("  Sequential blocks: %d/%d (%.1f%%)\n", 
//            sequential_blocks, stack->size - 1, fragmentation_ratio * 100);
//     printf("  Memory gaps: %d\n", total_gaps);
//     printf("  Estimated wasted space: %zu bytes\n", total_gap_size);
    
//     if (fragmentation_ratio > 0.8) {
//         printf("  Status: Good memory locality\n");
//     } else if (fragmentation_ratio > 0.5) {
//         printf("  Status: Moderate fragmentation\n");
//     } else {
//         printf("  Status: High fragmentation\n");
//     }
// }


void stack_status(stack_list_t *stack)
{
    if (stack->size == 0)
        printf("Stack empty\n");
    else
    {
        printf("Stack has %d elems\n", stack->size);
        list_node_t *cur_elem = stack->head;
        for(int i = 0; i < stack->size && cur_elem != NULL; ++i)
        {
            printf("[%d] data: %c address: %p\n", i+1, cur_elem->data, (void*)cur_elem);
            cur_elem = cur_elem->next;
        }
    }
}

error_t add_elem_to_stack(stack_list_t *stack, char data)
{
    list_node_t *elem = allocate_stack_elem();
    if (elem == NULL)
        return ERR_MEMORY;

    elem->data = data;
    elem->next = stack->head;

    stack->head = elem;
    (stack->size)++;
    return OK;
}

list_node_t *pop_elem_from_stack(stack_list_t *stack)
{
    if (stack->size == 0 || stack->head == NULL)
    {
        printf("Stack is empty, there is no elems to pop!");
        return NULL;
    }
    
    list_node_t *head = stack->head;
    stack->head = head->next;
    stack->size--;
    head->next = NULL;
    
    return head;
}
#include "queue_list.h"
void *free_list[MAX_ADDR_COUNT];
int free_list_count = 0; 

void add_to_free_list(void *addr)
{
    if (free_list_count < MAX_ADDR_COUNT)
    {
        free_list[free_list_count] = addr;
        free_list_count++;
    }
}

void print_free_list()
{
    printf("\n--- Список освобожденных адресов (для анализа фрагментации) ---\n");
    if (free_list_count == 0)
    {
        printf("Список пуст.\n");
        return;
    }

    for (int i = 0; i < free_list_count; i++)
    {
        printf("Addr[%d]: %p\n", i, free_list[i]);
    }
    printf("------------------------------------------------------------\n");
}

void create_queue(queue_t *q)
{
    q->pin = NULL;
    q->pout = NULL;
    q->count = 0;
}

int is_empty(const queue_t *q)
{
    return q->pout == NULL; 
}

int enqueue(queue_t *q, request_t req)
{
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    
    if (new_node == NULL)
        return -1; 

    new_node->data = req;
    new_node->next = NULL; 

    printf("[ENQUEUE] ADDED request ID %d at address: %p\n", req.id, new_node);
    
    if (is_empty(q))
    {
        q->pout = new_node;
        q->pin = new_node;
    }
    else
    {
        q->pin->next = new_node;
        q->pin = new_node;
    }
    
    q->count++;
    return 0;
}

int dequeue(queue_t *q, request_t *req)
{
    if (is_empty(q))
        return -1; 

    node_t *temp = q->pout;
    *req = temp->data;

    q->pout = q->pout->next; 

    if (q->pout == NULL)
    {
        q->pin = NULL;
    }

    printf("[DEQUEUE] REMOVED request ID %d from address: %p\n", req->id, temp);
    add_to_free_list(temp);

    free(temp); 
    
    q->count--;
    return 0;
}

void print_queue_list(const queue_t *q)
{
    printf("\n--- Состояние очереди (Список) ---\n");
    if (is_empty(q))
    {
        printf("Очередь пуста. (Count: 0)\n");
        return;
    }

    node_t *current = q->pout;
    printf("Голова (Pout): %p, Хвост (Pin): %p, Длина: %d\n", 
           (void*)q->pout, (void*)q->pin, q->count);
    
    printf("Элементы: ");
    while (current != NULL)
    {
        printf("[ID:%d, C:%d, Addr:%p] -> ", current->data.id, current->data.cycles, (void*)current);
        current = current->next;
    }
    printf("NULL\n");
}
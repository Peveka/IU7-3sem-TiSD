#include "queue_list.h"
void *free_list[MAX_ADDR_COUNT];
int free_list_count = 0; 
void *alloc_list[MAX_ADDR_COUNT];
int alloc_list_count = 0;

void add_to_free_list(void *addr)
{
    if (free_list_count < MAX_ADDR_COUNT)
    {
        free_list[free_list_count] = addr;
        free_list_count++;
    }
}

void add_to_alloc_list(void *addr)
{
    if (alloc_list_count < MAX_ADDR_COUNT)
    {
        alloc_list[alloc_list_count] = addr;
        alloc_list_count++;
    }
}

void clear_alloc_list()
{
    alloc_list_count = 0;
}

void print_free_list()
{
    printf("\n--- Список освобожденных адресов ---\n");
    if (free_list_count == 0)
    {
        printf("Список пуст.\n");
        return;
    }

    printf("Количество освобожденных адресов: %d\n", free_list_count);
    for (int i = 0; i < free_list_count && i < 20; i++) // Ограничим вывод
    {
        printf("Addr[%d]: %p\n", i, free_list[i]);
    }
    if (free_list_count > 20) {
        printf("... и еще %d адресов\n", free_list_count - 20);
    }
    printf("------------------------------------------------------------\n");
}

void print_alloc_list()
{
    printf("\n--- Список выделенных адресов ---\n");
    if (alloc_list_count == 0)
    {
        printf("Список пуст.\n");
        return;
    }

    printf("Количество выделенных адресов: %d\n", alloc_list_count);
    for (int i = 0; i < alloc_list_count && i < 20; i++) // Ограничим вывод
    {
        printf("Addr[%d]: %p\n", i, alloc_list[i]);
    }
    if (alloc_list_count > 20) {
        printf("... и еще %d адресов\n", alloc_list_count - 20);
    }
    
    // Проверка на фрагментацию
    int fragmented = 0;
    for (int i = 1; i < alloc_list_count; i++) {
        if (alloc_list[i] < alloc_list[i-1]) {
            fragmented = 1;
            break;
        }
    }
    printf("Фрагментация памяти: %s\n", fragmented ? "ДА" : "НЕТ");
    printf("------------------------------------------------------------\n");
}

void create_queue(queue_t *q)
{
    q->pin = NULL;
    q->pout = NULL;
    q->count = 0;
    q->memory_used = 0;
}

int is_empty(const queue_t *q)
{
    return q->pout == NULL; 
}

int enqueue(queue_t *q, request_t req, int verbose)
{
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    
    if (new_node == NULL)
        return -1; 

    new_node->data = req;
    new_node->next = NULL; 
    
    // Обновляем использование памяти
    q->memory_used += sizeof(node_t);
    
    if (verbose) {
        printf("[ENQUEUE] ADDED request ID %d at address: %p\n", req.id, new_node);
    }
    add_to_alloc_list(new_node);
    
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

int dequeue(queue_t *q, request_t *req, int verbose)
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

    if (verbose) {
        printf("[DEQUEUE] REMOVED request ID %d from address: %p\n", req->id, temp);
    }
    add_to_free_list(temp);

    // Обновляем использование памяти
    q->memory_used -= sizeof(node_t);
    
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

size_t get_queue_memory(const queue_t *q)
{
    return q->memory_used;
}
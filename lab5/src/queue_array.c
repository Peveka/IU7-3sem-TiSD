#include "queue_array.h"

void create_queue_array(array_queue_t *q)
{
    q->pin = 0; 
    q->pout = 0;
    q->count = 0;
    q->memory_used = sizeof(array_queue_t);
}

int is_empty_array(const array_queue_t *q)
{
    return q->count == 0;
}

int is_full_array(const array_queue_t *q)
{
    return q->count == MAX_QUEUE_SIZE;
}

int enqueue_array(array_queue_t *q, request_t req, int verbose)
{
    if (is_full_array(q))
    {
        if (verbose) {
            printf("[ENQUEUE_ARRAY] Ошибка: Очередь переполнена (Max Size: %d)\n", MAX_QUEUE_SIZE);
        }
        return -1; 
    }

    q->array[q->pin] = req;
    q->pin = (q->pin + 1) % MAX_QUEUE_SIZE;
    
    q->count++;
    if (verbose) {
        printf("[ENQUEUE_ARRAY] ADDED request ID %d at index: %d\n", req.id, (q->pin - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE);
    }
    
    return 0;
}

int dequeue_array(array_queue_t *q, request_t *req, int verbose)
{
    if (is_empty_array(q))
    {
        if (verbose) {
            printf("[DEQUEUE_ARRAY] Ошибка: Очередь пуста.\n");
        }
        return -1;
    }

    *req = q->array[q->pout];
    q->pout = (q->pout + 1) % MAX_QUEUE_SIZE;
    q->count--;
    
    if (verbose) {
        printf("[DEQUEUE_ARRAY] REMOVED request ID %d from index: %d\n", req->id, (q->pout - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE);
    }

    return 0;
}

void print_queue_array(const array_queue_t *q)
{
    printf("\n--- Состояние очереди (Массив) ---\n");
    if (is_empty_array(q))
    {
        printf("Очередь пуста. (Count: 0)\n");
        return;
    }

    printf("Pout: %d, Pin: %d, Длина: %d, Max Size: %d\n", 
           q->pout, q->pin, q->count, MAX_QUEUE_SIZE);
    
    printf("Элементы: ");
    int i = q->pout;
    int printed = 0;
    while (printed < q->count)
    {
        printf("[ID:%d, C:%d, Index:%d] -> ", q->array[i].id, q->array[i].cycles, i);
        i = (i + 1) % MAX_QUEUE_SIZE;
        printed++;
    }
    printf("NULL\n");
}

size_t get_array_queue_memory(const array_queue_t *q)
{
    return q->memory_used;
}
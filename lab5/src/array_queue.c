#include "array_queue.h"

void initialize_circular_buffer(circular_buffer_queue_t *queue)
{
    for (int i = 0; i < MAX_QUEUE_CAPACITY; i++)
    {
        queue->data[i] = 0;
    }
    queue->size = 0;
    queue->front = queue->data;
    queue->rear = queue->data;
}

int enqueue_circular_buffer(circular_buffer_queue_t *queue, int value)
{
    if (queue->size >= MAX_QUEUE_CAPACITY)
    {
        return QUEUE_OVERFLOW;
    }

    if (queue->rear >= queue->data + MAX_QUEUE_CAPACITY)
    {
        queue->rear = queue->data;
    }

    *(queue->rear) = value;
    queue->rear++;
    queue->size++;

    return OPERATION_SUCCESS;
}

int dequeue_circular_buffer(circular_buffer_queue_t *queue)
{
    if (queue->front == queue->rear && queue->size == 0)
    {
        return QUEUE_EMPTY;
    }

    if (queue->front >= queue->data + MAX_QUEUE_CAPACITY)
    {
        queue->front = queue->data;
    }

    queue->front++;
    queue->size--;

    return OPERATION_SUCCESS;
}

void display_queue_contents(const circular_buffer_queue_t *queue)
{
    if (queue->size == 0)
    {
        printf("Очередь пуста\n");
        return;
    }

    if (queue->rear > queue->front)
    {
        for (int *current = queue->front; current < queue->rear; current++)
        {
            printf("%d ", *current);
        }
    }
    else
    {
        for (int *current = queue->front; current < queue->data + MAX_QUEUE_CAPACITY; current++)
        {
            printf("%d ", *current);
        }
        for (int *current = queue->data; current < queue->rear; current++)
        {
            printf("%d ", *current);
        }
    }
    printf("\n");
}

bool is_queue_empty(const circular_buffer_queue_t *queue)
{
    return queue->size == 0;
}

bool is_queue_full(const circular_buffer_queue_t *queue)
{
    return queue->size >= MAX_QUEUE_CAPACITY;
}
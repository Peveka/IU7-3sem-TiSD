#include "linked_queue.h"

void initialize_linked_queue(dynamic_linked_queue_t *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

queue_node_t *create_queue_node(int value)
{
    queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));
    if (new_node != NULL)
    {
        new_node->data = value;
        new_node->next = NULL;
    }
    return new_node;
}

int enqueue_linked(dynamic_linked_queue_t *queue, int value)
{
    queue_node_t *new_node = create_queue_node(value);
    if (new_node == NULL)
    {
        return MEMORY_ALLOCATION_FAILED;
    }

    if (queue->rear == NULL)
    {
        queue->front = new_node;
        queue->rear = new_node;
    }
    else
    {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    queue->size++;
    return OPERATION_SUCCESS;
}

int dequeue_linked(dynamic_linked_queue_t *queue)
{
    if (queue->front == NULL)
    {
        return QUEUE_EMPTY;
    }

    queue_node_t *temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;
    return OPERATION_SUCCESS;
}

void display_linked_queue(const dynamic_linked_queue_t *queue)
{
    if (queue->front == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }

    queue_node_t *current = queue->front;
    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void cleanup_linked_queue(dynamic_linked_queue_t *queue)
{
    while (queue->front != NULL)
    {
        queue_node_t *temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    queue->rear = NULL;
    queue->size = 0;
}

void initialize_memory_tracker(memory_tracker_t *tracker)
{
    tracker->addresses = (size_t *)malloc(MAX_QUEUE_CAPACITY * sizeof(size_t));
    tracker->allocations = (bool *)malloc(MAX_QUEUE_CAPACITY * sizeof(bool));
    tracker->deallocations = (bool *)malloc(MAX_QUEUE_CAPACITY * sizeof(bool));
    tracker->count = 0;
}

void record_allocation(memory_tracker_t *tracker, void *address)
{
    if (tracker->count < MAX_QUEUE_CAPACITY)
    {
        tracker->addresses[tracker->count] = (size_t)address;
        tracker->allocations[tracker->count] = true;
        tracker->deallocations[tracker->count] = false;
        tracker->count++;
    }
}

void record_deallocation(memory_tracker_t *tracker, void *address)
{
    if (tracker->count < MAX_QUEUE_CAPACITY)
    {
        tracker->addresses[tracker->count] = (size_t)address;
        tracker->allocations[tracker->count] = false;
        tracker->deallocations[tracker->count] = true;
        tracker->count++;
    }
}

void display_memory_operations(const memory_tracker_t *tracker)
{
    if (tracker->count == 0)
    {
        printf("Операции с памятью не зафиксированы\n");
        return;
    }

    printf("\n════════════════ ЖУРНАЛ ОПЕРАЦИЙ С ПАМЯТЬЮ ════════════════\n");
    for (size_t i = 0; i < tracker->count; i++)
    {
        if (tracker->allocations[i])
        {
            printf("Выделено: %p\n", (void *)tracker->addresses[i]);
        }
        if (tracker->deallocations[i])
        {
            printf("Освобождено: %p\n", (void *)tracker->addresses[i]);
        }
    }
}
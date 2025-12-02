#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include "common_defines.h"

typedef struct
{
    int data[MAX_QUEUE_CAPACITY];
    int size;
    int *front;
    int *rear;
} circular_buffer_queue_t;

void initialize_circular_buffer(circular_buffer_queue_t *queue);
int enqueue_circular_buffer(circular_buffer_queue_t *queue, int value);
int dequeue_circular_buffer(circular_buffer_queue_t *queue);
void display_queue_contents(const circular_buffer_queue_t *queue);
bool is_queue_empty(const circular_buffer_queue_t *queue);
bool is_queue_full(const circular_buffer_queue_t *queue);

#endif
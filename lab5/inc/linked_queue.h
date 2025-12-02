#ifndef LINKED_QUEUE_H
#define LINKED_QUEUE_H

#include "common_defines.h"

typedef struct queue_node
{
    int data;
    struct queue_node *next;
} queue_node_t;

typedef struct
{
    queue_node_t *front;
    queue_node_t *rear;
    int size;
} dynamic_linked_queue_t;

typedef struct
{
    size_t *addresses;
    bool *allocations;
    bool *deallocations;
    size_t count;
} memory_tracker_t;

void initialize_linked_queue(dynamic_linked_queue_t *queue);
queue_node_t *create_queue_node(int value);
int enqueue_linked(dynamic_linked_queue_t *queue, int value);
int dequeue_linked(dynamic_linked_queue_t *queue);
void display_linked_queue(const dynamic_linked_queue_t *queue);
void cleanup_linked_queue(dynamic_linked_queue_t *queue);

void initialize_memory_tracker(memory_tracker_t *tracker);
void record_allocation(memory_tracker_t *tracker, void *address);
void record_deallocation(memory_tracker_t *tracker, void *address);
void display_memory_operations(const memory_tracker_t *tracker);

#endif
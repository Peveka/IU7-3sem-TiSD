#ifndef QUEUE_ARRAY_H
#define QUEUE_ARRAY_H

#include <stdio.h>
#include "queue_list.h"
#define MAX_QUEUE_SIZE 10 

typedef struct 
{
    request_t array[MAX_QUEUE_SIZE];
    int pin;
    int pout;
    int count;
    size_t memory_used;
} array_queue_t;

void create_queue_array(array_queue_t *q);
int is_empty_array(const array_queue_t *q);
int is_full_array(const array_queue_t *q);
int enqueue_array(array_queue_t *q, request_t req, int verbose);
int dequeue_array(array_queue_t *q, request_t *req, int verbose);
void print_queue_array(const array_queue_t *q);
size_t get_array_queue_memory(const array_queue_t *q);

#endif
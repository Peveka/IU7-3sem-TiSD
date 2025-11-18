#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#define MAX_ADDR_COUNT 100 

typedef struct 
{
    int id;        
    int cycles;    
    double time_in; 
} request_t;

typedef struct node
{
    request_t data;        
    struct node *next;     
} node_t;

typedef struct 
{
    node_t *pin;       
    node_t *pout;      
    int count;         
    size_t memory_used; // Память, используемая очередью
} queue_t;

// Массив для хранения освобожденных адресов
extern void *free_list[MAX_ADDR_COUNT];
extern int free_list_count; 
extern void *alloc_list[MAX_ADDR_COUNT];
extern int alloc_list_count;

void create_queue(queue_t *q);
int is_empty(const queue_t *q);
int enqueue(queue_t *q, request_t req, int verbose);
int dequeue(queue_t *q, request_t *req, int verbose);
void print_free_list();
void print_alloc_list();
void print_queue_list(const queue_t *q);
size_t get_queue_memory(const queue_t *q);
void clear_alloc_list();

#endif
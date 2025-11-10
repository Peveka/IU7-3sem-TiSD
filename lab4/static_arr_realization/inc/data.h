#ifndef DATA_H__
#define DATA_H__

#define MAX_ARR_LEN 128

typedef struct stack_elem_t
{
    char letter;
} stack_elem_t;

typedef struct stack_t
{
    stack_elem_t data[MAX_ARR_LEN];
    int max_size;                         
    int size;                
} stack_t;

#endif
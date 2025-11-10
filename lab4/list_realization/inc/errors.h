#ifndef ERRORS_H__
#define ERRORS_H__

typedef enum
{
    OK = 0,
    ERR_STACK_FULL = 1,
    ERR_MEMORY = 2,
    ERR_STACK_EMPTY = 3,
    ERR_INVALID_INPUT = 4
} error_t;

#endif
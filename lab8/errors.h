#ifndef ERRORS_H__
#define ERRORS_H__

typedef enum
{
    OK,
    ERR_NOT_ENOUGH_DATA,
    ERR_INVALID_DATA,
    ERR_MEM_ALLOC,
    ERR_FILE_OPEN,
    ERR_GRAPH_EMPTY,
    ERR_VERTEX_NOT_FOUND,
    ERR_EDGE_EXISTS,
    ERR_EDGE_NOT_FOUND,
    ERR_NEGATIVE_CYCLE
} error_t;

const char* error_message(error_t err);

#endif
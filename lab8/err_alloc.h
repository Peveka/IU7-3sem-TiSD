#ifndef ERR_ALLOC_H
#define ERR_ALLOC_H

#include <stdlib.h>
#include "errors.h"

void* safe_malloc(size_t size, error_t* err);
void* safe_realloc(void* ptr, size_t size, error_t* err);

#endif
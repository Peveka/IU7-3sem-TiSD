#include "err_alloc.h"

void* safe_malloc(size_t size, error_t* err) {
    void* ptr = malloc(size);
    if (!ptr && err) {
        *err = ERR_MEM_ALLOC;
    }
    return ptr;
}

void* safe_realloc(void* ptr, size_t size, error_t* err) {
    void* new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0 && err) {
        *err = ERR_MEM_ALLOC;
    }
    return new_ptr;
}
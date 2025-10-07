#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memory_alloc(size_t size);
void memory_free(void* pointer);
void* memory_realloc(void* pointer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
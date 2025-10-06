#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void* memory_alloc(size_t size);
void memory_free(void* pointer);
void* memory_realloc(void* pointer, size_t size);

#endif
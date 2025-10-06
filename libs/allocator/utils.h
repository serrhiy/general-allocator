#ifndef ALLOCATOR_UTILD_H
#define ALLOCATOR_UTILD_H

#include <stddef.h>

long int get_system_page_size();

size_t get_real_memory_size(size_t memory_size);

size_t get_correct_pages_number(size_t memory_size);

size_t get_memory_size_for_new_area(size_t memory_size);

uint32_t crc8(uint8_t* data, size_t length);

#endif
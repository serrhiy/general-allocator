#ifndef ALLOCATOR_BLOCK_H
#define ALLOCATOR_BLOCK_H

#include <stddef.h>
#include <stdint.h>

// #define FIRST_AREA_FLAG 1
// #define LAST_AREA_FLAG 1 << 1
#define DEFAULT_AREA 1 << 2
#define EXTENDED_AREA 1 << 3

#define BLOCK_EMPTY 1

struct __attribute__((packed)) Block {
  // Do not change order and data types!!!
  size_t size;               // 4b or 8b
  uint8_t flags;             // 1b
  uint8_t checksum;          // 1b
  struct Block* prev_block;  // 4b or 8b
};  // total: 10b or 18b

struct __attribute__((packed)) Area {
  // Do not change order and data types!!!
  uint16_t total_blocks_number;  // 2b
  uint16_t free_blocks_number;   // 2b
  size_t payload_size;           // 4b or 8b
  size_t area_size;              // 4b or 8b
  struct Area* next_area;        // 4b or 8b
  struct Area* prev_area;        // 4b or 8b
  struct Block* last_block;      // 4b or 8b
};  // total: 24b or 44b

#endif

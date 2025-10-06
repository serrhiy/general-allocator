#include "allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

#include "block.h"
#include "utils.h"

static struct Area* first_area = NULL;
static struct Area* current_area = NULL;

uint8_t* find_free_block(struct Area* area, size_t memory_size,
                         struct Area** output_area, struct Block** prev_block) {
  if (area == NULL) {
    return NULL;
  }

  *output_area = area;

  size_t free_memory = area->area_size - area->payload_size;
  if (free_memory < memory_size && area->free_blocks_number == 0) {
    return find_free_block(area->next_area, memory_size, output_area,
                           prev_block);
  }

  if (free_memory > memory_size) {
    *prev_block = area->last_block;
    return (uint8_t*)area + area->payload_size - sizeof(struct Area);
  }

  size_t best_size = SIZE_MAX;
  uint8_t* best_block = NULL;
  uint8_t* block_ptr = (uint8_t*)area + sizeof(struct Area);
  for (size_t i = 0; i < area->total_blocks_number; i++) {
    struct Block* block = (struct Block*)block_ptr;
    if (block->flags & BLOCK_EMPTY && block->size >= memory_size &&
        block->size < best_size) {
      best_size = block->size;
      best_block = (uint8_t*)block;
    }
    *prev_block = (struct Block*)block_ptr;
    block_ptr += block->size;
  }

  if (best_block == NULL) {
    return find_free_block(area->next_area, memory_size, output_area,
                           prev_block);
  }

  return block_ptr;
}

void* memory_alloc(size_t memory_size) {
  size_t real_ms = memory_size + sizeof(struct Block);

  struct Block* prev_block = NULL;
  struct Area* output_area = NULL;
  uint8_t* free_block =
      find_free_block(first_area, real_ms, &output_area, &prev_block);

  if (free_block == NULL) {
    real_ms += sizeof(struct Area);
    size_t pages_number = get_correct_pages_number(real_ms);
    size_t area_size = pages_number * get_system_page_size();
    void* area = mmap(NULL, area_size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANON, -1, 0);
    if (area == MAP_FAILED) {
      return NULL;
    }

    struct Area* area_p = area;
    area_p->total_blocks_number = 0;
    area_p->free_blocks_number = 0;
    area_p->payload_size = sizeof(struct Area);
    area_p->area_size = area_size;
    area_p->next_area = NULL;
    area_p->prev_area = current_area;

    if (first_area == NULL) {
      first_area = area;
    } else {
      current_area->next_area = area;
    }

    current_area = area;

    output_area = area;
    free_block = area + sizeof(struct Area);
  }

  struct Block* block_ptr = (struct Block*)free_block;
  block_ptr->flags = 0;
  block_ptr->prev_block = prev_block;
  block_ptr->size = memory_size + sizeof(struct Block);

  current_area->total_blocks_number++;
  current_area->last_block = block_ptr;
  current_area->payload_size += block_ptr->size;

  return free_block + sizeof(struct Block);
}

// void memory_free(void* pointer) {}

// void* memory_realloc(void* pointer, size_t size) { return NULL; }

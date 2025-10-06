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

struct Block* find_block(struct Area* area, uint8_t* ptr,
                         struct Area** output_area) {
  ptr -= sizeof(struct Block);
  for (struct Area* area_ptr = area; area_ptr != NULL;
       area_ptr = area->next_area) {
    if (ptr >= (uint8_t*)area_ptr &&
        ptr < (uint8_t*)area_ptr + area_ptr->area_size) {
      *output_area = area_ptr;
      struct Block* block = (struct Block*)ptr;
      uint8_t checksum =
          crc8(ptr, sizeof(struct Block) - sizeof(block->checksum));
      return checksum == block->checksum ? block : NULL;
    }
  }
  *output_area = NULL;
  return NULL;
}

uint8_t* find_free_block(struct Area* area, size_t memory_size,
                         struct Area** output_area) {
  if (area == NULL) {
    return NULL;
  }

  *output_area = area;

  size_t free_memory = area->area_size - area->payload_size;
  if (free_memory < memory_size && area->free_blocks_number == 0) {
    return find_free_block(area->next_area, memory_size, output_area);
  }

  if (free_memory > memory_size) {
    return (uint8_t*)area + area->payload_size;
  }

  size_t best_size = SIZE_MAX;
  uint8_t* best_block = NULL;
  uint8_t* block_ptr = (uint8_t*)area + sizeof(struct Area);
  for (size_t i = 0; i < area->blocks_number; i++) {
    struct Block* block = (struct Block*)block_ptr;
    if (block->flags & FREE_BLOCK && block->size >= memory_size &&
        block->size < best_size) {
      best_size = block->size;
      best_block = (uint8_t*)block;
    }
    block_ptr += block->size;
  }

  if (best_block == NULL) {
    return find_free_block(area->next_area, memory_size, output_area);
  }

  return best_block;
}

void* memory_alloc(size_t memory_size) {
  if (memory_size == 0) {
    return NULL;
  }

  size_t real_ms = memory_size + sizeof(struct Block);

  struct Area* output_area = NULL;
  uint8_t* free_block = find_free_block(first_area, real_ms, &output_area);

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
    area_p->blocks_number = 0;
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
  block_ptr->size = memory_size + sizeof(struct Block);
  block_ptr->checksum =
      crc8(free_block, sizeof(struct Block) - sizeof(block_ptr->checksum));

  output_area->blocks_number++;
  output_area->last_block = block_ptr;
  output_area->payload_size += block_ptr->size;

  return free_block + sizeof(struct Block);
}

void memory_free(void* pointer) {
  struct Area* output_area = NULL;
  struct Block* block = find_block(first_area, (uint8_t*)pointer, &output_area);
  if (block == NULL || block->flags & FREE_BLOCK) {
    return;
  }

  block->flags |= FREE_BLOCK;
  output_area->free_blocks_number++;
  if (output_area->free_blocks_number == output_area->blocks_number) {
    struct Area* temp = output_area->prev_area;
    if (output_area->prev_area != NULL) {
      output_area->prev_area->next_area = output_area->next_area;
    }
    if (output_area->next_area != NULL) {
      output_area->next_area->prev_area = temp;
    }

    if (output_area == first_area) {
      if (first_area->next_area != NULL) {
        first_area = first_area->next_area;
      } else {
        first_area = NULL;
      }
    }

    munmap((void*)output_area, output_area->area_size);
  }
}

void* memory_realloc(void* pointer, size_t size) {
  struct Area* output_area = NULL;
  struct Block* block = find_block(first_area, (uint8_t*)pointer, &output_area);

  if (block == NULL || block->flags & FREE_BLOCK) {
    return NULL;
  }

  size_t old_size = block->size;
  if (size <= block->size - sizeof(struct Block)) {
    if (block == output_area->last_block) {
      block->size = size + sizeof(struct Block);
      block->checksum =
          crc8((uint8_t*)block, sizeof(struct Block) - sizeof(block->checksum));
      output_area->payload_size -= old_size - block->size;
    }
    return pointer;
  }

  if (block == output_area->last_block) {
    size_t new_area_payload_size =
        output_area->payload_size - block->size + size + sizeof(struct Block);
    if (new_area_payload_size <= output_area->area_size) {
      block->size = size + sizeof(struct Block);
      block->checksum =
          crc8((uint8_t*)block, sizeof(struct Block) - sizeof(block->checksum));
      output_area->payload_size += block->size - old_size;
      return pointer;
    }
  }

  memory_free(pointer);
  return memory_alloc(size);
}

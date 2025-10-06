#include "utils.h"

#include <math.h>
#include <stddef.h>
#include <unistd.h>

#include "block.h"

long int get_system_page_size() {
  static long int page_size = -1;
  if (page_size == -1) {
    page_size = sysconf(_SC_PAGESIZE);
  }
  return page_size;
}

size_t get_real_memory_size(size_t memory_size) {
  long int page_size = get_system_page_size();
  if (memory_size > page_size) {
    return memory_size + sizeof(struct Area);
  }
  size_t real_size = memory_size + sizeof(struct Block);
  return real_size > page_size ? memory_size + sizeof(struct Area) : real_size;
}

size_t get_correct_pages_number(size_t memory_size) {
  return (size_t)ceil((double)memory_size / (double)get_system_page_size());
}

size_t get_memory_size_for_new_area(size_t memory_size) {
  long int page_size = get_system_page_size();
  if (memory_size > page_size) {
    return memory_size + sizeof(struct Area);
  }
  size_t real_size = memory_size + sizeof(struct Block) + sizeof(struct Area);
  return real_size > page_size ? memory_size + sizeof(struct Area) : real_size;
}

uint8_t crc8(const uint8_t* data, size_t length) {
  uint8_t crc = 0x00;
  uint8_t poly = 0x07;

  for (size_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ poly;
      else
        crc <<= 1;
    }
  }

  return crc;
}

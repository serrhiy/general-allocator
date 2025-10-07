#include <allocator.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>
#include <iostream>
#include <utils.hh>

const size_t MEMORY_SIZE = 3000;

void dump_memory_and_wait(uint8_t* memory) {
  std::cout << mem_info(test_structure(memory)) << '\n';
  std::cin.get();
}

int main() {
  uint8_t* memory2 = (uint8_t*)memory_alloc(MEMORY_SIZE / 5);
  dump_memory_and_wait(memory2);

  uint8_t* memory1 = (uint8_t*)memory_alloc(MEMORY_SIZE);
  dump_memory_and_wait(memory2);

  uint8_t* memory3 = (uint8_t*)memory_alloc(MEMORY_SIZE);
  dump_memory_and_wait(memory2);

  memset(memory1, 1, MEMORY_SIZE);
  memset(memory2, 2, MEMORY_SIZE / 5);
  memset(memory3, 3, MEMORY_SIZE);
  dump_memory_and_wait(memory2);

  memory1 = (uint8_t*)memory_realloc(memory1, MEMORY_SIZE * 2);
  dump_memory_and_wait(memory2);

  memory3 = (uint8_t*)memory_realloc(memory3, MEMORY_SIZE / 2);
  dump_memory_and_wait(memory2);

  memory_free(memory1);
  dump_memory_and_wait(memory2);
  memory_free(memory3);
  dump_memory_and_wait(memory2);
  memory_free(memory2);

  return 0;
}

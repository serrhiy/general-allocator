#include <assert.h>
#include <string.h>
#include <allocator.h>
#include <stddef.h>
#include <stdint.h>

const size_t MEMORY_SIZE = 3000;

int main() {
  uint8_t* memory1 = (uint8_t*)memory_alloc(MEMORY_SIZE);
  uint8_t* memory2 = (uint8_t*)memory_alloc(MEMORY_SIZE / 5);
  uint8_t* memory3 = (uint8_t*)memory_alloc(MEMORY_SIZE);

  memset(memory1, 1, MEMORY_SIZE);
  memset(memory2, 2, MEMORY_SIZE);
  memset(memory2, 3, MEMORY_SIZE / 5);

  memory1 = memory_realloc(memory1, MEMORY_SIZE * 2);
  memory3 = memory_realloc(memory2, MEMORY_SIZE / 2);

  memory_free(memory2);
  memory_free(memory1);
  memory_free(memory3);

  return 0;
}

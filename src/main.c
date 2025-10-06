#include "allocator/allocator.h"

int main() {
  void* memory1 = memory_alloc(3000);
  void* memory2 = memory_alloc(500);
  void* memory3 = memory_alloc(2000);

  memory_free(memory2);
  memory_free(memory1);
  memory_free(memory3);
}

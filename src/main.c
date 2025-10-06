#include "allocator/allocator.h"

int main() {
  void* memory1 = memory_alloc(3000);
  memory_realloc(memory1, 1000);
}

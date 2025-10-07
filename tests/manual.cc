#include <cassert>
#include <cstddef>
#include <cstdint>

#include <allocator.h>

constexpr size_t MIN_SIZE = 100;
constexpr size_t MIDDLE_SIZE = 400;
constexpr size_t LARGE_SIZE = 1000;

void allocation() {
  size_t min = MIN_SIZE, mid = MIDDLE_SIZE, big = LARGE_SIZE;

  int32_t* memory1 = static_cast<int32_t*>(memory_alloc(sizeof(int32_t) * min));
  int32_t* memory2 = static_cast<int32_t*>(memory_alloc(sizeof(int32_t) * mid));
  int32_t* memory3 = static_cast<int32_t*>(memory_alloc(sizeof(int32_t) * big));

  for (size_t index = 0; index < min; index++) memory1[index] = index;
  for (size_t index = 0; index < mid; index++) memory2[index] = index;
  for (size_t index = 0; index < big; index++) memory3[index] = index;

  memory1 = static_cast<int32_t*>(memory_realloc(memory1, sizeof(int32_t) * min * 2));
  memory2 = static_cast<int32_t*>(memory_realloc(memory2, sizeof(int32_t) * mid * 2));
  memory3 = static_cast<int32_t*>(memory_realloc(memory3, sizeof(int32_t) * big * 2));

  for (size_t index = 0; index < min; index++) assert(memory1[index] == index);
  for (size_t index = 0; index < mid; index++) assert(memory2[index] == index);
  for (size_t index = 0; index < big; index++) assert(memory3[index] == index);

  memory1 = static_cast<int32_t*>(memory_realloc(memory1, sizeof(int32_t) * min / 2));
  memory2 = static_cast<int32_t*>(memory_realloc(memory2, sizeof(int32_t) * mid / 2));
  memory3 = static_cast<int32_t*>(memory_realloc(memory3, sizeof(int32_t) * big / 2));

  // for (size_t index = 0; index < min / 2; index++) assert(memory1[index] == index);
  // for (size_t index = 0; index < mid / 2; index++) assert(memory2[index] == index);
  // for (size_t index = 0; index < big / 2; index++) assert(memory3[index] == index);

  // memory_free(memory1);
  // memory_free(memory2);
  // memory_free(memory3);
}

int main() {
  allocation();
  return 0;
}
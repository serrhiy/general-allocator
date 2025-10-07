#include <allocator.h>
#include <block.h>

#include <cassert>
#include <cstddef>
#include <limits>

#include "utils.hh"

constexpr size_t SMALL_MEMORY_SIZE = (1 << 20);        // 1mb
constexpr size_t MIDDLE_MEMORY_SIZE = (1 << 20) * 10;  // 10mb
constexpr size_t BIG_MEMORY_SIZE = (1 << 20) * 100;    // 100mb

void testSmallSingleInvocation(size_t size) {
  uint8_t* memory = static_cast<uint8_t*>(memory_alloc(size));
  for (size_t index = 0; index < size; index++) {
    memory[index] = index % (std::numeric_limits<uint8_t>::max() + 1);
  }

  const allocator_structure_t structure = test_structure(memory);

  assert(structure.size() == 1 && "Created more than 1 areas");

  const auto& [area, blocks] = structure[0];

  assert((area.blocks_number == 1 && blocks.size() == 1) &&
         "Created more than 1 blocks");
  assert(area.payload_size == size + sizeof(Area) + sizeof(Block) &&
         "Invalid payload size");
  assert(area.free_blocks_number == 0 && "Invalid free blocks number");

  const TestBlock& block = blocks[0];
  assert(block.block.size == size + sizeof(Block) && "Invalid block size");

  for (size_t index = 0; index < size; index++) {
    assert(memory[index] == block.block_data[index] && "Values mismatch");
  }
  memory_free(memory);
}

void testSingleInvocations() {
  testSmallSingleInvocation(SMALL_MEMORY_SIZE);
  testSmallSingleInvocation(MIDDLE_MEMORY_SIZE);
  testSmallSingleInvocation(BIG_MEMORY_SIZE);
}

void testMultipleInvocations() {
  constexpr size_t ALLOC_SIZE = 1 << 10;
  constexpr size_t NUM_BLOCKS = 5;

  std::vector<uint8_t*> blocks;
  for (size_t i = 0; i < NUM_BLOCKS; i++) {
    uint8_t* memory = static_cast<uint8_t*>(memory_alloc(ALLOC_SIZE));
    assert(memory != nullptr && "Allocation failed");
    for (size_t j = 0; j < ALLOC_SIZE; j++) {
      memory[j] = (i + j) % 256;
    }
    blocks.push_back(memory);
  }

  for (size_t i = 0; i < NUM_BLOCKS; i += 2) {
    memory_free(blocks[i]);
  }

  for (size_t i = 0; i < NUM_BLOCKS / 2; i++) {
    void* mem = memory_alloc(ALLOC_SIZE / 2);
    assert(mem != nullptr && "Reallocation failed");
  }

  for (size_t i = 1; i < NUM_BLOCKS; i += 2) {
    uint8_t* mem = static_cast<uint8_t*>(blocks[i]);
    for (size_t j = 0; j < ALLOC_SIZE; j++) {
      assert(mem[j] == (i + j) % 256 && "Data corruption after realloc");
    }
  }

  for (size_t i = 1; i < NUM_BLOCKS; i += 2) {
    memory_free(blocks[i]);
  }
}

int main() {
  testSingleInvocations();
  testMultipleInvocations();
  return 0;
}

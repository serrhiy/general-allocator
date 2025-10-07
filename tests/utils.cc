#include "utils.hh"

#include <allocator.h>
#include <block.h>

#include <cstring>
#include <memory>
#include <utility>
#include <vector>

allocator_structure_t test_structure(void* first_area) {
  allocator_structure_t result;
  uint8_t* area_begin =
      static_cast<uint8_t*>(first_area) - sizeof(Area) - sizeof(Block);
  Area* area_start = reinterpret_cast<Area*>(area_begin);
  for (Area* area = area_start; area != NULL; area = area->next_area) {
    uint8_t* raw_area = reinterpret_cast<uint8_t*>(area);
    Area area_data;
    std::memcpy(&area_data, raw_area, sizeof(Area));
    std::vector<TestBlock> blocks;
    uint8_t* block_ptr = raw_area + sizeof(Area);
    for (size_t index = 0; index < area->blocks_number; index++) {
      Block block;
      std::memcpy(&block, block_ptr, sizeof(block));
      size_t content_size = block.size - sizeof(Block);
      std::unique_ptr<uint8_t[]> block_data =
          std::make_unique<uint8_t[]>(content_size);
      std::memcpy(block_data.get(), block_ptr + sizeof(Block), content_size);
      blocks.emplace_back(TestBlock{block, std::move(block_data)});
      block_ptr += block.size;
    }
    result.emplace_back(area_data, std::move(blocks));
  }
  return result;
}

#include "utils.hh"

#include <allocator.h>
#include <block.h>

#include <cstring>
#include <format>
#include <iomanip>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
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

constexpr size_t MAX_DATA_BLOCK_SIZE = 100;

std::string mem_info(const allocator_structure_t& memory_info) {
  std::ostringstream output_stream;
  output_stream << "User space memory has " << memory_info.size() << " areas\n";

  for (const auto& [index, area_info] : std::views::enumerate(memory_info)) {
    const auto& [area, blocks] = area_info;
    output_stream << "#" << index
                  << " Area{blocks_number = " << area.blocks_number
                  << ", free_blocks_number = " << area.free_blocks_number
                  << ", payload_size = " << area.payload_size
                  << ", area_size = " << area.area_size << "}:\n";
    for (const auto& [index, block] : std::views::enumerate(blocks)) {
      output_stream << "\t#" << index << " Block{size = " << block.block.size
                    << ", flags = " << static_cast<int>(block.block.flags)
                    << ", checksum = " << static_cast<int>(block.block.checksum)
                    << "\n";
      output_stream << "\tdata: ";
      for (size_t i = 0; i < MAX_DATA_BLOCK_SIZE; i++) {
        output_stream << std::setw(2) << std::setfill('0')
                      << static_cast<int>(block.block_data[i]) << ' ';
      }
      if (MAX_DATA_BLOCK_SIZE < block.block.size - sizeof(struct Block)) {
        size_t diff = block.block.size - sizeof(struct Block) - MAX_DATA_BLOCK_SIZE;
        output_stream << "... more " << diff << " bytes";
      }
      output_stream << '\n';
    }
  }
  return output_stream.str();
}

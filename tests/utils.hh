#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <allocator.h>
#include <block.h>

#include <memory>
#include <utility>
#include <vector>

struct TestBlock {
  Block block;
  std::unique_ptr<uint8_t[]> block_data;
};

using allocator_structure_t =
    std::vector<std::pair<Area, std::vector<TestBlock>>>;

allocator_structure_t test_structure(void* first_area);

#endif
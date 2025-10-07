add_library(tests_library utils.cc)
target_compile_features(tests_library PRIVATE cxx_std_23)
set_target_properties(tests_library PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(tests_library allocator)
target_include_directories(tests_library INTERFACE .)

function(addTest filename testname)
  add_executable(${testname} ${filename})
  target_link_libraries(${testname} tests_library allocator)
  add_test(NAME ${testname} COMMAND ${testname})
endfunction()

addTest(allocator.cc "memory_allocation_test")
addTest(manual.cc "manual_test")

add_executable(simple_test simple.cc)
target_link_libraries(simple_test PRIVATE tests_library allocator)

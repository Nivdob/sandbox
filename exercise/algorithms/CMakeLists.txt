cmake_minimum_required(VERSION 3.2)

add_library(test_sort test_sort.h test_sort.cpp)
target_link_libraries(test_sort utils)
target_include_directories(test_sort INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(bubble_sort bubble_sort.cpp)
target_link_libraries(bubble_sort utils test_sort)
add_test(NAME bubble_sort COMMAND bubble_sort test)

add_executable(insertion_sort insertion_sort.cpp)
target_link_libraries(insertion_sort utils test_sort)
add_test(NAME insertion_sort COMMAND insertion_sort test)

add_executable(selection_sort selection_sort.cpp)
target_link_libraries(selection_sort utils test_sort)
add_test(NAME selection_sort COMMAND selection_sort test)

add_executable(merge_sort merge_sort.cpp)
target_link_libraries(merge_sort utils test_sort)
add_test(NAME merge_sort COMMAND merge_sort test)

add_executable(quick_sort quick_sort.cpp)
target_link_libraries(quick_sort utils test_sort)
add_test(NAME quick_sort COMMAND quick_sort test)

add_executable(heap_sort heap_sort.cpp)
target_link_libraries(heap_sort utils test_sort)
add_test(NAME heap_sort COMMAND heap_sort test)

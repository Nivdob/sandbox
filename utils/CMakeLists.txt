cmake_minimum_required(VERSION 3.2)

set(HEADERS
	countof.h
	array_file.h
	test_sort.h)
set(SOURCES
	array_file.cpp
	test_sort.cpp)
add_library(utils ${HEADERS} ${SOURCES})
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.2)

set(HEADERS
	countof.h
	array_file.h)
set(SOURCES
	array_file.cpp)
add_library(utils ${HEADERS} ${SOURCES})
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

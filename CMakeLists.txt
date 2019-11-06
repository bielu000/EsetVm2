cmake_minimum_required(VERSION 3.12)

project(EsetVm2)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

#
#set(ESET_LIB_SOURCES
#	src/main.cpp
#	src/Evm.cpp
#	src/Loader.cpp
#	src/Decoder.cpp
#)
#
#set(INCLUDE_DIR inc)
#
#add_library()
#add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
#
#
#include_directories(${INCLUDE_DIR})
#
#add_subdirectory(tests)
#
#target_link_libraries(${CMAKE_PROJECT_NAME} stdc++fs)

cmake_minimum_required(VERSION 3.12)

project(EsetVm2)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(src)

set(SOURCES
        src/main.cpp
        src/Evm.cpp
)

set(INCLUDE_DIR inc)


add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

include_directories(${INCLUDE_DIR})
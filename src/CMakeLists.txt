
include_directories(../inc)


add_library(
    Evm Evm.cpp Decoder.cpp Instruction.cpp Loader.cpp
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
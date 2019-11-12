
add_executable(EvmTestSuite src/MemBitStreamTest.cpp)

include_directories(../inc)

target_link_libraries(EvmTestSuite Evm)

target_link_libraries(EvmTestSuite gtest_main gtest pthread)

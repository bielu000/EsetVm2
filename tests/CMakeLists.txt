
add_executable(EvmTestSuite src/MemBitStreamTest.cpp)

target_link_libraries(EvmTestSuite Evm)

target_link_libraries(EvmTestSuite gtest_main gtest pthread)
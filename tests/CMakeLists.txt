
set(TEST_FIXTURES_SRC
    src/MemBitStreamTest.cpp
    src/MemoryTest.cpp
)


add_executable(EvmTestSuite ${TEST_FIXTURES_SRC})

include_directories(../inc)

target_link_libraries(EvmTestSuite Evm)

target_link_libraries(EvmTestSuite gtest_main gtest pthread)

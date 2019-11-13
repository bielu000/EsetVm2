//
// Created by pp on 12.11.19.
//

#include <gtest/gtest.h>
#include <Memory.hpp>

using namespace esetvm2::core;

class MemoryTestFixture : public ::testing::Test
{
};


TEST_F(MemoryTestFixture,  fetchMemoryUsingApropriateOffset)
{
  std::array<std::byte, 64> rawBytes {
    (std::byte)0xff, (std::byte)0xfa, (std::byte)0xde, (std::byte)0xc0, (std::byte)0xad, (std::byte)0xde
  };

  Memory sut;
  sut.alloc(rawBytes.size());
  std::copy(rawBytes.begin(), rawBytes.end(), sut.begin());

  uint16_t memoryOffset = 0;

  auto b1 = sut.read<uint8_t>(memoryOffset);
  ASSERT_EQ(b1, 0xff);

  auto b2 = sut.read<uint16_t>(++memoryOffset);
  ASSERT_EQ(b2, 0xdefa);

  auto b3 = sut.read<uint32_t>(++memoryOffset);
  ASSERT_EQ(b3, 0xdeadc0de);
}

TEST_F(MemoryTestFixture, fetchingMemoryUsingOutOfRangeOffsetCausesExceptionThrow)
{
  std::array<std::byte, 64> rawBytes {
    (std::byte)0xff
  };

  Memory sut;
  sut.alloc(rawBytes.size());

  try {
    sut.read<uint8_t>(rawBytes.size() + 1);
    FAIL();
  } catch (MemoryError& memoryError) {
    ASSERT_EQ(memoryError.getCode(), MemoryError::ErrorCode::OffsetOutOfRange);
  }
}

TEST_F(MemoryTestFixture, fetchingNotAllocatedMemoryCausesExceptionThrow)
{
  Memory sut;

  try {
    sut.read<uint8_t>(0);
  } catch (MemoryError& memoryError) {
    ASSERT_EQ(memoryError.getCode(), MemoryError::ErrorCode::MemoryNotAllocated);
  }
}
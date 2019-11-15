//
// Created by pp on 05.11.19.
//

#include <gtest/gtest.h>
#include <Memory.hpp>
#include <array>

using namespace esetvm2::core;

class MemBitStreamTestFixture : public ::testing::Test
{
protected:
  void SetUp() override
  {

  }

protected:

};

TEST_F(MemBitStreamTestFixture,  testNormalFetchBitsStream)
{
  auto bytes = std::array<std::byte, 64> {
    static_cast<std::byte>(0b10101010),
  };

  auto memory = Memory{};
  memory.alloc(bytes.size());
  std::copy(bytes.begin(), bytes.end(), memory.begin());

  auto sut = MemBitStream{&memory, 0};

  ASSERT_EQ(sut.get<2>(), 0b10);
  ASSERT_EQ(sut.get<4>(), 0b1010);
  ASSERT_EQ(sut.get<2>(), 0b10);
}


TEST_F(MemBitStreamTestFixture, loadMissingBits_ByLoadingOneByteFromMemory)
{
  //TODO: It should be configured in SetUp, on MemoryMock class, and appropriate methods like ON_CALL should returns
  //bytes that are needed in test. It will allow for to check whether e.g. Memory::loadxxx is be called by MemBitStream
  //whenever the stream needs to be updated because current amount of bits is not sufficient to complet MemButStream::get<x>() call
  auto bytes = std::array<std::byte, 64> {
    static_cast<std::byte>(0b10101010), //get<30> -> 8
    static_cast<std::byte>(0b10101010), //get<30> -> 8
    static_cast<std::byte>(0b10101010), //get<30> -> 8
    static_cast<std::byte>(0b10101010), //get<30> -> 6 | get<6> -> 2
    static_cast<std::byte>(0b11111111)  //get<6> -> 4 | get<2> -> 2
  };

  auto memory = Memory{};
  memory.alloc(bytes.size());
  std::copy(bytes.begin(), bytes.end(), memory.begin());

  auto sut = MemBitStream{&memory, 0 };
  ASSERT_EQ(sut.get<30>(), 0b101010101010101010101010101010);
  ASSERT_EQ(sut.get<6>(), 0b101111);
  ASSERT_EQ(sut.get<2>(), 0b11);
}

TEST_F(MemBitStreamTestFixture, loadMissingBits_ByLoadingTwoBytesFromMemory)
{
  auto bytes = std::array<std::byte, 64> {
    static_cast<std::byte>(0b10101010), //get<30> -> 8
    static_cast<std::byte>(0b10101010), //get<30> -> 8
    static_cast<std::byte>(0b10101010), //get<30> -> 8
    static_cast<std::byte>(0b10101010), //get<30> -> 6 | get<12> -> 2
    static_cast<std::byte>(0b11111111), //get<12> -> 8
    static_cast<std::byte>(0b10101010)  //get<12> -> 2
  };

  auto memory = Memory{};
  memory.alloc(bytes.size());
  std::copy(bytes.begin(), bytes.end(), memory.begin());

  auto sut = MemBitStream{&memory, 0 };
  ASSERT_EQ(sut.get<30>(), 0b101010101010101010101010101010);
  ASSERT_EQ(sut.get<12>(), 0b101111111110);
}
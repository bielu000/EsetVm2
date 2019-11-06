//
// Created by pp on 05.11.19.
//

#include <gtest/gtest.h>

class MemBitStreamTestFixture : public ::testing::Test
{
protected:
  virtual void SetUp() override
  {}

};

TEST_F(MemBitStreamTestFixture,  test)
{
  ASSERT_EQ(1, 1);
}
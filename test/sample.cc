#include "gtest/gtest.h"

namespace {

class MyTestSample : public ::testing::Test{};

TEST_F(MyIntTest, isOdd)
{
  std::string foo = "foo"
  EXPECT_EQ(false, foo == "bar");
}

} // namespace
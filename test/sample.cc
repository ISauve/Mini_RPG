#include "gtest/gtest.h"
#include "MyTestSample.h"

namespace {

// Test fixture
class MyTestSample : public ::testing::Test{};

TEST_F(MyTestSample, TestFixture)
{
  std::string foo = "foo";
  EXPECT_EQ(false, foo == "bar");
}

} // namespace
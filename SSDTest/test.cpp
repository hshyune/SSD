#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../SSD/SSD.cpp"

using namespace std;
using namespace testing;

class IoInterfaceMock : public IIoInterface {
public:
	MOCK_METHOD(int, read, (), (override));
	MOCK_METHOD(void, write, (), (override));
};

class SSDTest : public testing::Test {
public:
	IoInterfaceMock mock;
	SSD ssd{ &mock };
};

TEST_F(SSDTest, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST_F(SSDTest, TEST1) {
	int address = 1;
	int ret = ssd.read(1);
	EXPECT_EQ(ret, 0);
}


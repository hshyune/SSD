#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../SSD/SSD.cpp"

using namespace std;
using namespace testing;

class IoInterfaceMock : public IIoInterface {
public:
	MOCK_METHOD(int, read, (int address), (override));
	MOCK_METHOD(void, write, (int address, int data), (override));
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

TEST_F(SSDTest, Read) {
	EXPECT_CALL(mock, read(1)).WillOnce(Return(0));
	int address = 1;
	int ret = ssd.read(1);
	EXPECT_EQ(ret, 0);
}
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"

using namespace std;
using namespace testing;

class TestShellMock : public TestShell {
	MOCK_METHOD(void, write, (int address, int data), ());
	MOCK_METHOD(int, read, (int address), ());
	MOCK_METHOD(void, exit, (int address, int data), ());
	MOCK_METHOD(void, help, (int address, int data), ());
	MOCK_METHOD(void, fullwrite, (int data), ());
	MOCK_METHOD(void, fullread, (), ());
};

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}


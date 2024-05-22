#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"

using namespace std;
using namespace testing;

class TestShellMock : public TestShell {
public:
	MOCK_METHOD(void, write, (int address, int data), ());
	MOCK_METHOD(int, read, (int address), ());
	MOCK_METHOD(void, exit, (), ());
	MOCK_METHOD(void, help, (), ());
	MOCK_METHOD(void, fullwrite, (int data), ());
	MOCK_METHOD(void, fullread, (), ());
};

TEST(TestCaseName, TEST_SHELL_EXIT_CALL) {
	TestShellMock mock;

	EXPECT_CALL(mock, exit).Times(1);

	mock.exit();
}

TEST(TestCaseName, TEST_SHELL_EXIT_HELP) {
	TestShellMock mock;

	EXPECT_CALL(mock, help).Times(1);

	mock.help();
}


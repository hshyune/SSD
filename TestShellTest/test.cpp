#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"
#include <stdexcept>

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

TEST(TestCaseName, TEST_SHELL_READ_UNWRITTEN_LBA) {
	TestShellMock mock;

	int address = 0x0;
	int times = 3;
	int expectedData = 0x0;
	int resultData = mock.read(address);

	EXPECT_EQ(expectedData, resultData);
}

TEST(TestCaseName, TEST_SHELL_READ_WRITTEN_LBA) {
	TestShellMock mock;

	int address = 0x0;
	int times = 3;
	int expectedData = 0x12345678;
	mock.write(address, expectedData);
	int resultData = mock.read(address);

	EXPECT_EQ(expectedData, resultData);
}

TEST(TestCaseName, TEST_SHELL_WRITE_SUCCESS) {
	TestShellMock mock;

	EXPECT_CALL(mock, write(5, 0x12345678))
		.Times(1);

	mock.write(5, 0x12345678);
}

TEST(TestCaseName, TEST_SHELL_WRITE_INVLIAD_LBA) {
	TestShellMock mock;

	EXPECT_THROW(mock.write(999, 0x123), invalid_argument);
}

TEST(TestCaseName, TEST_SHELL_WRITE_INVLIAD_DATA) {
	TestShellMock mock;

	EXPECT_THROW(mock.write(5, 0xFFFFFFFF0), invalid_argument);
}


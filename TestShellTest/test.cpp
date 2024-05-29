//#include "gtest/gtest.h"
//#include "gmock/gmock.h"
//#include "../TestShell/TestShell.cpp"
//#include <stdexcept>
//
//using namespace std;
//using namespace testing;
//
//class TestShellMock : public TestShell {
//public:
//	MOCK_METHOD(void, write, (int address, string data), ());
//	MOCK_METHOD(string, read, (int address), ());
//	MOCK_METHOD(void, exit, (), ());
//	MOCK_METHOD(int, help, (), ());
//	MOCK_METHOD(void, fullwrite, (string data), ());
//	MOCK_METHOD(void, fullread, (), ());
//};
//
//class FixtureTestShell : public testing::Test {
//public:
//	TestShell testShell;
//
//};
//
//class FixtureTestShellMock : public testing::Test {
//public:
//	TestShellMock mock;
//
//};
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_EXIT_CALL) {
//	EXPECT_CALL(mock, exit).Times(1);
//
//	mock.exit();
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_EXIT_HELP) {
//	EXPECT_CALL(mock, help).Times(1);
//
//	mock.help();
//}
//
//TEST_F(FixtureTestShell, TEST_SHELL_HELP_READLINES) {
//	int ret = testShell.help();
//	EXPECT_THAT(ret, Gt(1));
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_READ_UNWRITTEN_LBA) {
//	int address = 0x0;
//	string expectedData = "0x0";
//
//	EXPECT_CALL(mock, read).WillOnce(Return(expectedData));
//	string resultData = mock.read(address);
//	EXPECT_EQ(expectedData, resultData);
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_READ_WRITTEN_LBA) {
//	int address = 0x0;
//	string expectedData = "0x12345678";
//
//	mock.write(address, expectedData);
//	EXPECT_CALL(mock, read).WillOnce(Return(expectedData));
//	string resultData = mock.read(address);
//	EXPECT_EQ(expectedData, resultData);
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_WRITE_SUCCESS) {
//	string data = "0x12345678";
//	EXPECT_CALL(mock, write(5, data))
//		.Times(1);
//
//	mock.write(5, data);
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_WRITE_INVLIAD_LBA) {
//	EXPECT_THROW(mock.write(999, "0x123"), invalid_argument);
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_WRITE_INVLIAD_DATA) {
//	EXPECT_THROW(mock.write(5, "0xFFFFFFFF0"), invalid_argument);
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_FULL_WRITE_Call) {
//	string data = "ab";
//
//	EXPECT_CALL(mock, fullwrite(data))
//		.Times(1);
//	mock.fullwrite(data);
//}
//
//TEST_F(FixtureTestShellMock, TEST_SHELL_FULL_READ_Call) {
//	EXPECT_CALL(mock, fullread())
//		.Times(1);
//
//	mock.fullread();
//}
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../SSD/SSD.cpp"

using namespace std;
using namespace testing;

class IoInterfaceMock : public IIoInterface {
public:
	MOCK_METHOD(string, read, (int address), (override));
	MOCK_METHOD(void, write, (int address, const string& data), (override));
};

class SSDTest : public testing::Test {
public:
	IoInterfaceMock mock;
	SSD ssd{ &mock };
};

TEST_F(SSDTest, Read_LBA_Success) {
	int address = 1;
	string expectedData = "0x12345678";
	EXPECT_CALL(mock, read(address)).Times(1).WillRepeatedly(Return(expectedData));
	string ret = ssd.read(address);
	EXPECT_EQ(ret, expectedData);
}

TEST_F(SSDTest, Read_LBA_OutOfBound) {
	int address = 200;
	string expectedData = "";
	EXPECT_CALL(mock, read(address)).Times(0);
	string ret = ssd.read(address);
	EXPECT_EQ(ret, expectedData);
}

TEST_F(SSDTest, Write_Success) {
	int address = 10;
	string data = "0x12345678";
	EXPECT_CALL(mock, write(address, data)).Times(1);
	ssd.write(address, data);
}

TEST_F(SSDTest, Write_OutOfBound) {
	int address = 200;
	string data = "0x12345678";
	EXPECT_CALL(mock, write(address, data)).Times(0);
	ssd.write(address, data);
}

TEST_F(SSDTest, Write_InvalidSize1) {
	int address = 10;
	string data = "0x1234";
	EXPECT_CALL(mock, write(address, data)).Times(0);
	ssd.write(address, data);
}

TEST_F(SSDTest, Write_InvalidSize2) {
	int address = 10;
	string data = "0x1234567890";
	EXPECT_CALL(mock, write(address, data)).Times(0);
	ssd.write(address, data);
}

TEST_F(SSDTest, Write_InvalidData1) {
	int address = 10;
	string data = "1234567890";
	EXPECT_CALL(mock, write(address, data)).Times(0);
	ssd.write(address, data);
}

TEST_F(SSDTest, Write_InvalidData2) {
	int address = 10;
	string data = "0xzzzzzzzz";
	EXPECT_CALL(mock, write(address, data)).Times(0);
	ssd.write(address, data);
}

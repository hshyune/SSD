#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../SSD/SSD.cpp"

using namespace std;
using namespace testing;

class IoInterfaceMock : public IIoInterface {
public:
	MOCK_METHOD(string, read, (int address), (override));
	MOCK_METHOD(void, write, (int address, const string& data), (override));
	MOCK_METHOD(void, erase, (int address, int size), (override));
	MOCK_METHOD(void, flush, (), (override));
};

class SSDTest : public testing::Test {
public:
	IoInterfaceMock mock;
	SSD ssd{ &mock };
};


TEST_F(SSDTest, Read_Success) {
	int address = 1;
	string expectedData = "0x12345678";
	EXPECT_CALL(mock, read(address)).Times(1).WillRepeatedly(Return(expectedData));
	string ret = ssd.read(address);
	EXPECT_EQ(ret, expectedData);
}

TEST_F(SSDTest, Read_OutOfBound) {
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

TEST_F(SSDTest, Erase_Success) {
	int address = 10;
	int size = 1;
	EXPECT_CALL(mock, erase(address, size)).Times(1);
	ssd.erase(address, size);
}

TEST_F(SSDTest, Erase_OutOfBound) {
	int address = 200;
	int size = 1;
	EXPECT_CALL(mock, erase(address, size)).Times(0);
	ssd.erase(address, size);
}

TEST_F(SSDTest, Erase_InvalidSize) {
	int address = 10;
	int size = 0;
	EXPECT_CALL(mock, erase(address, size)).Times(0);
	ssd.erase(address, size);
}

TEST_F(SSDTest, Erase_InvalidSize2) {
	int address = 10;
	int size = 100;
	EXPECT_CALL(mock, erase(address, size)).Times(0);
	ssd.erase(address, size);
}

TEST_F(SSDTest, Erase_InvalidSize3) {
	int address = 10;
	int size = -1;
	EXPECT_CALL(mock, erase(address, size)).Times(0);
	ssd.erase(address, size);
}

TEST_F(SSDTest, Erase_InvalidSize4) {
	int address = 10;
	int size = 11;
	EXPECT_CALL(mock, erase(address, size)).Times(0);
	ssd.erase(address, size);
}

TEST_F(SSDTest, Flush_Success) {
	EXPECT_CALL(mock, flush()).Times(1);
	ssd.flush();
}
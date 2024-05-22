#include "pch.h"
#include "../SSD/SSD.cpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(SSD_READ, TEST1) {
	SSD* ssd = new SSD();
	int address = 1;
	int ret = ssd->read(1);
	EXPECT_EQ(ret, 0);
}
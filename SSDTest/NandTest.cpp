#include "../SSD/Nand.cpp"
#include <gtest/gtest.h>
#include <fstream>
#include <string>

class NandTest : public ::testing::Test {
protected:
    std::string testFileName = "test_nand.txt";
    Nand nand;

    void SetUp() override {
        nand = Nand(testFileName);
        std::ofstream file(testFileName);
        file.close();
    }

    void TearDown() override {
        std::remove(testFileName.c_str());
    }
};
TEST_F(NandTest, ReadWithoutWrite) {
    EXPECT_EQ(nand.read(1), "");
    EXPECT_EQ(nand.read(2), "");
    EXPECT_EQ(nand.read(3), "");
}

TEST_F(NandTest, WriteAndRead) {
    nand.write(1, "hello");
    nand.write(2, "world");
    EXPECT_EQ(nand.read(1), "hello");
    EXPECT_EQ(nand.read(2), "world");
    EXPECT_EQ(nand.read(3), "");
}

TEST_F(NandTest, OverwriteAndRead) {
    nand.write(1, "hello");
    nand.write(2, "world");
    nand.write(2, "korea");
    EXPECT_EQ(nand.read(1), "hello");
    EXPECT_EQ(nand.read(2), "korea");
    EXPECT_EQ(nand.read(3), "");
}

TEST_F(NandTest, LoadMapFromFile) {
    std::ofstream file(testFileName);
    file << "1,hello\n2,world\n";
    file.close();

    auto dataMap = nand.LoadMapFromFile();
    EXPECT_EQ(dataMap[1], "hello");
    EXPECT_EQ(dataMap[2], "world");
}

TEST_F(NandTest, SaveMapToFile) {
    std::map<int, std::string> dataMap = { {1, "hello"}, {2, "world"} };
    nand.SaveMapToFile(dataMap);

    std::ifstream file(testFileName);
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "1,hello");
    std::getline(file, line);
    EXPECT_EQ(line, "2,world");
    file.close();
}
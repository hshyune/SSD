#include "gtest/gtest.h"
#include "../SSD/CommandBuffer.cpp"

class CommandBufferTest : public ::testing::Test {
protected:
    std::string testFileName = "test_command_buffer.txt";
    CommandBuffer* cb;

    void SetUp() override {
        cb = new CommandBuffer(testFileName);
        std::ofstream file(testFileName);
        file.close();
    }

    void TearDown() override {
        delete cb;
        std::remove(testFileName.c_str());
    }
};

TEST_F(CommandBufferTest, ReadTest) {
    cb->write(10, "data");
    string result = cb->read(10);
    EXPECT_EQ(result, "data");
}

TEST_F(CommandBufferTest, WriteTest) {
    cb->write(10, "data");
    string result = cb->read(10);
    EXPECT_EQ(result, "data");
}

TEST_F(CommandBufferTest, EraseTest) {
    cb->write(10, "data");
    cb->erase(10, 1);
    string result = cb->read(10);
    EXPECT_EQ(result, "0x00000000");
}

TEST_F(CommandBufferTest, LoadTest) {
    cb->write(10, "data");

    list<Command> commands = cb->LoadFromFile();
    EXPECT_EQ(commands.size(), 1);
    EXPECT_EQ(commands.front().type, 'W');
    EXPECT_EQ(commands.front().address, 10);
    EXPECT_EQ(commands.front().data, "data");
}

TEST_F(CommandBufferTest, FlushTest_1) {
    cb->write(10, "data");
    cb->flush();
    
    remove(testFileName.c_str()); 
    
    string result = cb->read(10);
    EXPECT_EQ(result, "data");
}

TEST_F(CommandBufferTest, FlushTest_2) {
    cb->write(10, "data");
    
    remove(testFileName.c_str());
    
    string result = cb->read(10);
    EXPECT_EQ(result, "0x00000000");
}

TEST_F(CommandBufferTest, FlushTest_3) {
    cb->write(10, "data");
    cb->write(11, "data");
    cb->erase(10, 1);
    cb->flush();

    remove(testFileName.c_str());

    string result1 = cb->read(10);
    EXPECT_EQ(result1, "0x00000000");

    string result2 = cb->read(11);
    EXPECT_EQ(result2, "data");
}

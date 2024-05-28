#include "gtest/gtest.h"
#include "../SSD/CommandBuffer.cpp"
#include "../SSD/Nand.h"

class CommandBufferTest : public ::testing::Test {
protected:
    std::string testCommandBufferFileName = "test_command_buffer.txt";
    CommandBuffer* cb;

    std::string testNandFileName = "test_nand.txt";
    IIoInterface* nand;

    void SetUp() override {
        nand = new Nand(testNandFileName);
        cb = new CommandBuffer(nand, testCommandBufferFileName);
        std::ofstream file(testCommandBufferFileName);
        file.close();
    }

    void TearDown() override {
        delete cb;
        std::remove(testCommandBufferFileName.c_str());
        delete nand;
        std::remove(testNandFileName.c_str());
    }
};

TEST_F(CommandBufferTest, ReadTestIfWriteIsInBuffer) {
    cb->write(10, "data");
    string result = cb->read(10);
    EXPECT_EQ(result, "data");
}


TEST_F(CommandBufferTest, ReadTestIfEraseIsInBuffer) {
    cb->write(10, "data1");
    cb->write(10, "data2");
    cb->write(11, "data3");
    cb->erase(11, 2);
    EXPECT_EQ(cb->read(10), "data2");
    EXPECT_EQ(cb->read(11), "0x00000000");
    EXPECT_EQ(cb->read(12), "0x00000000");
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

TEST_F(CommandBufferTest, FlushTest_testIfDataExistsAfterFlush) {
    cb->write(10, "data");
    cb->write(11, "data");
    cb->erase(10, 1);
    cb->flush();

    remove(testCommandBufferFileName.c_str());

    EXPECT_EQ(cb->read(10), "0x00000000");
    EXPECT_EQ(cb->read(11), "data");
}

TEST_F(CommandBufferTest, FlushTest_testIfBufferFlushedWhenBufferIsFull_1) {
    const int MAX_BUFFER_SIZE = 10;
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
		cb->write(i, "data"  + std::to_string(i));
	}
    EXPECT_EQ(cb->getBufferSize(), MAX_BUFFER_SIZE);
    cb->write(MAX_BUFFER_SIZE, "data" + std::to_string(MAX_BUFFER_SIZE));
    EXPECT_EQ(cb->getBufferSize(), 1);
    for (int i = 0; i <= MAX_BUFFER_SIZE; i++) {
        EXPECT_EQ(cb->read(i), "data" + std::to_string(i));
    }
}


TEST_F(CommandBufferTest, FlushTest_testIfBufferFlushedWhenBufferIsFull_2) {
    const int MAX_BUFFER_SIZE = 10;
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        cb->erase(i*3+1, 1);
    }
    EXPECT_EQ(cb->getBufferSize(), MAX_BUFFER_SIZE);
    cb->erase(MAX_BUFFER_SIZE*3 +1, 1);
    EXPECT_EQ(cb->getBufferSize(), 1);
}

// If a later 'write' command has the same address as an earlier one, remove the earlier one
TEST_F(CommandBufferTest, OptimizeBufferTest_WriteRemove) {
    cb->write(10, "data1");
    cb->write(10, "data2");
    EXPECT_EQ(cb->getBufferSize(), 1);
    string result = cb->read(10);
    EXPECT_EQ(result, "data2");
}

// If an 'erase' command includes the address of an earlier 'write' command, remove the 'write' command
TEST_F(CommandBufferTest, OptimizeBufferTest_EraseRemove) {
    cb->write(10, "data");
    cb->erase(10, 1);
    EXPECT_EQ(cb->getBufferSize(), 1);
    string result = cb->read(10);
    EXPECT_EQ(result, "0x00000000");
}

// If a 'write' command falls within the range of an earlier 'erase' command, adjust the 'erase' command
TEST_F(CommandBufferTest, OptimizeBufferTest_EraseRangeAdjust) {
    cb->erase(10, 2);
    cb->write(10, "data");
    EXPECT_EQ(cb->getBufferSize(), 2);
    string result1 = cb->read(10);
    string result2 = cb->read(11);
    EXPECT_EQ(result1, "data");
    EXPECT_EQ(result2, "0x00000000");
}

// If two 'erase' commands have overlapping or contiguous ranges, merge them into one, unless there is a 'write' command in between
TEST_F(CommandBufferTest, OptimizeBufferTest_EraseMerge) {
    cb->erase(10, 2);
    cb->erase(12, 2);
    cb->write(11, "data");
    EXPECT_EQ(cb->getBufferSize(), 2);
    string result1 = cb->read(10);
    string result2 = cb->read(11);
    string result3 = cb->read(12);
    string result4 = cb->read(13);
    EXPECT_EQ(result1, "0x00000000");
    EXPECT_EQ(result2, "data");
    EXPECT_EQ(result3, "0x00000000");
    EXPECT_EQ(result4, "0x00000000");
}


TEST_F(CommandBufferTest, OptimizeBufferTest_Scenario1) {
    cb->write(20, "data1");
    cb->write(21, "data2");
    cb->write(20, "data3");
    EXPECT_EQ(cb->getBufferSize(), 2);
    string result1 = cb->read(20);
    string result2 = cb->read(21);
    EXPECT_EQ(result1, "data3");
    EXPECT_EQ(result2, "data2");
}

TEST_F(CommandBufferTest, OptimizeBufferTest_Scenario2) {
    cb->write(20, "data1");
    cb->write(21, "data2");
    cb->erase(18, 5);
    EXPECT_EQ(cb->getBufferSize(), 1);
    string result1 = cb->read(20);
    string result2 = cb->read(21);
    EXPECT_EQ(result1, "0x00000000");
    EXPECT_EQ(result2, "0x00000000");
}

TEST_F(CommandBufferTest, OptimizeBufferTest_Scenario3) {
    cb->write(20, "data1");
    cb->erase(10, 2);
    cb->erase(12, 3);
    list<Command> buffer = cb->LoadFromFile();
    EXPECT_EQ(buffer.size(), 2);
    Command secondCmd = *next(buffer.begin());
    EXPECT_EQ(secondCmd.type, 'E');
    EXPECT_EQ(secondCmd.address, 10);
    EXPECT_EQ(secondCmd.data, "5");
    EXPECT_EQ(cb->read(20), "data1");
}

TEST_F(CommandBufferTest, OptimizeBufferTest_Scenario4_1) {
    
    cb->erase(10, 4);
    cb->erase(40, 5);
    cb->write(12, "data1");
    cb->write(13, "data2");
    list<Command> buffer = cb->LoadFromFile();
    EXPECT_EQ(buffer.size(), 4);
    Command firstCmd = *(buffer.begin());

    EXPECT_EQ(firstCmd.type, 'E');
    EXPECT_EQ(firstCmd.address, 10);
    EXPECT_EQ(firstCmd.data, "2");
    EXPECT_EQ(cb->read(12), "data1");
    EXPECT_EQ(cb->read(13), "data2");
}

TEST_F(CommandBufferTest, OptimizeBufferTest_Scenario4_2) {

    cb->erase(50, 1);
    cb->erase(40, 5);
    cb->write(50, "data1");
    list<Command> buffer = cb->LoadFromFile();
    EXPECT_EQ(buffer.size(), 2);
    Command firstCmd = *(buffer.begin());

    EXPECT_EQ(firstCmd.type, 'E');
    EXPECT_EQ(firstCmd.address, 40);
    EXPECT_EQ(firstCmd.data, "5");
    EXPECT_EQ(cb->read(50), "data1");
}

TEST_F(CommandBufferTest, OptimizeBufferTest_Scenario5) {

    cb->erase(10, 2);
    cb->write(10, "data1");
    cb->erase(12, 3);
    list<Command> buffer = cb->LoadFromFile();
    EXPECT_EQ(buffer.size(), 2);
    Command secondCmd = *next(buffer.begin());
    EXPECT_EQ(secondCmd.type, 'E');
    EXPECT_EQ(secondCmd.address, 11);
    EXPECT_EQ(secondCmd.data, "4");
    EXPECT_EQ(cb->read(10), "data1");
}

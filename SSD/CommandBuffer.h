﻿#pragma once
#include "IIoInterface.h"
#include <list>
#include <string>

using namespace std;

struct Command {
    char type;
    int address;
    string data;
};

class CommandBuffer : public IIoInterface {
public:
    CommandBuffer(IIoInterface* storage, const string& fileName = "buffer.txt");
    virtual ~CommandBuffer() {
	}
    string read(int address) override;
    void write(int address, const string& data) override;
    void erase(int address, int size) override;
    void flush(void) override;
    list<Command> LoadFromFile();
    
    size_t getBufferSize();

private:
    string fileName;
    IIoInterface* storage = nullptr;

    void saveToFile(const list<Command>& commandBuffer);
    void ignoreWriteAfterWrite(list<Command>& commandBuffer);
    void ignoreWriteAfterErase(list<Command>& commandBuffer);
    void optimizeBuffer(list<Command>& commandBuffer);
    void mergeErase(list<Command>& commandBuffer);
    void narrowRangeOfErase(list<Command>& commandBuffer);
};


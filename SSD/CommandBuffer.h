#pragma once
#include "IIoInterface.h"
#include <list>
#include <string>
#include "Nand.h"

using namespace std;

struct Command {
    char type;
    int address;
    string data;
};

class CommandBuffer : public IIoInterface {
public:
    CommandBuffer(const string& fileName = "buffer.txt");
    virtual ~CommandBuffer() {
		delete nand;
	}
    string read(int address) override;
    void write(int address, const string& data) override;
    void erase(int address, int size) override;
    void flush(void) override;
    list<Command> LoadFromFile();
    void SaveToFile(const list<Command>& commandBuffer);

private:
    string fileName;
    Nand* nand = nullptr;
};


#pragma once
#include "IIoInterface.h"
#include <map>
#include <string>
#include "Nand.h"

using namespace std;

class CommandBuffer : public IIoInterface {
public:
    CommandBuffer(const string& fileName = "buffer.txt");
    virtual ~CommandBuffer() {
		delete nand;
	}
    string read(int address) override;
    void write(int address, const string& data) override;
    void erase(int address, int size) override;
    map<int, string> LoadMapFromFile();
    void SaveMapToFile(map<int, string> data);

private:
    string fileName;
    Nand* nand = nullptr;
};


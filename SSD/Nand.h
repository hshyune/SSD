#pragma once
#include "IIoInterface.h"
#include <map>
#include <string>

using namespace std;

class Nand : public IIoInterface {
public:
    Nand(const string& fileName = "nand.txt");
    string read(int address) override;
    void write(int address, const string& data) override;
    void erase(int address, int size) override;
    void flush(void) override;
    map<int, string> LoadMapFromFile();
    void SaveMapToFile(map<int, string> data);

private:
    string fileName;
};


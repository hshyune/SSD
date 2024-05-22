#pragma once
#include "IIoInterface.h"
#include <map>
#include <string>

using namespace std;

class Nand : public IIoInterface {
public:
	string read(int address) override;
	void write(int address, const string& data) override;

private:
	map<int, string> LoadMapFromFile();
	void SaveMapToFile(map<int, string> data);
	const char* fileName = "nand.txt";
};


#pragma once
#include "IIoInterface.h"

#include <string>

class SSD {
public:
	SSD(IIoInterface* ioInterface);
	std::string read(int address);
	void write(int address, const std::string& data);

private:
	IIoInterface* ioInterface;
};
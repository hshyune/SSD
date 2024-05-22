#pragma once
#include "IIoInterface.h"
#include "ISSDInterface.h"

#include <string>

class SSD : public ISSDInterface {
public:
	SSD(IIoInterface* ioInterface);
	std::string read(int address) override;
	void write(int address, const std::string& data) override;

private:
	IIoInterface* ioInterface;
};
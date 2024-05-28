#pragma once
#include "IIoInterface.h"
#include "ISSDInterface.h"

#include <string>

constexpr unsigned int MAX_LBA_COUNT = 100;

class SSD : public ISSDInterface {
public:
	SSD(IIoInterface* ioInterface);
	std::string read(int address) override;
	void write(int address, const std::string& data) override;
	void erase(int address, int size) override;
	void flush(void) override;

private:
	IIoInterface* ioInterface;
};
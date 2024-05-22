#pragma once
#include "IIoInterface.h"

class SSD {
public:
	SSD(IIoInterface* ioInterface);
	int read(int address);
	void write(int address, int data);

private:
	IIoInterface* ioInterface;
};
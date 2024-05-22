#pragma once
#include "IIoInterface.h"
class Nand : public IIoInterface {
public:
	int read(int address) override;
	void write(int address, int data) override;
};


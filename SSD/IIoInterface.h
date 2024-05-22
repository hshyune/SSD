#pragma once

class IIoInterface {
public:
	virtual int read(int address) = 0;
	virtual void write(int address, int data) = 0;
};

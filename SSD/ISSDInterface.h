#pragma once

#include <string>

class ISSDInterface {
public:
	virtual std::string read(int address) = 0;
	virtual void write(int address, const std::string& data) = 0;
	virtual void erase(int address, int size) = 0;
};

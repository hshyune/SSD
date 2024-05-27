#include "SSD.h"
#include <regex>

SSD::SSD(IIoInterface* ioInterface) {
	this->ioInterface = ioInterface;
}
std::string SSD::read(int address) {
	if (address < 0 || address >= MAX_LBA_COUNT) {
		return "";
	}

	return ioInterface->read(address);
}
void SSD::write(int address, const std::string& data) {
	if (address < 0 || address >= MAX_LBA_COUNT) {
		return;
	}

	std::regex pattern("^0x[0-9A-F]{8}$");
	if (!std::regex_match(data, pattern)) {
		return;
	}

	ioInterface->write(address, data);
}

void SSD::erase(int address, int size) {
	if (address < 0 || address + size >= MAX_LBA_COUNT) {
		return;
	}

	ioInterface->erase(address, size);
}

void SSD::flush(void) {
	ioInterface->flush();
}
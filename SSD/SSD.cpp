#include "SSD.h"

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

	if (data.size() != 10) {
		return;
	}

	if (data.substr(0, 2) != "0x") {
		return;
	}

	for (int index = 2; index < 10; index++) {
		char value = data[index];
		if (value >= '0' && value <= '9') {
			continue;
		}

		if (value >= 'A' && value <= 'F') {
			continue;
		}

		return;
	}

	ioInterface->write(address, data);
}

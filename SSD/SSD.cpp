#include "SSD.h"

SSD::SSD(IIoInterface* ioInterface) {
	this->ioInterface = ioInterface;
}
std::string SSD::read(int address) {
	return ioInterface->read(address);
}
void SSD::write(int address, const std::string& data) {
	ioInterface->write(address, data);
}

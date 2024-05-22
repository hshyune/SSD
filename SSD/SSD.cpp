#include "SSD.h"

SSD::SSD(IIoInterface* ioInterface) {
	this->ioInterface = ioInterface;
}
int SSD::read(int address) {
	return ioInterface->read(address);
}
void SSD::write(int address, int data) {
	ioInterface->write(address, data);
}

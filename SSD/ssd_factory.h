#pragma once
#include "IIoInterface.h"
#include "ISSDInterface.h"
#include "SSD.h"
#include "Nand.h"

static ISSDInterface* GetNandSSD() {
	return new SSD(new Nand());
}
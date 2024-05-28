﻿#pragma once
#include "IIoInterface.h"
#include "ISSDInterface.h"
#include "SSD.h"
#include "CommandBuffer.h"

class SSDFactory {
public:
	static ISSDInterface* CreateNandSSD() {
		return new SSD(new CommandBuffer(new Nand()));

	}

};
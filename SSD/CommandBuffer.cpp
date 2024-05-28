#include "CommandBuffer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

CommandBuffer::CommandBuffer(IIoInterface* storage, const string& fileName) : fileName(fileName) {
	this->storage = storage;
}

string CommandBuffer::read(int address) {
	ifstream file(fileName, ios::binary);

	auto commandBuffer = LoadFromFile();

	for (const auto& command : commandBuffer) {
		if (command.type == 'W' && command.address == address) {
			return command.data;
		}
	}

	return storage->read(address);
}

bool isInRange(int target, int start, int end) {
	return (start <= target && target < end) ? true : false;
}

void CommandBuffer::ignoreWriteAfterWrite(list<Command>& commandBuffer) {
	auto lastCmd = &commandBuffer.back();
	for (auto iter = commandBuffer.begin(); iter != std::prev(commandBuffer.end()); ) {
		if (iter->type == 'W' && iter->address == lastCmd->address) {
			iter = commandBuffer.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void CommandBuffer::ignoreWriteAfterErase(list<Command>& commandBuffer) {
	auto lastCmd = &commandBuffer.back();
	for (auto iter = commandBuffer.begin(); iter != std::prev(commandBuffer.end());) {
		if (iter->type == 'W' && isInRange(iter->address, lastCmd->address, lastCmd->address + stoi(lastCmd->data))) {
			iter = commandBuffer.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void CommandBuffer::mergeErase(list<Command>& commandBuffer) {
	auto lastCmd = &commandBuffer.back();
	for (auto iter = commandBuffer.begin(); iter != std::prev(commandBuffer.end()); ) {
		if (iter->type == 'E' && ((iter->address <= lastCmd->address && lastCmd->address <= iter->address + stoi(iter->data)) ||
			(lastCmd->address <= iter->address && iter->address <= lastCmd->address + stoi(lastCmd->data)))) {
			lastCmd->data = to_string(max(iter->address + stoi(iter->data), lastCmd->address + stoi(lastCmd->data)) - iter->address);
			lastCmd->address = min(iter->address, lastCmd->address);
			iter = commandBuffer.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void CommandBuffer::narrowRangeOfErase(list<Command>&commandBuffer) {
	for (auto iterWrite = commandBuffer.begin(); iterWrite != commandBuffer.end(); ) {
		if (iterWrite->type == 'W') {
			for (auto iterErase = commandBuffer.begin(); iterErase != iterWrite; ) {
				if (iterErase->type == 'E' && isInRange(iterWrite->address, iterErase->address, iterErase->address + stoi(iterErase->data))) {
					if (stoi(iterErase->data) == 1) {
						iterErase = commandBuffer.erase(iterErase);
						narrowRangeOfErase(commandBuffer);
					}
					else if (iterErase->address == iterWrite->address) {
						iterErase->address++;
						iterErase++;
						narrowRangeOfErase(commandBuffer);
					}
					else if (iterErase->address + stoi(iterErase->data) - 1 == iterWrite->address) {
						iterErase->data = to_string(stoi(iterErase->data) - 1);
						iterErase++;
						narrowRangeOfErase(commandBuffer);
					}
					else {
						iterErase++;
					}
				}
				else {
					iterErase++;
				}
			}
		}
		iterWrite++;
	}
}


void CommandBuffer::optimizeBuffer(list<Command>& commandBuffer) {
	auto lastCmd = &commandBuffer.back();
	if (lastCmd->type == 'W') {
		ignoreWriteAfterWrite(commandBuffer);
		narrowRangeOfErase(commandBuffer);
	}
	else {
		ignoreWriteAfterErase(commandBuffer);
		mergeErase(commandBuffer);
	}
}

void CommandBuffer::write(int address, const string& data) {
	auto commandBuffer = LoadFromFile();

	if (getBufferSize() >= 10) {
		flush();
		commandBuffer = LoadFromFile();
	}

	Command c;
	c.type = 'W';
	c.address = address;
	c.data = data;

	commandBuffer.push_back(c);
	optimizeBuffer(commandBuffer);

	saveToFile(commandBuffer);
}

void CommandBuffer::erase(int address, int size) {
	auto commandBuffer = LoadFromFile();

	if (getBufferSize() >= 10) {
		flush();
		commandBuffer = LoadFromFile();
	}

	Command c;
	c.type = 'E';
	c.address = address;
	c.data = to_string(size);

	commandBuffer.push_back(c);
	optimizeBuffer(commandBuffer);

	saveToFile(commandBuffer);
}

void CommandBuffer::flush(void) {
	auto commandBuffer = LoadFromFile();

	list<Command>::iterator iter = commandBuffer.begin();
	for (auto iter = commandBuffer.begin(); iter != commandBuffer.end(); iter++) {
		switch (iter->type) {
		case 'W':
			storage->write(iter->address, iter->data);
			break;
		case 'E':
			storage->erase(iter->address, stoi(iter->data));
			break;
		}
	}

	commandBuffer.clear();
	saveToFile(commandBuffer);
}

size_t CommandBuffer::getBufferSize() {
	auto commandBuffer = LoadFromFile();
	cout << "Buffer size: " << commandBuffer.size() << endl;
	return commandBuffer.size();
}

list<Command> CommandBuffer::LoadFromFile()
{
	ifstream file(fileName, ios::binary);

	list<Command> commandBuffer;

	if (!file) {
		return commandBuffer;
	}

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		string typeStr, addressStr, data;

		if (!getline(iss, typeStr, ',') ||
			!getline(iss, addressStr, ',') ||
			!getline(iss, data, ',')) {
			break;
		}

		char type = typeStr[0];
		int address = stoi(addressStr);

		Command c;
		c.type = type;
		c.address = address;
		c.data = data;
		commandBuffer.push_back(c);
	}

	file.close();

	return commandBuffer;
}

void CommandBuffer::saveToFile(const list<Command>& commandBuffer)
{
	// always overwrite the file with the new data (ios::trunc)
	ofstream file(fileName, ios::binary | ios::trunc);

	if (!file) {
		cerr << "Savefile failed: File could not be opened\n";
		return;
	}

	if (commandBuffer.size() == 0) {
		file.close();
		remove(fileName.c_str());
	}
	else {
		for (auto& command : commandBuffer) {
			file << command.type << "," << command.address << "," << command.data << "\n";
		}
	}

	file.close();
}
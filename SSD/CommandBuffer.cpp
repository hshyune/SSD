#include "CommandBuffer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

CommandBuffer::CommandBuffer(Nand* nand, const string& fileName) : fileName(fileName) {
	this->nand = nand;
}

string CommandBuffer::read(int address) {
	ifstream file(fileName, ios::binary);

	auto commandBuffer = LoadFromFile();

	for (const auto& command : commandBuffer) {
		if (command.type == 'W' && command.address == address) {
			return command.data;
		}
	}

	return nand->read(address);
}

void CommandBuffer::optimizeBuffer(list<Command>& commandBuffer) {
	if (commandBuffer.size() < 2) {
		return;
	}
	for (auto earlierCmd = commandBuffer.begin(); earlierCmd != commandBuffer.end(); ++earlierCmd) {
		for (auto laterCmd = next(earlierCmd); laterCmd != commandBuffer.end(); ++laterCmd) {
			if (earlierCmd->type == 'W' && laterCmd->type == 'W' && earlierCmd->address == laterCmd->address) {
				commandBuffer.erase(earlierCmd);
				optimizeBuffer(commandBuffer);
				return;
			}
			else if (earlierCmd->type == 'W' && laterCmd->type == 'E' && laterCmd->address <= earlierCmd->address && earlierCmd->address < laterCmd->address + stoi(laterCmd->data)) {
				commandBuffer.erase(earlierCmd);
				optimizeBuffer(commandBuffer);
				return;
			}
			else if (earlierCmd->type == 'E' && laterCmd->type == 'W' && earlierCmd->address <= laterCmd->address && laterCmd->address < earlierCmd->address + stoi(earlierCmd->data)) {
				if (stoi(earlierCmd->data) == 1) {
					commandBuffer.erase(earlierCmd);
					optimizeBuffer(commandBuffer);
					return;
				}
				else if (earlierCmd->address == laterCmd->address) {
					earlierCmd->address++;
					optimizeBuffer(commandBuffer);
					return;
				}
				else if (earlierCmd->address + stoi(earlierCmd->data) - 1 == laterCmd->address) {
					earlierCmd->data = to_string(stoi(earlierCmd->data) - 1);
					optimizeBuffer(commandBuffer);
					return;
				}
			}
			else if (earlierCmd->type == 'E' && laterCmd->type == 'E') {
				if ((earlierCmd->address <= laterCmd->address && laterCmd->address <= earlierCmd->address + stoi(earlierCmd->data)) ||
					(laterCmd->address <= earlierCmd->address && earlierCmd->address <= laterCmd->address + stoi(laterCmd->data))) {
					bool canMerge = true;
					for (int curAddr = laterCmd->address; curAddr < laterCmd->address + stoi(laterCmd->data); curAddr++) {
						if (any_of(commandBuffer.begin(), commandBuffer.end(), [curAddr](const Command& cmd) { return cmd.type == 'W' && cmd.address == curAddr; })) {
							canMerge = false;
							break;
						}
					}

					if (canMerge) {
						earlierCmd->address = min(earlierCmd->address, laterCmd->address);
						earlierCmd->data = to_string(max(earlierCmd->address + stoi(earlierCmd->data), laterCmd->address + stoi(laterCmd->data)) - earlierCmd->address);
						commandBuffer.erase(laterCmd);
						optimizeBuffer(commandBuffer);
						return;
					}
				}
			}
		}
	}
}

void CommandBuffer::write(int address, const string& data) {
	auto commandBuffer = LoadFromFile();

	Command c;
	c.type = 'W';
	c.address = address;
	c.data = data;

	commandBuffer.push_back(c);

	optimizeBuffer(commandBuffer);

	saveToFile(commandBuffer);
	if (getBufferSize() >= 10) {
		flush();
	}
}

void CommandBuffer::erase(int address, int size) {
	auto commandBuffer = LoadFromFile();

	Command c;
	c.type = 'E';
	c.address = address;
	c.data = to_string(size);

	commandBuffer.push_back(c);

	optimizeBuffer(commandBuffer);

	saveToFile(commandBuffer);
	if (getBufferSize() >= 10) {
		flush();
	}
}

void CommandBuffer::flush(void) {
	auto commandBuffer = LoadFromFile();

	list<Command>::iterator iter = commandBuffer.begin();
	for (auto iter = commandBuffer.begin(); iter != commandBuffer.end(); iter++) {
		switch (iter->type) {
		case 'W':
			nand->write(iter->address, iter->data);
			break;
		case 'E':
			nand->erase(iter->address, stoi(iter->data));
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
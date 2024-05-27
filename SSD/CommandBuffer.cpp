#include "CommandBuffer.h"
#include <fstream>
#include <sstream>
#include <iostream>

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

void CommandBuffer::write(int address, const string& data) {
	auto commandBuffer = LoadFromFile();

	list<Command>::iterator iter = commandBuffer.begin();
	for (auto iter = commandBuffer.begin(); iter != commandBuffer.end(); ) {
		if (iter->type == 'W' && iter->address == address) {
			iter = commandBuffer.erase(iter);
		}
		else {
			++iter;
		}
	}

	Command c;
	c.type = 'W';
	c.address = address;
	c.data = data;
	
	commandBuffer.push_back(c);

	SaveToFile(commandBuffer);
}

void CommandBuffer::erase(int address, int size) {
	auto commandBuffer = LoadFromFile();

	// delete write command in erase range
	for (int curAddr = address; curAddr < address + size; curAddr++) {
		list<Command>::iterator iter = commandBuffer.begin();
		for (iter = commandBuffer.begin(); iter != commandBuffer.end(); ) {
			if (iter->type == 'W' && iter->address == address) {
				iter = commandBuffer.erase(iter);
			}
			else {
				iter++;
			}
		}
	}
	
	// todo: merge erase

	// add erase command to commandBuffer
	Command c;
	c.type = 'E';
	c.address = address;
	c.data = to_string(size);

	commandBuffer.push_back(c);

	SaveToFile(commandBuffer);
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
	SaveToFile(commandBuffer);
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

void CommandBuffer::SaveToFile(const list<Command>& commandBuffer)
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
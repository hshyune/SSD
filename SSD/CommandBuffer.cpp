#include "CommandBuffer.h"
#include <fstream>
#include <sstream>
#include <iostream>

CommandBuffer::CommandBuffer(const string& fileName) : fileName(fileName) {
	nand = new Nand();
}

string CommandBuffer::read(int address) {
	ifstream file(fileName, ios::binary);

	auto commandBuffer = LoadFromFile();

	list<Command>::iterator iter = commandBuffer.begin();
	for(iter= commandBuffer.begin();iter!=commandBuffer.end();iter++) {
		if (iter->type == 'W' && iter->address == address) {
			return iter->data;
		}
	}

	return nand->read(address);
}

void CommandBuffer::write(int address, const string& data) {
	auto commandBuffer = LoadFromFile();

	list<Command>::iterator iter = commandBuffer.begin();
	for (iter = commandBuffer.begin(); iter != commandBuffer.end(); iter++) {
		if (iter->type == 'W' && iter->address == address) {
			iter = commandBuffer.erase(iter);
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
	for (int curAddr = address; curAddr < address + size - 1; curAddr++) {
		list<Command>::iterator iter = commandBuffer.begin();
		for (iter = commandBuffer.begin(); iter != commandBuffer.end(); iter++) {
			if (iter->type == 'W' && iter->address == address) {
				iter = commandBuffer.erase(iter);
			}
		}
	}
	
	// todo: merge erase

	// add erase command to commandBuffer
	Command c;
	c.type = 'E';
	c.address = address;
	c.data = size;

	commandBuffer.push_back(c);

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
		string keyStr, value;

		char type;
		int address;
		string data;

		// need to parse file


		Command c;
		c.type = type;
		c.address = address;
		c.data = data;
		commandBuffer.push_back(c);
	}

	file.close();

	return commandBuffer;
}

void CommandBuffer::SaveToFile(list<Command> commandBuffer)
{
	// always overwrite the file with the new data (ios::trunc)
	ofstream file(fileName, ios::binary | ios::trunc);

	if (!file) {
		cerr << "Savefile failed: File could not be opened\n";
		return;
	}

	for (auto& command : commandBuffer) {
		file << command.type << "," << command.address << "," << command.size << "," << command.data << "\n";
	}

	file.close();
}
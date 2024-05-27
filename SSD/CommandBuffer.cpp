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

	int lastIndex = commandBuffer.size() - 1;
	for (int index = lastIndex; index >= 0; index--) {
		if (commandBuffer[index].type == 'W' && commandBuffer[index].address == address) {
			return commandBuffer[index].data;
		}
	}

	return nand->read(address);
}

void CommandBuffer::write(int address, const string& data) {
	auto commandBuffer = LoadFromFile();

	for (auto command : commandBuffer) {
		if (command.type == 'W' && command.address == address) {
			// erase entry
		}
	}

	Command c;
	c.type = 'W';
	c.address = address;
	c.size = 1;
	c.data = data;
	
	commandBuffer.push_back(c);

	SaveToFile(commandBuffer);
}

void CommandBuffer::erase(int address, int size) {
	auto commandBuffer = LoadFromFile();

	// delete write, erase command in current address range

	SaveToFile(commandBuffer);
}

vector<Command> CommandBuffer::LoadFromFile()
{
	ifstream file(fileName, ios::binary);

	vector<Command> commandBuffer;

	if (!file) {
		return commandBuffer;
	}

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		string keyStr, value;

		char type;
		int address;
		int size;
		string data;

		// need to parse file

		Command c;
		c.type = type;
		c.address = address;
		c.size = size;
		c.data = data;
		commandBuffer.push_back(c);
	}

	file.close();

	return commandBuffer;
}

void CommandBuffer::SaveToFile(vector<Command> commandBuffer)
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
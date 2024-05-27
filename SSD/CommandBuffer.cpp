#include "CommandBuffer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

CommandBuffer::CommandBuffer(const string& fileName) : fileName(fileName) {
}

string CommandBuffer::read(int address) {
	ifstream file(fileName, ios::binary);

	map<int, string> dataMap = LoadMapFromFile();

	// If the address exists in the map, return the corresponding value
	if (dataMap.find(address) != dataMap.end()) {
		return dataMap[address];
	}

	// If the address does not exist in the map, return empty string
	return "";
}

void CommandBuffer::write(int address, const string& data) {
	map<int, string> dataMap = LoadMapFromFile();
	// update dataMap
	dataMap[address] = data;

	SaveMapToFile(dataMap);
}

map<int, string> CommandBuffer::LoadMapFromFile()
{
	ifstream file(fileName, ios::binary);

	map<int, string> dataMap;

	if (!file) {
		return dataMap;
	}

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		string keyStr, value;

		// Use getline to parse the key and value separated by a comma
		if (!getline(iss, keyStr, ',') || !(iss >> value)) {
			cerr << "Error parsing line: " << line << "\n";
			continue;
		}

		int key = stoi(keyStr);
		dataMap[key] = value;
	}

	file.close();

	return dataMap;
}

void CommandBuffer::SaveMapToFile(map<int, string> data)
{
	// always overwrite the file with the new data (ios::trunc)
	ofstream file(fileName, ios::binary | ios::trunc);

	if (!file) {
		cerr << "Savefile failed: File could not be opened\n";
		return;
	}

	for (auto& pair : data) {
		file << pair.first << "," << pair.second << "\n";
	}

	file.close();
}
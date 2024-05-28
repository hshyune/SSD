#pragma once
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>


using namespace std;

class SSDRunner {
public:
	SSDRunner() {
		this->ssdPath = "SSD.exe";
	}

	string read(int address) {
		string result = "";
		string cmd = ssdPath + " R " + to_string(address);
		result = this->exec(cmd);
		return result;
	}

	void write(int address, string data) {
		string cmd = ssdPath + " W " + to_string(address) + " " + data;
		this->exec(cmd);
	}

	string fullread() {
		string result = "";
		for (int addr = 0; addr < MAX_LBA_SIZE; addr++) {
			result += to_string(addr) + "," + this->read(addr);
		}
		return result;
	}

	void fullwrite(string data) {
		for (int addr = 0; addr < MAX_LBA_SIZE; addr++) {
			this->write(addr, data);
		}
	}

	void erase(int addr, int size) {
		string cmd = ssdPath + " E " + to_string(addr) + " " + to_string(size);
		this->exec(cmd);
	}

private:
	string ssdPath = "";
	const int MAX_LBA_SIZE = 100;

	string exec(const string cmdstr) {
		// string to char*
		char cmd[512];
		strcpy_s(cmd, cmdstr.c_str());
		// exec command
		char buffer[128];
		string result = "";
		FILE* pipe = _popen(cmd, "r");
		if (!pipe)
			throw runtime_error("popen() failed!");
		try {
			while (fgets(buffer, sizeof buffer, pipe) != NULL) {
				result += buffer;
			}
		}
		catch (...) {
			_pclose(pipe);
			throw;
		}
		_pclose(pipe);
		return result;
	}
};
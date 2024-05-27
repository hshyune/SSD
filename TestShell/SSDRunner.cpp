#include <stdio.h>
#include <iostream>
#include <cstdlib> 
#include <iostream>
#include <stdexcept>
#include <string> 

using namespace std;

class SSDRunner {
public:
	string read(int address) {
		string result = "";
		//string cmd = this->SSDPath + " R " + to_string(address);
		string cmd = "SSD.exe R " + to_string(address);
		result = this->exec(cmd);
		return result;
	}

	void write(int address, string data) {
		//string cmd = this->SSDPath + " W " + to_string(address) + " " + data;
		string cmd = "SSD.exe W " + to_string(address) + " " + data;
		this->exec(cmd);
	}

private:
	string SSDPath = "../TestShell/SSD.exe";
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
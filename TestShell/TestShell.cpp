#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "SSDRunner.cpp"
#include "TestLogger.h"
#include "Command.cpp"
using namespace std;

const string INVALID_PARAMETER = "INVALID PARAMETER";
const string INVALID_LBA_RANGE = "INVALID_LBA_RANGE";
const string INVALID_DATA = "INVALID_DATA";
const string INVALID_COMMAND = "INVALID COMMAND";

class TestShell {
public:
	TestShell() {
		this->ssdRunner = new SSDRunner();
	}
	~TestShell() {
	}
	void write(int address, string data) {
		ssd.write(address, data);
		this->logger.print("LBA: " + to_string(address) + " / DATA: " + data);
	}

	string read(int address) {
		string data = ssd.read(address);
		this->logger.print("LBA: " + to_string(address) + " / DATA: " + data);
		return data;
	}

	void exit() {
		this->logger.print("EXIT");
		this->isRunning = false;
	}

	int help() {
		int lineCnt = 0;

		ifstream file("help.txt");
		string line = "";
		if (file.is_open()) {
			while (getline(file, line)) {
				cout << line << endl;
				lineCnt += 1;
			}
			file.close();
		}
		else {
			cerr << "Unable to open help.txt!" << endl;
		}
		this->logger.print("HELP");
		return lineCnt;
	}

	void fullwrite(string data) {
		const int LBA_SIZE = 100;
		this->logger.print("LBA: ALL / DATA: " + data);
		for (int address = 0; address < LBA_SIZE; ++address) { // Address [0,99]
			this->write(address, data);
		}
		this->logger.print("DONE");
	}

	void fullread() {
		const int LBA_SIZE = 100;
		this->logger.print("LBA: ALL");
		string readData = "";
		for (int address = 0; address < LBA_SIZE; ++address) { // Address [0,99]
			readData += to_string(address) + "," + this->read(address);
		}
		cout << readData << endl;
		this->logger.print("DONE");
	}

	void run() {
		// run
		while (this->isRunning) {
			// input command
			string input = "";
			cout << "input : ";
			getline(cin, input);

			// validation
			// split string
			istringstream iss(input);
			string tmp;
			vector<string> args;
			while (getline(iss, tmp, ' ')) {
				args.push_back(tmp);
			}
			// empty input
			if (args.size() == 0) continue;

			// validation & execution
			string cmd = args.at(0);
			if (cmd == "read") {
				ReadCommand* command = new ReadCommand(this->ssdRunner);
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else if (cmd == "write") {
				WriteCommand* command = new WriteCommand(this->ssdRunner);
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else if (cmd == "exit") {
				ExitCommand* command = new ExitCommand();
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
					this->isRunning = false;
				}
				continue;
			}
			else if (cmd == "help") {
				HelpCommand* command = new HelpCommand();
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else if (cmd == "fullwrite") {
				FullwriteCommand* command = new FullwriteCommand(this->ssdRunner);
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else if (cmd == "fullread") {
				FullreadCommand* command = new FullreadCommand(this->ssdRunner);
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else if (cmd == "erase") {
				EraseCommand* command = new EraseCommand(this->ssdRunner);
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else if (cmd == "erase_range") {
				EraseRangeCommand* command = new EraseRangeCommand(this->ssdRunner);
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				else {
					continue;
				}
			}
			else {
				InvalidCommand* command = new InvalidCommand();
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				continue;
			}
		}
	}

private:
	SSDRunner ssd;
	bool isRunning = true;
	LoggerSingleton& logger{ LoggerSingleton::getInstance() };

	CommandContoller commandContoller;
	SSDRunner* ssdRunner;
};

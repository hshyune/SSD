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

class TestShell {
public:
	TestShell() {
		this->ssdRunner = new SSDRunner();
	}
	~TestShell() {
	}

	vector<string> getInput() {
		string input = "";
		cout << "input : ";
		getline(cin, input);

		// split string
		vector<string> args;
		istringstream iss(input);
		string tmp;
		while (getline(iss, tmp, ' ')) {
			args.push_back(tmp);
		}
		return args;
	}

	void run() {
		// run
		while (this->isRunning) {
			//// input command
			vector<string> args = this->getInput();
			
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

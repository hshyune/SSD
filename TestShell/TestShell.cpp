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

	void runCommand(Command* command, std::vector<std::string>& args)
	{
		commandContoller.setCommand(command);
		if (commandContoller.validate(args))
			commandContoller.execute();
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
			Command* command;
			if (cmd == "read") {
				command = new ReadCommand(this->ssdRunner);
			}
			else if (cmd == "write") {
				command = new WriteCommand(this->ssdRunner);
			}
			else if (cmd == "fullwrite") {
				command = new FullwriteCommand(this->ssdRunner);
			}
			else if (cmd == "fullread") {
				command = new FullreadCommand(this->ssdRunner);
			}
			else if (cmd == "erase") {
				command = new EraseCommand(this->ssdRunner);
			}
			else if (cmd == "erase_range") {
				command = new EraseRangeCommand(this->ssdRunner);
			}
			else if (cmd == "flush") {
				command = new FlushCommand(this->ssdRunner);
			}
			else if (cmd == "help") {
				command = new HelpCommand();
			}
			else if (cmd == "exit") {
				command = new ExitCommand();
				commandContoller.setCommand(command);
				if (commandContoller.validate(args)){
					this->isRunning = false;

					// flush and exit
					command = new FlushCommand(this->ssdRunner);
					commandContoller.setCommand(command);
					commandContoller.execute();
					continue;
				}
			}
			else {
				command = new InvalidCommand();
			}
			runCommand(command, args);
		}
	}

private:
	bool isRunning = true;
	LoggerSingleton& logger{ LoggerSingleton::getInstance() };

	CommandContoller commandContoller;
	SSDRunner* ssdRunner;
};

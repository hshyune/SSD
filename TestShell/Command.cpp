#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "SSDRunner.cpp"
#include "TestLogger.h"

using namespace std;

class Command {
public:
	virtual ~Command() {

	}
	virtual void execute() = 0;
	virtual bool validate(vector<string> args) = 0;

private:

protected:
	// exception msg
	const string INVALID_PARAMETER = "INVALID PARAMETER";
	const string INVALID_LBA_RANGE = "INVALID_LBA_RANGE";
	const string INVALID_DATA = "INVALID_DATA";
	const string INVALID_COMMAND = "INVALID COMMAND";
	const string INVALID_SIZE = "INVALID SIZE";
	const string INVALID_ADDRESS_RANGE = "END ADDRESS GT START ADDRESS";

	// integers
	const int MAX_LBA_ADDR = 99;
	const int MIN_LBA_ADDR = 0;
	const int MAX_ERASE_SIZE = 10;

	string commandName;
};

class ReadCommand : public Command {
public:
	ReadCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->addr = -1;
		this->commandName = "READ";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		string result = this->ssd->read(this->addr);
		LoggerSingleton::getInstance().print(this->commandName + "addr: " + to_string(addr) + " / " + result);
	}

	bool validate(vector<string> args) override {
		// read [addr]
		LoggerSingleton::getInstance().print(this->commandName);
		try {
			// format
			if (args.size() != 2)	throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int addr = stoi(args.at(1));

			// range
			if (addr < MIN_LBA_ADDR || MAX_LBA_ADDR < addr) throw runtime_error(this->INVALID_LBA_RANGE);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}

		// set address from args
		this->addr = stoi(args.at(1));
		return true;
	}
private:
	SSDRunner* ssd;
	int addr;
};

class WriteCommand : public Command {
public:
	WriteCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->addr = -1;
		this->data = "";
		this->commandName = "WRITE";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		this->ssd->write(this->addr, this->data);
		LoggerSingleton::getInstance().print(this->commandName + "addr: " + to_string(addr) + " / data" + data);
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// write [addr] [data]
		try {
			// format
			if (args.size() != 3) throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int addr = stoi(args.at(1));

			// range
			if (addr < MIN_LBA_ADDR || MAX_LBA_ADDR < addr) throw runtime_error(this->INVALID_LBA_RANGE);

			// data validation -> 0x00000000~0xFFFFFFFF
			string data = args.at(2);
			if (data.length() != 10) throw runtime_error(this->INVALID_DATA);
			if (data[0] != '0' || data[1] != 'x') throw runtime_error(this->INVALID_DATA);
			for (int i = 2; i < 10; i++) {
				char ch = data[i];
				if (!(('0' <= ch && ch <= '9')
					|| ('A' <= ch && ch <= 'F'))) {
					throw runtime_error(this->INVALID_DATA);
				}
			}
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}

		// set address from args
		this->addr = stoi(args.at(1));
		this->data = args.at(2);
		return true;
	}
private:
	SSDRunner* ssd;
	int addr;
	string data;
};

class ExitCommand : public Command {
public:
	ExitCommand() {
		this->commandName = "EXIT";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		LoggerSingleton::getInstance().print(this->commandName);
	}

	bool validate(vector<string> args) override {
		// exit
		LoggerSingleton::getInstance().print(this->commandName);
		try {
			// format
			if (args.size() != 1) throw runtime_error(this->INVALID_PARAMETER);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}

		return true;
	}
};

class HelpCommand : public Command {
public:
	HelpCommand() {
		this->commandName = "HELP";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		LoggerSingleton::getInstance().print(this->commandName);
		int lineCnt = 0;

		string filePath = "..\\..\\resources\\help.txt";
		ifstream file(filePath);
		string line = "";
		if (file.is_open()) {
			while (getline(file, line)) {
				cout << line << endl;
				lineCnt += 1;
			}
			file.close();
		}
		else {
			// To-Do try-catch
			cerr << "Unable to open help.txt!" << endl;
		}
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// help
		try {
			// format
			if (args.size() != 1) throw runtime_error(this->INVALID_PARAMETER);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}

		return true;
	}
};

class FullwriteCommand : public Command {
public:
	FullwriteCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->data = "";
		this->commandName = "FULLWRITE";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		LoggerSingleton::getInstance().print(this->commandName + "data: " + data);
		this->ssd->fullwrite(this->data);
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// fullwrite [data]
		try {
			// format
			if (args.size() != 2) throw runtime_error(this->INVALID_PARAMETER);

			// data validation -> 0x00000000~0xFFFFFFFF
			string data = args.at(1);
			if (data.length() != 10) throw runtime_error(this->INVALID_DATA);
			if (data[0] != '0' || data[1] != 'x') throw runtime_error(this->INVALID_DATA);
			for (int i = 2; i < 10; i++) {
				char ch = data[i];
				if (!(('0' <= ch && ch <= '9')
					|| ('A' <= ch && ch <= 'F'))) {
					throw runtime_error(this->INVALID_DATA);
				}
			}
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}

		// set data from args
		this->data = args.at(1);
		return true;
	}
private:
	SSDRunner* ssd;
	string data;
};

class FullreadCommand : public Command {
public:
	FullreadCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->commandName = "FULLREAD";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		LoggerSingleton::getInstance().print(this->commandName);
		cout << this->ssd->fullread() << endl;
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// fullwrite [data]
		try {
			// format
			if (args.size() != 1) throw runtime_error(this->INVALID_PARAMETER);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}
		return true;
	}
private:
	SSDRunner* ssd;
	string data;
};


class InvalidCommand : public Command {
public:
	InvalidCommand() {
		this->commandName = "INVALID COMMAND";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		cout << this->INVALID_COMMAND << endl;
		LoggerSingleton::getInstance().print(this->commandName);
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		return true;
	}
};

class EraseCommand : public Command {
public:
	EraseCommand(SSDRunner* ssd) {
		this->ssdRunner = ssd;
		this->addr = -1;
		this->size = -1;
		this->commandName = "ERASE";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		this->ssdRunner->erase(this->addr, this->size);
		LoggerSingleton::getInstance().print(this->commandName + "addr: " + to_string(addr) + " / size: " + to_string(size));
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// erase [addr] [size]
		try {
			// format
			if (args.size() != 3)	throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int addr = stoi(args.at(1));

			// range
			if (addr < MIN_LBA_ADDR || MAX_LBA_ADDR < addr) throw runtime_error(this->INVALID_LBA_RANGE);
			// string to integer
			int size = stoi(args.at(2));
			if (size > MAX_ERASE_SIZE) throw runtime_error(this->INVALID_SIZE);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}
		// set address from args

		this->addr = stoi(args.at(1));
		this->size = stoi(args.at(2));
		return true;
	}
private:
	SSDRunner* ssdRunner;
	int addr;
	int size;
};

class EraseRangeCommand : public Command {
public:
	EraseRangeCommand(SSDRunner* ssd) {
		this->ssdRunner = ssd;
		this->startAddr = -1;
		this->endAddr = -1;
		this->commandName = "ERASE_RANGE";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		LoggerSingleton::getInstance().print(this->commandName + "start Addr: " + to_string(startAddr) + " / end Addr: " + to_string(endAddr));
		while (true) {
			int size = endAddr - startAddr;
			if (size > MAX_ERASE_SIZE) {
				//cout << "erase " << to_string(startAddr) << " 10" << endl;
				this->ssdRunner->erase(this->startAddr, MAX_ERASE_SIZE);
				startAddr += MAX_ERASE_SIZE;
			}
			else {
				//cout << "erase " << to_string(startAddr) << " " << to_string(size) << endl;
				this->ssdRunner->erase(this->startAddr, size);
				break;
			}

		}
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// erase [startAddr] [endAddr]
		try {
			// format
			if (args.size() != 3)	throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int startAddr = stoi(args.at(1));
			// range
			if (startAddr < MIN_LBA_ADDR || MAX_LBA_ADDR < startAddr) throw runtime_error(this->INVALID_LBA_RANGE);

			// string to integer
			int endAddr = stoi(args.at(2));
			// range
			if (endAddr < MIN_LBA_ADDR || MAX_LBA_ADDR < endAddr) throw runtime_error(this->INVALID_LBA_RANGE);

			// start end gt
			if (endAddr <= startAddr) throw runtime_error(this->INVALID_ADDRESS_RANGE);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}

		// set address from args
		this->startAddr = stoi(args.at(1));
		this->endAddr = stoi(args.at(2));
		return true;
	}
private:
	SSDRunner* ssdRunner;
	int startAddr;
	int endAddr;
};

class FlushCommand : public Command {
public:
	FlushCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->commandName = "FLUSH";
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		this->ssd->flush();
		LoggerSingleton::getInstance().print(this->commandName);
	}

	bool validate(vector<string> args) override {
		LoggerSingleton::getInstance().print(this->commandName);
		// flush
		try {
			// format
			if (args.size() != 1) throw runtime_error(this->INVALID_PARAMETER);
		}
		catch (exception e) {
			cout << e.what() << endl;
			return false;
		}
		return true;
	}
private:
	SSDRunner* ssd;
	string data;
};

// invoker
class CommandContoller {
public:
	void setCommand(Command* command) {
		this->command = command;
	}

	void execute() {
		this->command->execute();
	}

	bool validate(vector<string> args) {
		return this->command->validate(args);
	}
private:
	Command* command;

};
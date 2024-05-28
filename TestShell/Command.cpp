#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "SSDRunner.cpp"

using namespace std;

class Command {
public:
	virtual ~Command() {

	}
	virtual void execute() = 0;
	virtual bool validate(vector<string> args) = 0;

private:

protected:
	const string INVALID_PARAMETER = "INVALID PARAMETER";
	const string INVALID_LBA_RANGE = "INVALID_LBA_RANGE";
	const string INVALID_DATA = "INVALID_DATA";
	const string INVALID_COMMAND = "INVALID COMMAND";
	const string INVALID_SIZE = "INVALID SIZE";
<<<<<<< HEAD
	const string INVALID_ADDRESS_RANGE = "END ADDRESS GT START ADDRESS";
=======
>>>>>>> 05aafb3c780efa5f4ed090d686caf6854dffcdd7
};

class ReadCommand : public Command {
public:
	ReadCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->addr = -1;
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		cout << this->ssd->read(this->addr) << endl;

	}

	bool validate(vector<string> args) override {
		// read [addr]
		try {
			// format
			if (args.size() != 2)	throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int addr = stoi(args.at(1));

			// range
			if (addr < 0 || 99 < addr) throw runtime_error(this->INVALID_LBA_RANGE);
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
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		this->ssd->write(this->addr, this->data);
	}

	bool validate(vector<string> args) override {
		// write [addr] [data]
		try {
			// format
			if (args.size() != 3) throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int addr = stoi(args.at(1));

			// range
			if (addr < 0 || 99 < addr) throw runtime_error(this->INVALID_LBA_RANGE);

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

	}

	// Command을(를) 통해 상속됨
	void execute() override {
	}

	bool validate(vector<string> args) override {
		// exit
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

	}

	// Command을(를) 통해 상속됨
	void execute() override {
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
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		this->ssd->fullwrite(this->data);
	}

	bool validate(vector<string> args) override {
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
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		cout << this->ssd->fullread() << endl;
	}

	bool validate(vector<string> args) override {
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

	}

	// Command을(를) 통해 상속됨
	void execute() override {
		cout << this->INVALID_COMMAND << endl;
	}

	bool validate(vector<string> args) override {
		return true;
	}
};

class EraseCommand : public Command {
public:
	EraseCommand(SSDRunner* ssd) {
		this->ssdRunner = ssd;
		this->addr = -1;
		this->size = -1;
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		cout << "erase " << to_string(addr) << " " << to_string(size) << endl;
		this->ssdRunner->erase(this->addr, this->size);
	}

	bool validate(vector<string> args) override {
		// erase [addr] [size]
		try {
			// format
			if (args.size() != 3)	throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int addr = stoi(args.at(1));

			// range
			if (addr < 0 || 99 < addr) throw runtime_error(this->INVALID_LBA_RANGE);
			// string to integer
			int size = stoi(args.at(2));
			if (size > 10) throw runtime_error(this->INVALID_SIZE);
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
	}

	// Command을(를) 통해 상속됨
	void execute() override {		
		while (true) {
			int size = endAddr - startAddr;
			if (size > 10) {
				//cout << "erase " << to_string(startAddr) << " 10" << endl;
				this->ssdRunner->erase(this->startAddr, 10);
				startAddr += 10;
			}
			else {
				//cout << "erase " << to_string(startAddr) << " " << to_string(size) << endl;
				this->ssdRunner->erase(this->startAddr, size);
				break;
			}
			
		}


	}

	bool validate(vector<string> args) override {
		// erase [startAddr] [endAddr]
		try {
			// format
			if (args.size() != 3)	throw runtime_error(this->INVALID_PARAMETER);

			// addr validation
			// string to integer
			int startAddr = stoi(args.at(1));
			// range
			if (startAddr < 0 || 99 < startAddr) throw runtime_error(this->INVALID_LBA_RANGE);

			// string to integer
			int endAddr = stoi(args.at(2));
			// range
			if (endAddr < 0 || 99 < endAddr) throw runtime_error(this->INVALID_LBA_RANGE);

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
#pragma once
#include <vector>
#include <string>
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
};

class ReadCommand : public Command {
public:
	ReadCommand(SSDRunner* ssdRunner) {
		this->ssd = ssdRunner;
		this->addr = -1;
	}

	// Command을(를) 통해 상속됨
	void execute() override {
		string result = this->ssd->read(this->addr);
		cout << result << endl;
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
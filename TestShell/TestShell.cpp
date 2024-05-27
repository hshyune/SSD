#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "SSDRunner.cpp"

#include "TestScript.cpp"
#include "TestLogger.cpp"
#include "Command.cpp"
using namespace std;

class TestShell {
public:
	TestShell() {
		testApp = nullptr;
		this->ssdRunner = new SSDRunner();
	}
	~TestShell() {
		delete testApp;
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

	void setTestApp(const string& testName) {
		logger.print(testName + " 테스트를 등록");
		if (testName == "testapp1") {
			testApp = new TestApp1;
		}
		else if (testName == "testapp2") {
			testApp = new TestApp2;
		}
		else if (testName == "FullWriteReadCompare") {
			testApp = new FullWriteReadCompare;
		}
		else if (testName == "FullRead10AndCompare") {
			testApp = new FullRead10AndCompare;
		}
		else if (testName == "Write10AndCompare") {
			testApp = new Write10AndCompare;
		}
		else if (testName == "Loop_WriteAndReadCompare") {
			testApp = new Loop_WriteAndReadCompare;
		}
		else {
			testApp = nullptr;
		}
	}

	int runTest(const string& testName) {
		logger.print("등록된 Test를 시작");
		cout << testName << "    ---    " << "Run...";
		if (testApp == nullptr)
		{
			cout << endl;
			throw exception(" - 존재하지 않는 TestScript입니다.");
		}

		return testApp->runTest();
	}

	void runTestList()
	{
		logger.print("'run_list.lst'의 테스트들을 불러 실행");
		ifstream file(PATH_TESTLIST_FILE);
		string testName = "";
		if (file.is_open()) {
			try
			{
				while (getline(file, testName)) {
					setTestApp(testName);
					switch (runTest(testName))
					{
					case SUCCESS:
						cout << "PASS" << endl;
						break;
					case FAIL:
						cout << "FAIL" << endl;
						break;
					default:
						break;
					}
				}
			}
			catch (const exception& e)
			{
				logger.print(e.what());
			}
			file.close();
		}
		else {
			cout << "Unable to open " << PATH_TESTLIST_FILE << endl;
			return;
		}
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
			else {
				InvalidCommand* command = new InvalidCommand();
				commandContoller.setCommand(command);

				bool isValid = commandContoller.validate(args);
				if (isValid) {
					commandContoller.execute();
				}
				continue;
			}

			// execution - testScript
			if (cmd == "run_list.lst") {
				this->runTestList();
			}
			else {

			}
		}
	}

private:
	SSDRunner ssd;
	ITestApp* testApp;
	bool isRunning = true;
	Logger logger;

	const string INVALID_PARAMETER = "INVALID PARAMETER";
	const string INVALID_LBA_RANGE = "INVALID_LBA_RANGE";
	const string INVALID_DATA = "INVALID_DATA";
	const string INVALID_COMMAND = "INVALID COMMAND";

	CommandContoller commandContoller;
	SSDRunner* ssdRunner;
};

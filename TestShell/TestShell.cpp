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
		logger.print("Regist test '" + testName + "'");
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
		logger.print("Run " + testName);
		cout << testName << "    ---    " << "Run...";
		if (testApp == nullptr)
			return ERROR_TEST_NOT_EXISTED;

		return testApp->runTest();
	}

	void runTestList()
	{
		logger.print("Run all of tests in " + PATH_TESTLIST_FILE);
		ifstream file(PATH_TESTLIST_FILE);
		string testName = "";
		if (file.is_open()) {
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
				case ERROR_TEST_NOT_EXISTED:
					cout << "ERROR - This test is not existed" << endl;
					break;
				default:
					break;
				}
			}
			file.close();
		}
		else {
			cout << "Unable to open " << PATH_TESTLIST_FILE << endl;
			return;
		}
	}

	void runTestListUsingFile()
	{
		logger.print("Run all of tests in " + PATH_TESTLIST_FILE);
		ifstream file(PATH_TESTLIST_FILE);
		string testName = "";
		if (file.is_open()) {
			while (getline(file, testName)) {
				string testFileName = "test_" + testName + ".txt";
				ifstream testFile(testFileName);
				string line = "";
				if (testFile.is_open())
				{
					while (getline(testFile, line)) {
						cout << line << endl;
						// 여기에 실행코드 구현
						// split string
						istringstream iss(line);
						string tmp;
						vector<string> args;
						while (getline(iss, tmp, ' ')) {
							args.push_back(tmp);
						}
						// empty input
						if (args.size() == 0) continue;
						string cmd = args.at(0);
						if (cmd == "read") {
							// read [addr]
							// format
							try {
								if (args.size() != 2) {
									throw runtime_error(this->INVALID_PARAMETER);
								}
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
							// addr validation
							try {
								// string to integer
								int addr = stoi(args.at(1));
								// range
								if (addr < 0 || 99 < addr) throw runtime_error(this->INVALID_LBA_RANGE);
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
						}
						else if (cmd == "write") {
							// write [addr] [data]
							// format
							try {
								if (args.size() != 3) {
									throw runtime_error(this->INVALID_PARAMETER);
								}
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
							// addr validation
							try {
								// string to integer
								int addr = stoi(args.at(1));
								// range
								if (addr < 0 || 99 < addr) throw runtime_error(this->INVALID_LBA_RANGE);
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
							// data validation -> 0x00000000~0xFFFFFFFF
							try {
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
								continue;
							}
						}
						else if (cmd == "exit") {
							// exit
							// format
							try {
								if (args.size() != 1) {
									throw runtime_error(this->INVALID_PARAMETER);
								}
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
						}
						else if (cmd == "help") {
							// help
							// format
							try {
								if (args.size() != 1) {
									throw runtime_error(this->INVALID_PARAMETER);
								}
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
						}
						else if (cmd == "fullwrite") {
							// fullwrite [data]
							// format
							try {
								if (args.size() != 2) {
									throw runtime_error(this->INVALID_PARAMETER);
								}
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
							// data validation -> 0x00000000~0xFFFFFFFF
							try {
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
								continue;
							}
						}
						else if (cmd == "fullread") {
							// fullread
							// format
							try {
								if (args.size() != 1) {
									throw runtime_error(this->INVALID_PARAMETER);
								}
							}
							catch (exception e) {
								cout << e.what() << endl;
								continue;
							}
						}
						else if (cmd == "run_list.lst") {
						}
						else {
							cout << this->INVALID_COMMAND << endl;
							continue;
						}
						// execution
						if (cmd == "read") {
							int addr = stoi(args.at(1));
							string result = this->read(addr);
							cout << result << endl;
						}
						else if (cmd == "write") {
							int addr = stoi(args.at(1));
							string data = args.at(2);
							this->write(addr, data);
						}
						else if (cmd == "help") {
							this->help();
						}
						else if (cmd == "exit") {
							this->exit();
						}
						else if (cmd == "fullread") {
							this->fullread();
						}
						else if (cmd == "fullwrite") {
							string data = args.at(1);
							this->fullwrite(data);
						}
						else if (cmd == "run_list.lst") {
							this->runTestListUsingFile();
						}
						else {

						}
					}
					testFile.close();
				}
				else {
					cout << "This test is not existed : " << testFileName << endl;
				}
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

			if (cmd == "run_list.lst") {
				this->runTestListUsingFile();
			}
			else {

			}
		}
	}

private:
	SSDRunner ssd;
	ITestApp* testApp;
	bool isRunning = true;
	LoggerSingleton& logger{ LoggerSingleton::getInstance() };

	const string INVALID_PARAMETER = "INVALID PARAMETER";
	const string INVALID_LBA_RANGE = "INVALID_LBA_RANGE";
	const string INVALID_DATA = "INVALID_DATA";
	const string INVALID_COMMAND = "INVALID COMMAND";

	CommandContoller commandContoller;
	SSDRunner* ssdRunner;
};

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cstring>
#include "SSDRunner.cpp"
#include "Exception.cpp"
#include "TestScript.cpp"
using namespace std;

class Logger {
public:
	Logger() {
		std::fstream fs(loggerFile, std::fstream::out);
		fs.close();
	}

	std::string getCurrentTime() {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80]{};

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "[%g.%m.%d %R]", timeinfo);
		return std::string(buffer);
	}

	std::string getCallerName(const std::string& callerName) {
		const int fixedFunctionLength = 30;
		char outBuffer[fixedFunctionLength + 1]{};
		fill_n(outBuffer, fixedFunctionLength, 0x20);
		strcpy(outBuffer, callerName.c_str());
		if (callerName.size() < fixedFunctionLength)
			outBuffer[callerName.size()] = 0x20;
		outBuffer[fixedFunctionLength] = 0x0;
		return std::string(" ") + outBuffer;
	}

	std::string getLogMessage(const std::string& logMessage) {
		return std::string(": ") + logMessage;
	}

	void printConsoleAndFile(std::string loggerOutput) {
		std::cout << loggerOutput << std::endl;
		std::fstream fs(loggerFile, std::fstream::out | std::fstream::app);
		fs << loggerOutput << std::endl;
		fs.close();
	}

	void print(std::string logMessage, const char* str = __builtin_FUNCTION()) {
		std::string loggerOutput = getCurrentTime();
		loggerOutput += getCallerName(std::string(str) + "()");
		loggerOutput += getLogMessage(logMessage);
		printConsoleAndFile(loggerOutput);
	}
private:
	std::string loggerFile{ "result.txt" };
};

class TestShell {
public:
	TestShell() {
		testApp = nullptr;
	}
	~TestShell() {
		delete testApp;
	}
	void write(int address, string data) {
		ssd.write(address, data);
	}

	string read(int address) {
		// TODO: 임의로 IIoInterface를 넣어 구현, 추후에 실제 SSD로 inteface 수정 필요
		//ssd 에 명령어를 전달한다 + result.txt 에 결과를 출력한다.
		return ssd.read(address);
	}

	void exit() {
		this->isRunning = false;
	}

	int help() {
		// 각 명령어당 사용 방법을 출력한다
		int lineCnt = 0;

		ifstream file("../TestShell/help.txt");
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
		return lineCnt;
	}

	void fullwrite(string data) {
		//LBA 0 번부터 99 번 까지 Write를 수행한다.
		//ssd 전체에 값이 써진다.
		//ex) fullwrite 0xABCD'FFFF
		//모든 LBA에 값 0xABCD'FFFF 가 적힌다.

		//IIoInterface* ioInterface = new Nand();
		//IIoInterface* ioInterface;
		//SSD ssd(ioInterface);

		const int LBA_SIZE = 100;
		for (int address = 0; address < LBA_SIZE; ++address) { // Address [0,99]
			this->write(address, data);
		}
	}

	void fullread() {
		//LBA 0 번부터 99 번 까지 Read를 수행한다.
		//ssd 전체 값을 모두 화면에 출력한다.
		//ex) fullread
		//모든 LBA의 값들이 화면에 출력된다

		//IIoInterface* ioInterface = new Nand();
		//IIoInterface* ioInterface;
		//SSD ssd(ioInterface);

		const int LBA_SIZE = 100;
		string readData = "";
		for (int address = 0; address < LBA_SIZE; ++address) { // Address [0,99]
			//readData += ssd.read(address);
			//readData += to_string(address) + "," + this->read(address) + "\n";
			readData += to_string(address) + "," + this->read(address);
		}
		cout << readData << endl;
	}

	void setTestApp(const string& testName) {
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

	int runTest() {
		if (testApp == nullptr)
			throw exception(" - 존재하지 않는 TestScript입니다.");

		return testApp->runTest();
	}

	void runTestList()
	{
		ifstream file(PATH_TESTLIST_FILE);
		string testName = "";
		if (file.is_open()) {
			try
			{
				while (getline(file, testName)) {
					setTestApp(testName);
					cout << testName << "    ---    " << "Run...";
					switch (runTest())
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
				cout << e.what() << endl;
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
};

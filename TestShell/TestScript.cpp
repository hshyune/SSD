#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "SSDRunner.cpp"
#include "TestLogger.cpp"
#include "Command.cpp"
using namespace std;

const string PATH_TESTLIST_FILE = "run_list.lst";
const string PATH_NAND = "nand.txt";
enum TEST_RESULT {
	PASS = 0,
	FAIL,
	ERROR_TEST_NOT_EXISTED,
	ERROR_CANNOT_OPEN_FILE,
	ERROR_CANNOT_READ_NAND,
};

class ITestApp {
public:
	virtual int runTest() = 0;
};

class TestApp1 : public ITestApp {
public:
	int runTest() override
	{
		// 먼저fullwrite를 수행한다.
		// fullread를 하면서, write 한 값대로 read가 되는지 확인한다.
		return TEST_RESULT::PASS;
	}
};

class TestApp2 : public ITestApp {
public:
	int runTest() override
	{
		// 0~5 번 LBA 에 0xAAAABBBB 값으로 총 30번 Write를 수행한다.
		// 0~5 번 LBA 에 0x12345678 값으로 1 회 Over Write를 수행한다.
		// 0~5 번 LBA Read 했을 때 정상적으로 값이 읽히는지 확인한다.
		return TEST_RESULT::PASS;
	}
};

class FullWriteReadCompare : public ITestApp {
public:
	int runTest() override
	{
		// fullwrite를 수행
		// fullread를 하면서, write 한 값대로 read가 되는지 확인한다.
		return TEST_RESULT::PASS;
	}
};

class FullRead10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		// fullread를 10회
		return TEST_RESULT::PASS;
	}
};

class Write10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		// full write 10회
		return TEST_RESULT::PASS;
	}
};

class Loop_WriteAndReadCompare : public ITestApp {
public:
	int runTest() override
	{
		// write하고 read를 각 1회씩 반복테스트
		return TEST_RESULT::FAIL;
	}
};

class TestScriptRunner {
public:
	void setTestApp(const string& testName) {
		LoggerSingleton::getInstance().print("Regist test '" + testName + "'");
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
		LoggerSingleton::getInstance().print("Run " + testName);
		cout << testName << "    ---    " << "Run...";
		if (testApp == nullptr)
			return TEST_RESULT::ERROR_TEST_NOT_EXISTED;

		return testApp->runTest();
	}

	void runTestList()
	{
		LoggerSingleton::getInstance().print("Run all of tests in " + PATH_TESTLIST_FILE);
		ifstream file(PATH_TESTLIST_FILE);
		string testName = "";
		if (file.is_open()) {
			while (getline(file, testName)) {
				setTestApp(testName);
				switch (runTest(testName))
				{
				case TEST_RESULT::PASS:
					cout << "TEST_RESULT::PASS" << endl;
					break;
				case TEST_RESULT::FAIL:
					cout << "TEST_RESULT::FAIL" << endl;
					break;
				case TEST_RESULT::ERROR_TEST_NOT_EXISTED:
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

	string getDataFromFile(string filePath)
	{
		string result = "";
		ifstream file(filePath);
		if (file.is_open()) {
			string line = "";
			while (getline(file, line)) {
				result += line;
			}
		}
		else {
			cout << "Cannot open this file : " << filePath << endl;
			return "-1";
		}
		return result;
	}

	int getTestResult(const string& expectedFilePath)
	{
		string expectedData = getDataFromFile(expectedFilePath);
		string nandData = getDataFromFile(PATH_NAND);

		if (expectedData == "-1")
			return TEST_RESULT::ERROR_CANNOT_OPEN_FILE;
		if (nandData == "-1")
			return TEST_RESULT::ERROR_CANNOT_READ_NAND;

		if (nandData == expectedData)
			return TEST_RESULT::PASS;
		else
			return TEST_RESULT::FAIL;
	}

	void runTestListUsingFile(string filePath)
	{
		LoggerSingleton::getInstance().print("Run all of tests in " + filePath + " using text file");
		ifstream file(filePath);
		string testName = "";
		if (file.is_open()) {
			while (getline(file, testName)) {
				cout << testName << "    ---    " << "Run..." << endl;
				string testFileName = "test_" + testName + ".txt";
				ifstream testFile(testFileName);
				string input = "";
				if (testFile.is_open())
				{
					while (getline(testFile, input)) {
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
					}
					testFile.close();
					// pass / fail 판단
					string expectedFileName = "expected_" + testName + ".txt";
					int testResult = getTestResult(expectedFileName);
					switch (testResult)
					{
					case TEST_RESULT::PASS:
						cout << "TEST_RESULT::PASS" << endl;
						break;
					case TEST_RESULT::FAIL:
						cout << "TEST_RESULT::FAIL" << endl;
						break;
					case TEST_RESULT::ERROR_CANNOT_OPEN_FILE:
						cout << "ERROR - Cannot open file" << endl;
					case TEST_RESULT::ERROR_CANNOT_READ_NAND:
						cout << "ERROR - Cannot read nand" << endl;
						break;
					}
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

private:
	ITestApp* testApp;
	CommandContoller commandContoller;
	SSDRunner* ssdRunner;
};

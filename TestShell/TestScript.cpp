﻿#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "SSDRunner.cpp"
#include "TestLogger.h"
#include "Command.cpp"
using namespace std;

const string PATH_TESTLIST_FILE = "run_list.lst";
const string PATH_NAND = "nand.txt";
const string PATH_RESULT = "result.txt";
const string PATH_RESOURCE_FOLDER = "..\\..\\resources\\";

enum TEST_RESULT {
	PASS = 0,
	FAIL,
	ERROR_TEST_NOT_EXISTED,
	ERROR_CANNOT_OPEN_FILE,
	ERROR_CANNOT_READ_NAND,
};

class ITestApp {
public:
	ITestApp() {
		ssdRunner = new SSDRunner();
	}
	virtual int runTest() = 0;
	void runCmd(Command* command, std::vector<std::string>& args)
	{
		commandContoller.setCommand(command);
		if (commandContoller.validate(args))
			commandContoller.execute();
	}
	CommandContoller commandContoller;
	SSDRunner* ssdRunner;
};

class TestApp1 : public ITestApp {
public:
	int runTest() override
	{
		// fullwrite
		vector<string> args = { "fullwrite", "0x12345678" };
		Command* command = new FullwriteCommand(this->ssdRunner);
		runCmd(command, args);

		// fullread
		vector<string> args2 = { "" };
		command = new FullreadCommand(this->ssdRunner);
		runCmd(command, args2);

		return TEST_RESULT::PASS;
	}
};

class TestApp2 : public ITestApp {
public:
	int runTest() override
	{
		// 0~5 번 LBA 에 0xAAAABBBB write
		Command* command = new WriteCommand(this->ssdRunner);
		vector<string> args;
		for (int lba = 0; lba <= 5; ++lba) {
			args.push_back("write");
			args.push_back(to_string(lba));
			args.push_back("0xAAAABBBB");
			runCmd(command, args);
			args.clear();
		}

		// 0~5 번 LBA 에 0x12345678 write
		for (int lba = 0; lba <= 5; ++lba) {
			args.push_back("write");
			args.push_back(to_string(lba));
			args.push_back("0x12345678");
			runCmd(command, args);
			args.clear();
		}

		// 0~5 번 LBA Read 
		command = new ReadCommand(this->ssdRunner);
		for (int lba = 0; lba <= 5; ++lba) {
			args.push_back("read");
			args.push_back(to_string(lba));
			runCmd(command, args);
			args.clear();
		}

		return TEST_RESULT::PASS;
	}
};

class FullWriteReadCompare : public ITestApp {
public:
	int runTest() override
	{
		vector<string> args = { "fullwrite", "0xCAFECAFE" };
		Command* command = new FullwriteCommand(this->ssdRunner);
		runCmd(command, args);

		// fullread
		vector<string> args2 = { "" };
		command = new FullreadCommand(this->ssdRunner);
		runCmd(command, args2);

		return TEST_RESULT::PASS;
	}
};

class FullRead10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		// fullread를 10회
		vector<string> args2 = { "" };
		Command* command = new FullreadCommand(this->ssdRunner);
		for (int cnt = 0; cnt < 10; ++cnt)
			runCmd(command, args2);
		return TEST_RESULT::PASS;
	}
};

class Write10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		// full write 10회
		vector<string> args = { "fullwrite", "0xCAFECAFE" };
		Command* command = new FullwriteCommand(this->ssdRunner);
		for (int cnt = 0; cnt < 10; ++cnt)
			runCmd(command, args);
		return TEST_RESULT::PASS;
	}
};

class Loop_WriteAndReadCompare : public ITestApp {
public:
	int runTest() override
	{
		// write하고 read를 각 1회씩 반복테스트
		Command* command;
		vector<string> args;
		for (int cnt = 0; cnt < 10; ++cnt) {
			command = new WriteCommand(this->ssdRunner);
			args.push_back("write");
			args.push_back(to_string(0));
			args.push_back("0x12345678");
			runCmd(command, args);
			args.clear();

			command = new ReadCommand(this->ssdRunner);
			args.push_back("read");
			args.push_back(to_string(0));
			runCmd(command, args);
			args.clear();
		}
		return TEST_RESULT::PASS;
	}
};

class TestScriptRunner {
public:
	TestScriptRunner() {
		ssdRunner = new SSDRunner();
	}
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
		cout << testName << "    ---    " << "Run..." << endl;
		if (testApp == nullptr)
			return TEST_RESULT::ERROR_TEST_NOT_EXISTED;

		return testApp->runTest();
	}

	void runTestList()
	{
		LoggerSingleton::getInstance().print("Run all of tests in " + PATH_TESTLIST_FILE);
		ifstream file(PATH_RESOURCE_FOLDER + PATH_TESTLIST_FILE);
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

	void runTC(const string& testName) {
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

	void printResult(int testResult)
	{
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

	Command* setCmd(std::string& cmd)
	{
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
		else {
			command = new InvalidCommand();
		}
		return command;
	}

	void runCmd(Command* command, std::vector<std::string>& args)
	{
		commandContoller.setCommand(command);
		if (commandContoller.validate(args))
			commandContoller.execute();
	}

	void runTestListUsingFile(string filePath)
	{
		LoggerSingleton::getInstance().print("Run all of tests in " + filePath + " using text file");
		ifstream file(PATH_RESOURCE_FOLDER + filePath);
		string testName = "";
		if (file.is_open()) {
			while (getline(file, testName)) {
				cout << testName << "    ---    " << "Run..." << endl;
				string testFileName = "test_" + testName + ".txt";
				ifstream testFile(PATH_RESOURCE_FOLDER + testFileName);
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
						string cmd = args.at(0); \
							runCmd(setCmd(cmd), args);
					}
					testFile.close();
					// pass / fail 판단
					string expectedFileName = "expected_" + testName + ".txt";
					printResult(getTestResult(PATH_RESOURCE_FOLDER + expectedFileName));
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

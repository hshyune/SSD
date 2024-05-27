#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const string PATH_TESTLIST_FILE = "run_list.lst";
const int SUCCESS = 0;
const int FAIL = 1;

class ITestApp {
public:
	virtual int runTest() = 0;
};

class TestApp1 : public ITestApp {
public:
	int runTest() override
	{
		cout << "@@@@@@@@@@ TestApp1 실행" << endl;
		// TestApp1 제작하기
		// 먼저fullwrite를 수행한다.
		// fullread를 하면서, write 한 값대로 read가 되는지 확인한다.
		return SUCCESS;
	}
};

class TestApp2 : public ITestApp {
public:
	int runTest() override
	{
		cout << "@@@@@@@@@@ TestApp2 실행" << endl;
		// 0~5 번 LBA 에 0xAAAABBBB 값으로 총 30번 Write를 수행한다.
		// 0~5 번 LBA 에 0x12345678 값으로 1 회 Over Write를 수행한다.
		// 0~5 번 LBA Read 했을 때 정상적으로 값이 읽히는지 확인한다.
		return SUCCESS;
	}
};

class FullWriteReadCompare : public ITestApp {
public:
	int runTest() override
	{
		cout << "@@@@@@@@@@ FullWriteReadCompare 실행" << endl;
		return SUCCESS;
	}
};

class FullRead10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		cout << "@@@@@@@@@@ FullRead10AndCompare 실행" << endl;
		return SUCCESS;
	}
};

class Write10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		cout << "@@@@@@@@@@ Write10AndCompare 실행" << endl;
		return SUCCESS;
	}
};

class Loop_WriteAndReadCompare : public ITestApp {
public:
	int runTest() override
	{
		cout << "@@@@@@@@@@ Loop_WriteAndReadCompare 실행" << endl;
		return FAIL;
	}
};

class TestScriptRunner {
	ITestApp* testApp;
public:
	TestScriptRunner() {
		testApp = nullptr;
	}
	~TestScriptRunner() {
		delete testApp;
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
	void runTest() {
		if (testApp == nullptr)
			throw exception(" - 존재하지 않는 TestScript입니다.");

		testApp->runTest();
	}
	void runTestList()
	{
		string testName = "";
		ifstream file(PATH_TESTLIST_FILE);

		if (file.is_open()) {
			while (getline(file, testName)) {
				setTestApp(testName);
				cout << testName << "    ---    " << "Run...";
				switch (testApp->runTest())
				{
				case SUCCESS:
					cout << "PASS" << endl;
					return;
				case FAIL:
					cout << "FAIL" << endl;
					return;
				}
			}
			file.close();
		}
		else {
			cout << "Unable to open " << PATH_TESTLIST_FILE << endl;
			return;
		}
	}
};

#pragma once
#include <iostream>
#include <string>
using namespace std;

class ITestApp {
public:
	virtual void runTest() = 0;
};

class TestApp1 : public ITestApp {
public:
	void runTest() override
	{
		cout << "@@@@@@@@@@ TestApp1 실행" << endl;
		// TestApp1 제작하기
		// 먼저fullwrite를 수행한다.
		// fullread를 하면서, write 한 값대로 read가 되는지 확인한다.
	}
};

class TestApp2 : public ITestApp {
public:
	void runTest() override
	{
		cout << "@@@@@@@@@@ TestApp2 실행" << endl;
		// 0~5 번 LBA 에 0xAAAABBBB 값으로 총 30번 Write를 수행한다.
		// 0~5 번 LBA 에 0x12345678 값으로 1 회 Over Write를 수행한다.
		// 0~5 번 LBA Read 했을 때 정상적으로 값이 읽히는지 확인한다.
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
		else {
			testApp = nullptr;
		}
	}

	void runTest() {
		if (testApp == nullptr)
			throw exception("없는 TestName 입니다.");

		testApp->runTest();
	}
};
	
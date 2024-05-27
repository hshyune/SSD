#pragma once
#include <iostream>
#include <string>
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

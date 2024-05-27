#pragma once
#include <iostream>
#include <string>
using namespace std;

const string PATH_TESTLIST_FILE = "run_list.lst";
const int SUCCESS = 0;
const int FAIL = 1;
const int ERROR_TEST_NOT_EXISTED = 2;

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
		return SUCCESS;
	}
};

class TestApp2 : public ITestApp {
public:
	int runTest() override
	{
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
		// fullwrite를 수행
		// fullread를 하면서, write 한 값대로 read가 되는지 확인한다.
		return SUCCESS;
	}
};

class FullRead10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		// fullread를 10회
		return SUCCESS;
	}
};

class Write10AndCompare : public ITestApp {
public:
	int runTest() override
	{
		// full write 10회
		return SUCCESS;
	}
};

class Loop_WriteAndReadCompare : public ITestApp {
public:
	int runTest() override
	{
		// write하고 read를 각 1회씩 반복테스트
		return FAIL;
	}
};

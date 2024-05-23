#include <iostream>
#include <exception>
using namespace std;

class ExceptionInvalidCommand : public exception
{
public:
	char const* what() const override
	{
		"INVALID COMMAND";
	}
};

class ExceptionLbaOutOfRange : public exception
{
public:
	char const* what() const override
	{
		"LBA 범위는 0~99 입니다.";
	}
};

class ExceptionInvalidData : public exception
{
public:
	char const* what() const override
	{
		"Data의 1자리당 범위는 16진수로 0~F 입니다.";
	}
};

class ExceptionInvalidParameter : public exception
{
public:
	char const* what() const override
	{
		"파라미터 Format이 정상적이지 않습니다.";
	}
};

class ExceptionExitProgram : public exception {
public:
	explicit ExceptionExitProgram(char const* _msg) :exception(_msg) {

	}
};
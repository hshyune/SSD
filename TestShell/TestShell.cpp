#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "../TestShellTest/Exception.cpp""
#include "../SSD/SSD.h"
using namespace std;

class TestShell {
public:
	void write(int address, string data) {
		ssd->write(address, data);
	}

	string read(int address) {
		// TODO: 임의로 IIoInterface를 넣어 구현, 추후에 실제 SSD로 inteface 수정 필요
		//ssd 에 명령어를 전달한다 + result.txt 에 결과를 출력한다.
		return ssd->read(address);
	}

	void exit() {
		throw ExceptionExitProgram("프로그램을 종료합니다.");
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
		IIoInterface* ioInterface;
		SSD ssd(ioInterface);

		const int LBA_SIZE = 100;
		for (int address = 0; address < LBA_SIZE; ++address) { // Address [0,99]
			ssd.write(address, data);
		}
	}

	void fullread() {
		//LBA 0 번부터 99 번 까지 Read를 수행한다.
		//ssd 전체 값을 모두 화면에 출력한다.
		//ex) fullread
		//모든 LBA의 값들이 화면에 출력된다

		//IIoInterface* ioInterface = new Nand();
		IIoInterface* ioInterface;
		SSD ssd(ioInterface);

		const int LBA_SIZE = 100;
		string readData = "";
		for (int address = 0; address < LBA_SIZE; ++address) { // Address [0,99]
			//readData += ssd.read(address);
			readData += to_string(address) + "," + ssd.read(address) + "\n";
		}
		cout << readData << endl;
	}

private:
	ISSDInterface* ssd;
};

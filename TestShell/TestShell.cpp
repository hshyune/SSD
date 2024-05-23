#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "../SSD/SSD.h"
using namespace std;

class TestShell {
public:
	void write(int address, string data) {
		/*
		3번 LBA 에 0xAAAABBBB 를 기록한다.
		ssd 에 명령어를 전달한다
		*/
	}

	string read(int address, int times) {
		//times 횟수만큼 LBA 를 읽는다.

		// TODO: 임의로 IIoInterface를 넣어 구현, 추후에 실제 SSD로 inteface 수정 필요
		IIoInterface* ioInterface{};
		SSD ssd(ioInterface);
		string data = "";

		//ssd 에 명령어를 전달한다.
		for (int i = 0; i < times; ++i) {
			//result.txt 에 적힌 결과를 화면에 출력한다.
			data += ssd.read(address);
			data += '\n';
		}

		return data;
	}

	void exit() {
		// Shell 이 종료된다
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
		//ex) fullwrite 0xABCDFFFF
		//모든 LBA에 값 0xABCDFFF 가 적힌다.
	}
	void  fullread() {
		//LBA 0 번부터 99 번 까지 Read를 수행한다.
		//ssd 전체 값을 모두 화면에 출력한다.
		//ex) fullread
		//모든 LBA의 값들이 화면에 출력된다
	}
};

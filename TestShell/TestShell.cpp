#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "SSDRunner.cpp"
using namespace std;

class TestShell {
public:
	void write(int address, string data) {
		ssd.write(address, data);
	}

	string read(int address) {
		// TODO: 임의로 IIoInterface를 넣어 구현, 추후에 실제 SSD로 inteface 수정 필요
		//ssd 에 명령어를 전달한다 + result.txt 에 결과를 출력한다.
		return ssd.read(address);
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

	void run() {
		// run
		bool isRunning = true;
		while (isRunning) {
			// input command
			string input = "";
			cout << "input : ";
			getline(cin, input);

			// split string
			istringstream iss(input);
			string tmp;
			vector<string> args;
			while (getline(iss, tmp, ' ')) {
				args.push_back(tmp);
			}
			// validation

			string cmd = args.at(0);
			if (cmd == "read") {
				int addr = stoi(args.at(1));
				this->read(addr);
				// read result.txt
				ifstream file("../TestShell/result.txt");
				string line = "";
				if (file.is_open()) {
					while (getline(file, line)) {
						cout << line << endl;
					}
					file.close();
				}
				else {
					cerr << "Unable to open help.txt!" << endl;
				}
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
				isRunning = false;
			}
			else if (cmd == "fullread") {
				this->fullread();
			}
			else if (cmd == "fullwrite") {
				string data = args.at(1);
				this->fullwrite(data);
			}
			else {

			}
		}
	}

private:
	SSDRunner ssd;
};

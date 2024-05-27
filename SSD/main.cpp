#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ssd_factory.h"

using namespace std;

int main(int argc, char* argv[]) {
    ISSDInterface* ssd = SSDFactory::CreateNandSSD();

    std::string operation = argv[1];

    if (operation == "W") {
        int address = std::stoi(argv[2]);
        if (argc < 4) {
            std::cout << "Insufficient arguments for write operation" << std::endl;
            return 1;
        }
        std::string data = argv[3];
        ssd->write(address, data);
    }
    else if (operation == "R") {
        int address = std::stoi(argv[2]);
        if (argc < 3) {
            std::cout << "Insufficient arguments" << std::endl;
            return 1;
        }
        string data = ssd->read(address);
        std::cout << "Read data: " << data << std::endl;
        // write data to result.txt(always overwrite)
		std::ofstream file("result.txt");
		file << data;
		file.close();
    }
    else if (operation == "E") {
        int address = std::stoi(argv[2]);
        if (argc < 4) {
            std::cout << "Insufficient arguments for erase operation" << std::endl;
            return 1;
        }
        int size = std::stoi(argv[3]);
        std::cout << "Erase data from " << address << " to " << address + size -1 << std::endl;
        ssd->erase(address, size);
    }
    else if (operation == "F") {
        if (argc < 2) {
            std::cout << "Insufficient arguments for erase operation" << std::endl;
            return 1;
        }
        std::cout << "Flush data" << std::endl;
        ssd->flush();
    }
    else {
        std::cout << "Invalid operation" << std::endl;
    }

    delete ssd;
    return 0;
}
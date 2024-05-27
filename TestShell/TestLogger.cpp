#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <string>
#include <algorithm>

class Logger {
public:
	Logger() {
		std::fstream fs(getLogPath(loggerFile), std::fstream::out);
		fs.close();
	}

	std::string getCurrentTime() {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80]{};

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "[%g.%m.%d %R]", timeinfo);
		return std::string(buffer);
	}

	std::string getCallerName(const std::string& callerName) {
		const int fixedFunctionLength = 30;
		char outBuffer[fixedFunctionLength + 1]{};
		std::fill_n(outBuffer, fixedFunctionLength, 0x20);
		strcpy(outBuffer, callerName.c_str());
		if (callerName.size() < fixedFunctionLength)
			outBuffer[callerName.size()] = 0x20;
		outBuffer[fixedFunctionLength] = 0x0;
		return std::string(" ") + outBuffer;
	}

	std::string getLogMessage(const std::string& logMessage) {
		return std::string(": ") + logMessage;
	}

	void printConsoleAndFile(const std::string& loggerOutput) {
		std::cout << loggerOutput << std::endl;
		std::fstream fs(getLogPath(loggerFile), std::fstream::out | std::fstream::app);
		fs << loggerOutput << std::endl;
		fs.close();
	}

	void print(std::string logMessage, const char* str = __builtin_FUNCTION()) {
		manageLogFiles();

		std::string loggerOutput = getCurrentTime();
		loggerOutput += getCallerName(std::string(str) + "()");
		loggerOutput += getLogMessage(logMessage);
		printConsoleAndFile(loggerOutput);
	}

	std::ifstream::pos_type filesize() {
		std::ifstream in(getLogPath(loggerFile).c_str(), std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
	}

	void manageLogFiles() {
		auto loggerFileSize = filesize();
		if (loggerFileSize >= loggerMaxByte) {
			std::string backupFile = std::string("until_") + getLogCurrentTime() + ".log";
			std::rename(getLogPath(loggerFile).c_str(), getLogPath(backupFile).c_str());
		}
	}

	std::string getLogCurrentTime() {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80]{};

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%g%m%d_%Hh_%Mm_%Ss", timeinfo);
		return std::string(buffer);
	}

	std::string getLogPath(const std::string& logFileName) {
		return loggerBase + "/" + logFileName;
	}

private:
	const int loggerMaxByte{ 1024 * 10 };
	std::string loggerBase{ "../log" };
	std::string loggerFile{ "latest.log" };
};

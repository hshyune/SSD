#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <string>
#include <algorithm>

enum class LOG_LEVEL {
	DEBUG,
	INFO,
	NO,
};

class LoggerSingleton {
public:
	static LoggerSingleton& getInstance(LOG_LEVEL logLevel = LOG_LEVEL::INFO) {
		static LoggerSingleton instance{};
		instance.setLoglevel(logLevel);
		return instance;
	}
private:
	LoggerSingleton() {
	}
	LoggerSingleton& operator=(const LoggerSingleton& other) = delete;
	LoggerSingleton(const LoggerSingleton& other) = delete;
public:
	void setLoglevel(LOG_LEVEL logLevel) {
		this->logLevel = logLevel;
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

	std::string getLogCurrentTime() {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80]{};

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%g%m%d_%Hh_%Mm_%Ss", timeinfo);
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

	void printConsoleAndFile(const std::string& logOutput) {
		printConsole(logOutput);
		printFile(logOutput);
	}

	void printConsole(const std::string& logOutput) {
		std::cout << logOutput << std::endl;
	}

	void printFile(const std::string& logOutput) {
		std::fstream fs(getLogPath(logFile), std::fstream::out | std::fstream::app);
		fs << logOutput << std::endl;
		fs.close();
	}

	void print(std::string logMessage, const char* str = __builtin_FUNCTION()) {
		if (logLevel == LOG_LEVEL::NO)
			return;
		manageLogFiles();

		std::string logOutput = getCurrentTime();
		logOutput += getCallerName(std::string(str) + "()");
		logOutput += getLogMessage(logMessage);
		printConsoleAndFile(logOutput);
	}

	std::ifstream::pos_type filesize() {
		std::ifstream in(getLogPath(logFile).c_str(), std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
	}

	void manageLogFiles() {
		auto logFileSize = filesize();
		if (logFileSize >= logMaxSize) {
			makeNewUntilLog();
		}
	}

	void makeNewUntilLog() {
		latestBackupFile = std::string("until_") + getLogCurrentTime();
		std::string baselogFile = getLogPath(logFile);
		std::string backuplogFile = getLogPath(latestBackupFile + ".log");

		try {
			int result = std::rename(baselogFile.c_str(), backuplogFile.c_str());
			if (result != 0) {
				throw std::runtime_error(std::string("[Rename Error] : ") + baselogFile + " -> " + backuplogFile);
			}
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
			exit(1);
		}
	}

	std::string getLogPath(const std::string& logFileName) {
		return logBase + "/" + logFileName;
	}

private:
	const int logMaxSize{ 1024 * 10 };
	std::string logBase{ "../log" };
	std::string logFile{ "latest.log" };
	std::string latestBackupFile;
	LOG_LEVEL logLevel{ LOG_LEVEL::INFO };
};

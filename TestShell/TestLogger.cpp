#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum class LOG_LEVEL {
	DEBUG,
	INFO,
	NO,
};

class LoggerSingleton {
public:
	static LoggerSingleton& getInstance(LOG_LEVEL logLevel = LOG_LEVEL::INFO, bool cleanLog = false) {
		static LoggerSingleton instance{};
		instance.setLoglevel(logLevel);
		instance.runCleanLog(cleanLog, { "log", "zip" });
		return instance;
	}
private:
	LoggerSingleton() {}
	LoggerSingleton& operator=(const LoggerSingleton& other) = delete;
	LoggerSingleton(const LoggerSingleton& other) = delete;
public:
	void setLoglevel(LOG_LEVEL logLevel) {
		this->logLevel = logLevel;
	}

	static void runCleanLog(bool cleanLog, const std::vector<std::string>& cleanExtensionLists) {
		if (cleanLog == false)
			return;
		try {
			removeExtension(makeStringExtensionLists(cleanExtensionLists));
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
			exit(1);
		}
	}

	static std::string makeStringExtensionLists(const std::vector<std::string>& cleanExtensionLists) {
		std::string cleanExtLists;
		std::string logBase{ "../log" };
		for (const std::string& cleanExtension : cleanExtensionLists) {
			cleanExtLists += logBase + "/*." + cleanExtension + " ";
		}
		return cleanExtLists;
	}

	static void removeExtension(const std::string& clean_ExtLists) {
		std::string git_rm{ "\"C:/Program Files/Git/usr/bin/rm.exe\"" };
		std::string cleanCommand = git_rm + " -f " + clean_ExtLists;
		int result = std::system(cleanCommand.c_str());
		if (result != 0) {
			throw std::runtime_error(std::string("[Clean Error] : ") + cleanCommand);
		}
	}

	void print(std::string logMessage, const char* str = __builtin_FUNCTION()) {
		if (logLevel == LOG_LEVEL::NO)
			return;
		manageLogFiles();

		std::string logOutput = getCurrentTime("[%g.%m.%d %R]");
		logOutput += getCallerName(std::string(str) + "()");
		logOutput += getLogMessage(logMessage);
		printConsoleAndFile(logOutput);
		LogIterationDebugger();
	}

	void manageLogFiles() {
		auto logFileSize = filesize();
		if (logFileSize >= logMaxSize) {
			if (untilFileExist) {
				makeZipLog();
			}
			makeNewUntilLog();
		}
	}

	std::ifstream::pos_type filesize() {
		std::ifstream in(getLogPath(logFile).c_str(), std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
	}

	void makeZipLog() {
		std::string logFile = getLogPath(latestBackupFile + ".log");
		std::string zipFile = getLogPath(latestBackupFile) + ".zip";
		renameSrcToDst(logFile, zipFile);
	}

	void renameSrcToDst(std::string& srcFile, std::string& dstFile) {
		try {
			int result = std::rename(srcFile.c_str(), dstFile.c_str());
			if (result != 0) {
				throw std::runtime_error(std::string("[Rename Error] : ") + srcFile + " -> " + dstFile);
			}
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
			exit(1);
		}
	}

	void makeNewUntilLog() {
		latestBackupFile = std::string("until_") + getCurrentTime("%g%m%d_%Hh_%Mm_%Ss");
		std::string baselogFile = getLogPath(logFile);
		std::string backuplogFile = getLogPath(latestBackupFile + ".log");
		renameSrcToDst(baselogFile, backuplogFile);
		untilFileExist = true;
	}

	std::string getLogPath(const std::string& logFileName) {
		return logBase + "/" + logFileName;
	}

	std::string getCurrentTime(const std::string& timeFormat) {
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80]{};

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, timeFormat.c_str(), timeinfo);
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

	void LogIterationDebugger(const char* str = __builtin_FUNCTION()) {
		if (logLevel == LOG_LEVEL::DEBUG) {
			static int count = 0;
			std::cout << "========================== [" << str << "] " << count++ << " Iteration \n";
			listLogOutputDirectory();
			std::cout << "========================== \n";
		}
	}

	void listLogOutputDirectory() {
		std::string findLogFile = git_ls + " " + logBase;
		system(findLogFile.c_str());
	}

private:
	bool untilFileExist{ false };
	const int logMaxSize{ 1024 * 10 };
	std::string logBase{ "../log" };
	std::string logFile{ "latest.log" };
	std::string latestBackupFile;
	std::string git_ls{ "\"C:/Program Files/Git/usr/bin/ls.exe\"" };
	LOG_LEVEL logLevel{ LOG_LEVEL::INFO };
};
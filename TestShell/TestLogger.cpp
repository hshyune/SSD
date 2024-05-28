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

/////////////////////////////////////////////////////////////////////////////////

class LogEnv {
public:
	static const std::string& getLogBase() {
		return logBase;
	}

	void setLoglevel(LOG_LEVEL logLevel) {
		this->logLevel = logLevel;
	}

	LOG_LEVEL getLogLevel() {
		return logLevel;
	}

	const std::string& getLogFile() {
		return logFile;
	}

private:
	const static std::string logBase;
	const std::string logFile{ "latest.log" };
	LOG_LEVEL logLevel{ LOG_LEVEL::INFO };
};

const std::string LogEnv::logBase{ "../log" };

/////////////////////////////////////////////////////////////////////////////////

class LogUtils : public LogEnv {
public:
	std::string getCurrentTime(const std::string& timeFormat) {
		time_t rawtime;
		time(&rawtime);
		struct tm* timeinfo = localtime(&rawtime);
		char buffer[80]{};
		strftime(buffer, 80, timeFormat.c_str(), timeinfo);
		return std::string(buffer);
	}

	std::string getLogPath(const std::string& logFileName) {
		return getLogBase() + "/" + logFileName;
	}

	std::ifstream::pos_type filesize() {
		std::ifstream in(getLogPath(getLogFile()).c_str(), std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
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
};

/////////////////////////////////////////////////////////////////////////////////

class LogManager : public LogUtils {
public:
	void manageLogFiles() {
		auto logFileSize = filesize();
		if (logFileSize >= logMaxSize) {
			if (untilFileExist) {
				makeZipLog();
			}
			makeBackupLog();
		}
	}

	void makeZipLog() {
		std::string logFile = getLogPath(latestBackupFile + ".log");
		std::string zipFile = getLogPath(latestBackupFile) + ".zip";
		renameSrcToDst(logFile, zipFile);
	}

	void makeBackupLog() {
		latestBackupFile = std::string("until_") + getCurrentTime("%g%m%d_%Hh_%Mm_%Ss");
		std::string baselogFile = getLogPath(getLogFile());
		std::string backuplogFile = getLogPath(latestBackupFile + ".log");
		renameSrcToDst(baselogFile, backuplogFile);
		untilFileExist = true;
	}

private:
	bool untilFileExist{ false };
	const int logMaxSize{ 1024 * 10 };
	std::string latestBackupFile;
};

/////////////////////////////////////////////////////////////////////////////////

class LogCleaner {
public:
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
		for (const std::string& cleanExtension : cleanExtensionLists) {
			cleanExtLists += LogEnv::getLogBase() + "/*." + cleanExtension + " ";
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
};

/////////////////////////////////////////////////////////////////////////////////

class LogDebugger : public LogEnv {
public:
	void debugListLogFile(const char* callerName = __builtin_FUNCTION()) {
		if (getLogLevel() == LOG_LEVEL::DEBUG) {
			static int count = 0;
			std::cout << "========================== [" << callerName << "] " << count++ << " Iteration \n";
			listLogOutputDirectory();
			std::cout << "========================== \n";
		}
	}

	void listLogOutputDirectory()
	{
		std::string findLogFile = git_ls + " " + getLogBase();
		system(findLogFile.c_str());
	}

private:
	std::string git_ls{ "\"C:/Program Files/Git/usr/bin/ls.exe\"" };
};

/////////////////////////////////////////////////////////////////////////////////

class LoggerSingleton : public LogEnv {
public:
	static LoggerSingleton& getInstance(LOG_LEVEL logLevel = LOG_LEVEL::INFO, bool cleanLog = false) {
		static LoggerSingleton instance{};
		instance.setLoglevel(logLevel);
		instance.cleaner.runCleanLog(cleanLog, { "log", "zip" });
		return instance;
	}
private:
	LoggerSingleton() {}
	LoggerSingleton& operator=(const LoggerSingleton& other) = delete;
	LoggerSingleton(const LoggerSingleton& other) = delete;

public:
	void print(std::string logMessage, const char* callerName = __builtin_FUNCTION()) {
		if (getLogLevel() == LOG_LEVEL::NO)
			return;
		manager.manageLogFiles();

		std::string logOutput = getCurrentTime("[%g.%m.%d %R]");
		logOutput += getCallerName(std::string(callerName) + "()");
		logOutput += getLogMessage(logMessage);
		printConsoleAndFile(logOutput);
		debugger.debugListLogFile();
	}

	std::string getLogPath(const std::string& logFileName) {
		return getLogBase() + "/" + logFileName;
	}

	std::string getCurrentTime(const std::string& timeFormat) {
		time_t rawtime;
		time(&rawtime);
		struct tm* timeinfo = localtime(&rawtime);
		char buffer[80]{};
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
		std::fstream fs(getLogPath(getLogFile()), std::fstream::out | std::fstream::app);
		fs << logOutput << std::endl;
		fs.close();
	}

private:
	LogManager manager;
	LogCleaner cleaner;
	LogDebugger debugger;
};
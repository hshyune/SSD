#pragma once
#include <fstream>
#include <string>
#include <vector>

enum class LOG_LEVEL {
	DEBUG,
	INFO,
	NO,
};

/////////////////////////////////////////////////////////////////////////////////

class LogEnv {
public:
	static const std::string& getLogBase();
	void setLoglevel(LOG_LEVEL logLevel);
	LOG_LEVEL getLogLevel();
	const std::string& getLogFile();

private:
	const static std::string logBase;
	const std::string logFile{ "latest.log" };
	LOG_LEVEL logLevel{ LOG_LEVEL::INFO };
};

/////////////////////////////////////////////////////////////////////////////////

class LogUtils : public LogEnv {
public:
	std::string getCurrentTime(const std::string& timeFormat);
	std::string getLogPath(const std::string& logFileName);
	std::ifstream::pos_type filesize();
	void renameSrcToDst(std::string& srcFile, std::string& dstFile);
};

/////////////////////////////////////////////////////////////////////////////////

class LogManager : public LogUtils {
public:
	void manageLogFiles();
	void makeZipLog();
	void makeBackupLog();

private:
	bool untilFileExist{ false };
	const int logMaxSize{ 1024 * 10 };
	std::string latestBackupFile;
};

/////////////////////////////////////////////////////////////////////////////////

class LogCleaner {
public:
	static void runCleanLog(bool cleanLog, const std::vector<std::string>& cleanExtensionLists);
	static std::string makeStringExtensionLists(const std::vector<std::string>& cleanExtensionLists);
	static void removeExtension(const std::string& clean_ExtLists);
};

/////////////////////////////////////////////////////////////////////////////////

class LogDebugger : public LogEnv {
public:
	void debugListLogFile(const char* callerName = __builtin_FUNCTION());
	void listLogOutputDirectory();

private:
	const std::string git_ls{ "\"C:/Program Files/Git/usr/bin/ls.exe\"" };
};

/////////////////////////////////////////////////////////////////////////////////

class LoggerSingleton : public LogUtils {
public:
	static LoggerSingleton& getInstance(LOG_LEVEL logLevel = LOG_LEVEL::INFO, bool cleanLog = false);

private:
	LoggerSingleton() {}
	LoggerSingleton& operator=(const LoggerSingleton& other) = delete;
	LoggerSingleton(const LoggerSingleton& other) = delete;

public:
	void print(std::string logMessage, const char* callerName = __builtin_FUNCTION());
	std::string getCallerName(const std::string& callerName);
	std::string getLogMessage(const std::string& logMessage);
	void printConsoleAndFile(const std::string& logOutput);
	void printConsole(const std::string& logOutput);
	void printFile(const std::string& logOutput);

private:
	LogManager manager;
	LogCleaner cleaner;
	LogDebugger debugger;
};

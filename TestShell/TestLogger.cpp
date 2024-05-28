#pragma once
#include "TestLogger.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


/////////////////////////////////////////////////////////////////////////////////

const std::string& LogEnv::getLogBase() {
	return logBase;
}

void LogEnv::setLoglevel(LOG_LEVEL logLevel) {
	this->logLevel = logLevel;
}

LOG_LEVEL LogEnv::getLogLevel() {
	return logLevel;
}

const std::string& LogEnv::getLogFile() {
	return logFile;
}

const std::string LogEnv::logBase{ "../log" };

/////////////////////////////////////////////////////////////////////////////////

std::string LogUtils::getCurrentTime(const std::string& timeFormat) {
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo = localtime(&rawtime);
	char buffer[80]{};
	strftime(buffer, 80, timeFormat.c_str(), timeinfo);
	return std::string(buffer);
}

std::string LogUtils::getLogPath(const std::string& logFileName) {
	return getLogBase() + "/" + logFileName;
}

std::ifstream::pos_type LogUtils::filesize() {
	std::ifstream in(getLogPath(getLogFile()).c_str(), std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

void LogUtils::renameSrcToDst(std::string& srcFile, std::string& dstFile) {
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

/////////////////////////////////////////////////////////////////////////////////

void LogManager::manageLogFiles() {
	auto logFileSize = filesize();
	if (logFileSize >= logMaxSize) {
		if (untilFileExist) {
			makeZipLog();
		}
		makeBackupLog();
	}
}

void LogManager::makeZipLog() {
	std::string logFile = getLogPath(latestBackupFile + ".log");
	std::string zipFile = getLogPath(latestBackupFile) + ".zip";
	renameSrcToDst(logFile, zipFile);
}

void LogManager::makeBackupLog() {
	latestBackupFile = std::string("until_") + getCurrentTime("%g%m%d_%Hh_%Mm_%Ss");
	std::string baselogFile = getLogPath(getLogFile());
	std::string backuplogFile = getLogPath(latestBackupFile + ".log");
	renameSrcToDst(baselogFile, backuplogFile);
	untilFileExist = true;
}

/////////////////////////////////////////////////////////////////////////////////

void LogCleaner::runCleanLog(bool cleanLog, const std::vector<std::string>& cleanExtensionLists) {
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

std::string LogCleaner::makeStringExtensionLists(const std::vector<std::string>& cleanExtensionLists) {
	std::string cleanExtLists;
	for (const std::string& cleanExtension : cleanExtensionLists) {
		cleanExtLists += LogEnv::getLogBase() + "/*." + cleanExtension + " ";
	}
	return cleanExtLists;
}

void LogCleaner::removeExtension(const std::string& clean_ExtLists) {
	std::string git_rm{ "\"C:/Program Files/Git/usr/bin/rm.exe\"" };
	std::string cleanCommand = git_rm + " -f " + clean_ExtLists;
	int result = std::system(cleanCommand.c_str());
	if (result != 0) {
		throw std::runtime_error(std::string("[Clean Error] : ") + cleanCommand);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void LogDebugger::debugListLogFile(const char* callerName) {
	if (getLogLevel() == LOG_LEVEL::DEBUG) {
		static int count = 0;
		std::cout << "========================== [" << callerName << "] " << count++ << " Iteration \n";
		listLogOutputDirectory();
		std::cout << "========================== \n";
	}
}

void LogDebugger::listLogOutputDirectory() {
	std::string findLogFile = git_ls + " " + getLogBase();
	system(findLogFile.c_str());
}

/////////////////////////////////////////////////////////////////////////////////

LoggerSingleton& LoggerSingleton::getInstance(LOG_LEVEL logLevel, bool cleanLog) {
	static LoggerSingleton instance{};
	instance.setLoglevel(logLevel);
	instance.cleaner.runCleanLog(cleanLog, { "log", "zip" });
	return instance;
}

void LoggerSingleton::print(std::string logMessage, const char* callerName) {
	if (getLogLevel() == LOG_LEVEL::NO)
		return;
	manager.manageLogFiles();

	std::string logOutput = getCurrentTime("[%g.%m.%d %R]");
	logOutput += getCallerName(std::string(callerName) + "()");
	logOutput += getLogMessage(logMessage);
	printConsoleAndFile(logOutput);
	debugger.debugListLogFile();
}

std::string LoggerSingleton::getCallerName(const std::string& callerName) {
	const int fixedFunctionLength = 30;
	char outBuffer[fixedFunctionLength + 1]{};
	std::fill_n(outBuffer, fixedFunctionLength, 0x20);
	strcpy(outBuffer, callerName.c_str());
	if (callerName.size() < fixedFunctionLength)
		outBuffer[callerName.size()] = 0x20;
	outBuffer[fixedFunctionLength] = 0x0;
	return std::string(" ") + outBuffer;
}

std::string LoggerSingleton::getLogMessage(const std::string& logMessage) {
	return std::string(": ") + logMessage;
}

void LoggerSingleton::printConsoleAndFile(const std::string& logOutput) {
	printConsole(logOutput);
	printFile(logOutput);
}

void LoggerSingleton::printConsole(const std::string& logOutput) {
	std::cout << logOutput << std::endl;
}

void LoggerSingleton::printFile(const std::string& logOutput) {
	std::fstream fs(getLogPath(getLogFile()), std::fstream::out | std::fstream::app);
	fs << logOutput << std::endl;
	fs.close();
}

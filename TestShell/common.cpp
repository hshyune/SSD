#pragma once
#include <windows.h>
#include <atlconv.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

std::string getCurPath() {
	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);

	USES_CONVERSION;
	std::string str = W2A(path);
	str = str.substr(0, str.find_last_of("\\/"));

	return str;
}

std::string getResourcePath() {
	string exePath = getCurPath();
	// split string
	istringstream iss(exePath);
	string tmp;
	vector<string> args;
	while (getline(iss, tmp, '\\')) {
		args.push_back(tmp);
	}
	string resourcePath = "";
	for (int i = 0; i < args.size() - 2; i++) {
		resourcePath += args.at(i) + "\\";
	}
	resourcePath += "resources";

	return resourcePath;
}

std::string getSolutionPath() {
	string exePath = getCurPath();
	// split string
	istringstream iss(exePath);
	string tmp;
	vector<string> args;
	while (getline(iss, tmp, '\\')) {
		args.push_back(tmp);
	}
	string solutionPath = "";
	for (int i = 0; i < args.size() - 2; i++) {
		solutionPath += args.at(i) + "\\";
	}

	return solutionPath;
}
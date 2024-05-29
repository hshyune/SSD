#include "TestShell.cpp"
#include "TestScript.cpp"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		TestShell testShell;
		testShell.run();
		return 1;
	}
	std::string str(argv[1]);
	if (str == "run_list.lst") {
		TestScriptRunner testScriptRunner;
		testScriptRunner.runTestListUsingFile(PATH_TESTLIST_FILE);
	}
	else {
		TestScriptRunner testScriptRunner;
		testScriptRunner.runTC(str);
	}
	return 1;
}
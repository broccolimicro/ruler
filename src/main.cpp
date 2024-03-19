#include <iostream>
#include <string>
#include <vector>

#include <ruler/Tech.h>

using namespace std;

void printHelp() {
	printf("Usage: ruler [options] <rules.py> <file.gds>...\n");
	printf("Check a GDS file against design rules.\n");
	printf("\nOptions:\n");
	printf(" -h,--help      Display this information\n");
	printf("    --version   Display version information\n");
}

void printVersion() {
	printf("ruler 0.0.0\n");
	printf("Copyright (C) 2024 Broccoli, LLC.\n");
	printf("There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
	printf("\n");
}

int main(int argc, char **argv) {
	string rulePath = "";
	string outputDir = "";
	vector<string> gdsPaths;
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "--help" or arg == "-h") {
			printHelp();
			return 0;
		} else if (arg == "--version") {
			printVersion();
			return 0;
		} else if (rulePath.size() == 0) {
			rulePath = argv[i];
		} else {
			gdsPaths.push_back(argv[i]);
		}
	}

	if (gdsPaths.size() == 0) {
		printHelp();
		return 0;
	}
}


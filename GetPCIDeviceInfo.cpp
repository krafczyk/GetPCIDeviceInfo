#include <cstdio>
#include <string>

#include "ArgParse/ArgParse.h"

#include "pci/pci.h"

int main(int argc, char** argv) {
	std::string directory = "";

	ArgParse::ArgParser Parser("Program to print PCI Device information inside a directory");
	Parser.AddArgument("-d/--directory", "The directory from which to get PCI information", &directory, ArgParse::Argument::Required);
	if (Parser.ParseArgs(argc, argv) < 0) {
		fprintf(stderr, "There was a problem parsing the arguments!");
		return -1;
	}

	printf("The directory passed was: (%s)\n", directory.c_str());

	return 0;
}

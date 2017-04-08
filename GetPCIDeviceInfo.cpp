#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <sstream>

#include "ArgParse/ArgParse.h"

#include "pci/pci.h"

int get_hex_number_from_file(const std::string& filepath, u16& hex_number) {
	if (access( filepath.c_str(), F_OK) == -1) {
		fprintf(stderr, "File (%s) doesn't exist\n", filepath.c_str());
		return -1;
	}

	std::string line;

	std::ifstream input_file(filepath, std::ifstream::in);

	if(! std::getline(input_file, line)) {
		fprintf(stderr, "Could not get line from File (%s)\n", filepath.c_str());
		input_file.close();
		return -2;
	}

	input_file.close();

	std::stringstream ss;
	ss << std::hex << line;
	ss >> hex_number;

	return 0;
}

int get_vendor_id(const std::string& directory, u16& vendor_id) {
	std::string vendor_filepath = directory + "/vendor";

	if (get_hex_number_from_file(vendor_filepath, vendor_id) < 0) {
		fprintf(stderr, "There was a problem getting the vendor ID from the file\n");
		vendor_id = 0;
		return -1;
	}
	return 0;
}

int main(int argc, char** argv) {
	std::string directory = "";

	ArgParse::ArgParser Parser("Program to print PCI Device information inside a directory");
	Parser.AddArgument("-d/--directory", "The directory from which to get PCI information", &directory, ArgParse::Argument::Required);
	if (Parser.ParseArgs(argc, argv) < 0) {
		fprintf(stderr, "There was a problem parsing the arguments!\n ");
		return -1;
	}

	printf("The directory passed was: (%s)\n", directory.c_str());

	struct pci_access *pacc;
	pacc = pci_alloc();
	pci_init(pacc);

	u16 vendor_id = 0;

	if( get_vendor_id(directory, vendor_id) < 0) {
		fprintf(stderr, "Couldn't get vendor id!");
		return -2;
	}

	printf("The vendor id was: (%i)\n", vendor_id);

	pci_cleanup(pacc);

	return 0;
}

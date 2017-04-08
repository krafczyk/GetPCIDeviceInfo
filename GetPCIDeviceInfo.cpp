#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <sstream>

#include "ArgParse/ArgParse.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "pci/pci.h"

#ifdef __cplusplus
}
#endif

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

int get_device_id(const std::string& directory, u16& device_id) {
	std::string device_filepath = directory + "/device";

	if (get_hex_number_from_file(device_filepath, device_id) < 0) {
		fprintf(stderr, "There was a problem getting the device ID from the file\n");
		device_id = 0;
		return -1;
	}
	return 0;
}

int get_class_id(const std::string& directory, u16& class_id) {
	std::string class_filepath = directory + "/class";

	if (get_hex_number_from_file(class_filepath, class_id) < 0) {
		fprintf(stderr, "There was a problem getting the device ID from the file\n");
		class_id = 0;
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

	struct pci_access *pacc;
	pacc = pci_alloc();
	pci_init(pacc);

	u16 vendor_id = 0;

	if( get_vendor_id(directory, vendor_id) < 0) {
		fprintf(stderr, "Couldn't get vendor id!\n");
		return -2;
	}

	u16 device_id = 0;

	if( get_device_id(directory, device_id) < 0) {
		fprintf(stderr, "Couldn't get device id!\n");
		return -2;
	}

	u16 class_id = 0;
	if( get_class_id(directory, class_id) < 0) {
		fprintf(stderr, "Couldn't get class id!\n");
		return -3;
	}

	size_t PCI_STRING_BUFF_SIZE=1024;
	char vendor_name[PCI_STRING_BUFF_SIZE];

	pci_lookup_name(pacc, vendor_name, sizeof(vendor_name), PCI_LOOKUP_VENDOR, vendor_id);

	char device_name[PCI_STRING_BUFF_SIZE];

	pci_lookup_name(pacc, device_name, sizeof(device_name), PCI_LOOKUP_DEVICE, vendor_id, device_id);

	char class_name[PCI_STRING_BUFF_SIZE];

	pci_lookup_name(pacc, class_name, sizeof(class_name), PCI_LOOKUP_CLASS, class_id);

	//printf("This device's information is:\n");
	//printf("Class: (%s)\n", class_name);
	printf("Vendor: (%s)\n", vendor_name);
	printf("Device: (%s)\n", device_name);

	pci_cleanup(pacc);

	return 0;
}

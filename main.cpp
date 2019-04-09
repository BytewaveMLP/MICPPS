#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <iomanip>

#include "MipsCPU.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " file.bin" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream asmFile(argv[1], std::ios::binary | std::ios::ate);

	if (!asmFile.is_open()) {
		std::cerr << "Failed to open input file " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	std::streamsize asmSize = asmFile.tellg();

	if (asmSize % 4 != 0) {
		std::cerr << "Malformed input assembly - file size must be divisible by 4." << std::endl;
		return EXIT_FAILURE;
	}

	asmFile.seekg(0, std::ios::beg);

	std::vector<char> asmRaw(asmSize);

	if (!asmFile.read(asmRaw.data(), asmSize)) {
		std::cerr << "Failed to read input data!" << std::endl;
		return EXIT_FAILURE;
	}

	MipsCPU cpu(asmRaw, 4096);

	cpu.run_until_complete();

	return EXIT_SUCCESS;
}
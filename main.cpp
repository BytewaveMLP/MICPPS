#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <iomanip>

#define REG_ZERO 0
#define REG_AT   1
#define REG_V0   2
#define REG_V1   3
#define REG_A0   4
#define REG_A1   5
#define REG_A2   6
#define REG_A3   7
#define REG_T0   8
#define REG_T1   9
#define REG_T2   10
#define REG_T3   11
#define REG_T4   12
#define REG_T5   13
#define REG_T6   14
#define REG_T7   15
#define REG_S0   16
#define REG_S1   17
#define REG_S2   18
#define REG_S3   19
#define REG_S4   20
#define REG_S5   21
#define REG_S6   22
#define REG_S7   23
#define REG_T8   24
#define REG_T9   25
#define REG_K0   26
#define REG_K1   27
#define REG_GP   28
#define REG_SP   29
#define REG_FP   30
#define REG_RA   31

void invalid_instruction_err(uint32_t inst, uint32_t pc) {
	std::cerr << "INVALID INSTRUCTION:\n"
	          << std::showbase << std::hex
	          << "    PC   = " << pc << "\n"
	          << "    INST = " << inst << "\n";
}

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

	std::array<uint32_t, 32> registers = { 0 };

	if (!asmFile.read(asmRaw.data(), asmSize)) {
		std::cerr << "Failed to read input data!" << std::endl;
		return EXIT_FAILURE;
	}

	for (uint32_t PC = 0; PC < asmSize; PC += 4) {
		uint32_t inst = (asmRaw[PC] << 24) | (asmRaw[PC+1] << 16) | (asmRaw[PC+2] << 8) | (asmRaw[PC+3]);
		uint8_t  op   = inst >> 26u;

		switch(op) {
			case 0: { // R-type
				uint8_t rs    = inst >> 21 & 0b00011111;
				uint8_t rt    = inst >> 16 & 0b00011111;
				uint8_t rd    = inst >> 11 & 0b00011111;
				uint8_t shant = inst >> 6  & 0b00011111;
				uint8_t funct = inst       & 0b00111111;

				switch (funct) {
					case 0x20: { // ADD
						std::cout << std::dec << std::showbase
						          << "ADD\n"
						          << "RD = " << unsigned(rd) << "\n"
						          << "RS = " << unsigned(rs) << "\n"
						          << "RT = " << unsigned(rt) << std::endl;
						registers[rd] = registers[rs] + registers[rt];
					} break;
					default: {
						invalid_instruction_err(inst, PC);
						std::cerr << std::hex << std::showbase
						          << "    RS = " << unsigned(rs) << "\n"
						          << "    RT = " << unsigned(rt) << "\n"
						          << "    RD = " << unsigned(rd) << "\n"
						          << "    SHANT = " << unsigned(shant) << "\n"
						          << "    FUNCT = " << unsigned(funct) << std::endl;
						return EXIT_FAILURE;
					}
				}
			} break;
			case 2: case 3: { // J-type
				PC = ((inst & 0b00000011'11111111'11111111'11111111 << 2) | ((PC+4) & 0xf0000000)) - 4;
			} break;
		}
	}

	return EXIT_SUCCESS;
}
#ifndef MICPPS_MIPSCPU_H
#define MICPPS_MIPSCPU_H

#include <vector>
#include <array>
#include <cinttypes>
#include <iostream>
#include <iomanip>

#include "mipstypes.h"

#define MIPS_REGISTER_ZERO 0
#define MIPS_REGISTER_AT   1
#define MIPS_REGISTER_V0   2
#define MIPS_REGISTER_V1   3
#define MIPS_REGISTER_A0   4
#define MIPS_REGISTER_A1   5
#define MIPS_REGISTER_A2   6
#define MIPS_REGISTER_A3   7
#define MIPS_REGISTER_T0   8
#define MIPS_REGISTER_T1   9
#define MIPS_REGISTER_T2   10
#define MIPS_REGISTER_T3   11
#define MIPS_REGISTER_T4   12
#define MIPS_REGISTER_T5   13
#define MIPS_REGISTER_T6   14
#define MIPS_REGISTER_T7   15
#define MIPS_REGISTER_S0   16
#define MIPS_REGISTER_S1   17
#define MIPS_REGISTER_S2   18
#define MIPS_REGISTER_S3   19
#define MIPS_REGISTER_S4   20
#define MIPS_REGISTER_S5   21
#define MIPS_REGISTER_S6   22
#define MIPS_REGISTER_S7   23
#define MIPS_REGISTER_T8   24
#define MIPS_REGISTER_T9   25
#define MIPS_REGISTER_K0   26
#define MIPS_REGISTER_K1   27
#define MIPS_REGISTER_GP   28
#define MIPS_REGISTER_SP   29
#define MIPS_REGISTER_FP   30
#define MIPS_REGISTER_RA   31

class MipsCPU {
private:
	std::vector<char>       code;
	std::vector<char>       memory;
	std::array<int32_t, 32> registers;

	uint32_t PC = 0;

	uint32_t do_rtype(mips::instruction_types::rtype inst);
	uint32_t do_jtype(mips::instruction_types::jtype inst);
	uint32_t do_itype(mips::instruction_types::itype inst);

	int32_t& reg(int index) {
		return registers[index];
	}

public:
	MipsCPU(const std::vector<char>& code, unsigned int memSize);

	void step();

	void run_until_complete() {
		while (PC < code.size()) step();
	}

	void dump_registers(int base = 10) {
		std::cout << std::setbase(base) << std::showbase
		          << "REGISTERS\n"
		          << "---------\n"
		          << "AT = " << reg(MIPS_REGISTER_AT) << "\t"
		          << "V0 = " << reg(MIPS_REGISTER_V0) << "\t"
		          << "V1 = " << reg(MIPS_REGISTER_V1) << "\t"
		          << "A0 = " << reg(MIPS_REGISTER_A0) << "\n"
				  << "A1 = " << reg(MIPS_REGISTER_A1) << "\t"
				  << "A2 = " << reg(MIPS_REGISTER_A2) << "\t"
				  << "A3 = " << reg(MIPS_REGISTER_A3) << "\t"
				  << "T0 = " << reg(MIPS_REGISTER_T0) << "\n"
				  << "T1 = " << reg(MIPS_REGISTER_T1) << "\t"
				  << "T2 = " << reg(MIPS_REGISTER_T2) << "\t"
				  << "T3 = " << reg(MIPS_REGISTER_T3) << "\t"
				  << "T4 = " << reg(MIPS_REGISTER_T4) << "\n"
				  << "T5 = " << reg(MIPS_REGISTER_T5) << "\t"
				  << "T6 = " << reg(MIPS_REGISTER_T6) << "\t"
				  << "T7 = " << reg(MIPS_REGISTER_T7) << "\t"
				  << "S0 = " << reg(MIPS_REGISTER_S0) << "\n"
				  << "S1 = " << reg(MIPS_REGISTER_S1) << "\t"
				  << "S2 = " << reg(MIPS_REGISTER_S2) << "\t"
				  << "S3 = " << reg(MIPS_REGISTER_S3) << "\t"
				  << "S4 = " << reg(MIPS_REGISTER_S4) << "\n"
				  << "S5 = " << reg(MIPS_REGISTER_S5) << "\t"
				  << "S6 = " << reg(MIPS_REGISTER_S6) << "\t"
				  << "S7 = " << reg(MIPS_REGISTER_S7) << "\t"
				  << "T8 = " << reg(MIPS_REGISTER_T8) << "\n"
				  << "T9 = " << reg(MIPS_REGISTER_T9) << "\t"
				  << "K0 = " << reg(MIPS_REGISTER_K0) << "\t"
				  << "K1 = " << reg(MIPS_REGISTER_K1) << "\t"
				  << "GP = " << reg(MIPS_REGISTER_GP) << "\n"
				  << "SP = " << reg(MIPS_REGISTER_SP) << "\t"
				  << "FP = " << reg(MIPS_REGISTER_FP) << "\t"
				  << "RA = " << reg(MIPS_REGISTER_RA)
				  << std::endl;
	}
};

#endif //MICPPS_MIPSCPU_H

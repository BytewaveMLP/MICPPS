#include <sstream>

#include "MipsCPU.h"

MipsCPU::MipsCPU(const std::vector<char>& code, unsigned int memSize) : code(code), memory(std::vector<char>(memSize)), registers(std::array<int32_t, 32>()) {}

uint32_t MipsCPU::do_rtype(mips::instruction_types::rtype inst) {
	switch(inst.funct) {
		case 0x20: { // ADD
			reg(inst.rd) = reg(inst.rs) + reg(inst.rt);
		} break;
		case 0x22: { // SUB
			reg(inst.rd) = reg(inst.rs) - reg(inst.rt);
		} break;
		default: {
			std::stringstream ss;
			ss << "Invalid R-type instruction @ PC = " << PC;
			throw std::runtime_error(ss.str());
		}
	}

	return PC + 4;
}

uint32_t MipsCPU::do_jtype(mips::instruction_types::jtype inst) {
	if (inst.op == 0x03) reg(MIPS_REGISTER_RA) = PC;

	return (PC & 0xf0000000) | (inst.target << 2);
}

uint32_t MipsCPU::do_itype(mips::instruction_types::itype inst) {
	switch(inst.op) {
		case 0x0d: { // ORI
			reg(inst.rt) = reg(inst.rs) | inst.imm;
		} break;
		default: {
			std::stringstream ss;
			ss << "Invalid I-type instruction @ PC = " << PC;
			throw std::runtime_error(ss.str());
		}
	}

	return PC + 4;
}

void MipsCPU::step() {
	mips::instruction inst = {};
	inst.bytecode = (code[PC] << 24) | (code[PC+1] << 16) | (code[PC+2] << 8) | (code[PC+3]);

	uint8_t op = inst.bytecode >> 26u;

	if (op == 0x00) {
		PC = do_rtype(inst.rtype);
	} else if (op == 0x02 || op == 0x03) {
		PC = do_jtype(inst.jtype);
	} else {
		PC = do_itype(inst.itype);
	}

	reg(MIPS_REGISTER_ZERO) = 0; // lazy hack to avoid writing to $zero
}
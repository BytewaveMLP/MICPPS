#include "MipsCPU.h"

MipsCPU::MipsCPU(const std::vector<char>& code, unsigned int memSize) : code(code), memory(std::vector<char>(memSize)), registers(std::array<int32_t, 32>()) {}

uint32_t MipsCPU::do_rtype(mips::instruction_types::rtype inst) {
	switch(inst.funct) {
		case 0x08: {
			PC = reg(inst.rs) - 4;
		}
		case 0x20: case 21: { // ADD, ADDU
			reg(inst.rd) = reg(inst.rs) + reg(inst.rt);
		} break;
		case 0x22: case 0x23: { // SUB
			reg(inst.rd) = reg(inst.rs) - reg(inst.rt);
		} break;
		case 0x24: { // AND
			reg(inst.rd) = reg(inst.rs) & reg(inst.rt);
		} break;
		case 0x25: { // OR
			reg(inst.rd) = reg(inst.rs) | reg(inst.rt);
		} break;
		case 0x27: { // NOR
			reg(inst.rd) = ~(reg(inst.rs) | reg(inst.rt));
		} break;
		case 0x2a: { // SLT
			reg(inst.rd) = reg(inst.rs) < reg(inst.rt);
		} break;
		case 0x2b: { // SLTU
			reg(inst.rd) = ((uint32_t) reg(inst.rs)) < ((uint32_t) reg(inst.rt));
		} break;
		default: {
			throw std::runtime_error("Invalid R-type instruction @ PC = " + std::to_string(PC));
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
		case 0x04: { // BEQ
			if (reg(inst.rt) == reg(inst.rs)) PC += inst.imm;
		} break;
		case 0x05: { // BNE
			if (reg(inst.rt) != reg(inst.rs)) PC += inst.imm;
		} break;
		case 0x08: case 0x09: { // ADDI, ADDIU
			reg(inst.rt) = reg(inst.rs) + sign_extend(inst.imm);
		} break;
		case 0x0a: { // SLTI
			reg(inst.rt) = reg(inst.rs) < sign_extend(inst.imm);
		} break;
		case 0x0b: { // SLTIU
			reg(inst.rt) = ((uint32_t) reg(inst.rs)) < ((uint32_t) sign_extend(inst.imm));
		}
		case 0x0c: { // ANDI
			reg(inst.rt) = reg(inst.rs) & inst.imm;
		} break;
		case 0x0d: { // ORI
			reg(inst.rt) = reg(inst.rs) | inst.imm;
		} break;
		case 0x0f: { // LUI
			reg(inst.rt) = inst.imm << 16;
		}
		case 0x23: { // LW
			reg(inst.rt) = memory[reg(inst.rs) + sign_extend(inst.imm)];
		} break;
		case 0x24: { // LBU
			reg(inst.rt) = 0xff & memory[reg(inst.rs) + sign_extend(inst.imm)];
		} break;
		case 0x25: { // LHU
			reg(inst.rt) = 0xffff & memory[reg(inst.rs) + sign_extend(inst.imm)];
		} break;
		case 0x28: { // SB
			memory[reg(inst.rs) + sign_extend(inst.imm)] &= ~0xff;
			memory[reg(inst.rs) + sign_extend(inst.imm)] |= reg(inst.rt) & 0xff;
		} break;
		case 0x29: { // SH
			memory[reg(inst.rs) + sign_extend(inst.imm)] &= ~0xffff;
			memory[reg(inst.rs) + sign_extend(inst.imm)] |= reg(inst.rt) & 0xffff;
		} break;
		case 0x2b: { // SW
			memory[reg(inst.rs) + sign_extend(inst.imm)] = reg(inst.rt);
		} break;
		default: {
			throw std::runtime_error("Invalid I-type instruction @ PC = " + std::to_string(PC));
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
#ifndef CHIP8_INSTRUCTION_HANDLER_HPP
#define CHIP8_INSTRUCTION_HANDLER_HPP

#include "Chip8Common.hpp"

namespace Chip8 {
	enum Opcode {
		// CHIP-8 instructions
		op_CLS = 0,
		op_RET_ADDR,
		op_SYS_ADDR,
		op_JP_ADDR,
		op_CALL_ADDR,
		op_SE_VX_IMM,
		op_SNE_VX_IMM,
		op_SE_VX_VY,
		op_LD_VX_IMM,
		op_ADD_VX_IMM,
		op_LD_VX_VY,
		op_OR_VX_VY,
		op_AND_VX_VY,
		op_XOR_VX_VY,
		op_ADD_VX_VY,
		op_SUB_VX_VY,
		op_SHR_VX_VY,
		op_SUBN_VX_VY,
		op_SHL_VX_VY,
		op_SNE_VX_VY,
		op_LD_I_VY,
		op_JP_V0_ADDR,
		op_RND_VX_IMM,
		op_DRW_VX_VY_IMM,
		op_SKP_VX,
		op_SKNP_VX,
		op_LD_VX_DT,
		op_LD_VX_K,
		op_LD_DT_VX,
		op_LD_ST_VX,
		op_LD_F_VX,
		op_LD_B_VX,
		op_STORE_REGS,
		op_LOAD_REGS,

		// SUPER CHIP-8 INSTRUCTIONS
		op_SCD,
		op_SCR,
		op_SCL,
		op_EXIT,
		op_LOW,
		op_HIGH,
		op_DRW_VX_VY_0,
		op_LD_HF_VX,
		op_LD_R_VX,
		op_LD_VX_R,

		// reserved value for unknown opcode
		op_unknown,

		// FINAL VALUE TO GIVE NUMBER OF ITMES
		NUM_OPCODES
	};

	class Instruction {
	public:
		virtual ~Instruction();
		virtual void executeInstruction(u16 instr) = 0;
	};

	// decode the passed in word and return the appropriate Opcode value
	Opcode decodeWord(u16 word);

	class InstructionHandler {
	private:
		Instruction* instructions[NUM_OPCODES];

	public:
		InstructionHandler();
		~InstructionHandler();

		void execute(u16 instr);

	};
}

#endif

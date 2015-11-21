#include "Chip8Common.hpp"
#include "Instructions.hpp"
#include "Memory.hpp"
#include "Registers.hpp"


namespace Chip8 {
	inline bool checkBitMask(u16 opcode, u16 bitmask) {
		return (opcode & bitmask) == bitmask;
	}

	// implementations of the Instruction interface below

	// for unimplemented instructions
	class UnimplementedInstruction : public Instruction {
	public:
		void executeInstruction(u16 opcode) {
			// intentionally blank
		}
	};

	// Instruction implementations
	class AddInstruction : public Instruction {
	private:
		Registers* regs;
		Memory* mem;

	public:
		AddInstruction(Memory* memManager, Registers* regManager) {
			mem = memManager;
			regs = regManager;
		}

		void executeInstruction(u16 opcode) {
			if (checkBitMask(opcode, 0x7000)) {
				u8 x = getLowNibble(getHighByte(opcode));
				u8 kk = getLowByte(opcode);
				regs->setRegister(x, regs->getRegister(x) + kk);
			}
			else if (checkBitMask(opcode, 0x8004)) {
				u8 x = getLowNibble(getHighByte(opcode));
				u8 y = getHighNibble(getLowByte(opcode));
				u16 result = (u16) regs->getRegister(x) + (u16) regs->getRegister(y);
				regs->setRegister(x, result & 0xFF);
				regs->setRegister(0xF, result > 0xFF ? 1 : 0);
			}
			else if (checkBitMask(opcode, 0xF01E)) {
				u8 x = getLowNibble(getHighByte(opcode));
				regs->setMemAddrReg(regs->getMemAddrReg() + regs->getRegister(x));
			}
		}
	};

	class SubtractInstruction : public Instruction {
	private:
		Registers* regs;
		Memory* mem;

	public:
		SubtractInstruction(Memory* memManager, Registers* regManager) {
			mem = memManager;
			regs = regManager;
		}

		void executeInstruction(u16 opcode) {
			u8 x = getLowNibble(getHighByte(opcode));
			u8 y = getHighNibble(getLowByte(opcode));

			if (checkBitMask(opcode, 0x8005)) {
				u16 result = ((u16) regs->getRegister(x) | 0x100) - ((u16) regs->getRegister(y));
				regs->setRegister(x, result & 0xFF);
				// set VF to 1 if no borrow occurred, 0 if one did occur
				regs->setRegister(0xF, checkBitMask(result, 0x100) ? 1 : 0);
			}
			else if (checkBitMask(opcode, 0x8007)) {
				u16 result = ((u16)regs->getRegister(y) | 0x100) - ((u16)regs->getRegister(x));
				regs->setRegister(x, result & 0xFF);
				// set VF to 1 if no borrow occurred, 0 if one did occur
				regs->setRegister(0xF, checkBitMask(result, 0x100) ? 1 : 0);
			}
		}

	};

	// instruction handler method implementations
	InstructionHandler::InstructionHandler() {
		// TODO make this usable, for now it just makes every instruction unimplemented
		for (int i = 0; i < NUM_OPCODES; ++i) {
			instructions[i] = new UnimplementedInstruction;
		}
	}

	InstructionHandler::~InstructionHandler() {
		for (int i = 0; i < NUM_OPCODES; ++i) {
			delete instructions[i];
		}
	}

	void InstructionHandler::execute(u16 instr) {
		Opcode op = decodeWord(instr);
		instructions[op]->executeInstruction(instr);
	}

	Opcode decodeWord(u16 word) {
		return op_unknown;
	}
}
#include "Chip8Common.hpp"
#include "Instructions.hpp"
#include "Memory.hpp"
#include "Registers.hpp"


namespace Chip8 {
	// implementations of the Instruction interface below

	// for unimplemented instructions
	class UnimplementedInstruction : public Instruction {
	public:
		void executeInstruction(u16 opcode) {
			// intentionally blank
		}
	};

	// TODO
	// not done, just showing passing of managers into instructions
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
			// IMPLEMENT!!!!
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
#include "Chip8Common.hpp"
#include "Registers.hpp"

namespace Chip8 {
	Registers::Registers() {
		// zero out the general-purpose registers
		for (int i = 0; i < NumberOfRegs; ++i) {
			V[i] = 0;
		}

		// clear the stack
		for (int i = 0; i < StackDepth; ++i) {
			stack[i] = 0;
		}

		// set the memory addressing register
		I = 0;

		// Program counter starts at 0x200
		PC = 0x200;

		// stack pointer is initially set at 0
		SP = 0;
	}

	u8 Registers::getRegister(u8 n) {
		return n < NumberOfRegs ?
			V[n] :
			0;
	}

	void Registers::setRegister(u8 n, u8 val) {
		if (n < NumberOfRegs) {
			V[n] = val;
		}
	}

	u16 Registers::getMemAddrReg() {
		return I;
	}

	void Registers::setMemAddrReg(u16 val) {
		I = val;
	}

	u16 Registers::getProgCounter() {
		return PC;
	}

	void Registers::setProgCounter(u16 val) {
		PC = val;
	}

	u8 Registers::readStackPointer() {
		return SP;
	}

	u16 Registers::readAddressOnStack() {
		// TODO error handling
		return SP == 0 || SP > StackDepth ?
			0 :
			stack[SP - 1];
	}

	void Registers::pushToStack(u16 addr) {
		// TODO error handling
		stack[SP] = addr;
		++SP;
	}

	u16 Registers::popFromStack() {
		// TODO error handling
		--SP;
		return stack[SP];
	}
}

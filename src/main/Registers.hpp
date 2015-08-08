#ifndef CHIP8_REGISTERS_HPP
#define CHIP8_REGISTERS_HPP

namespace Chip8 {

	const int NumberOfRegs = 16;
	const int StackDepth = 16;

	// handles registers and the stack for a CHIP-8 system
	class Registers {
	private:
		u8 V[NumberOfRegs]; // general purpose regs
		u16 stack[StackDepth]; // stack
		u16 I; // memory addressing reg
		u16 PC; // program counter
		u8 SP; // stack pointer. is one higher than index of stack[]
		       // represents how many calls deep we are

	public:
		Registers();

		// getters/setters for internal registers
		u8 getRegister(u8 n);
		void setRegister(u8 n, u8 val);
		u16 getMemAddrReg();
		void setMemAddrReg(u16 val);
		u16 getProgCounter();
		void setProgCounter(u16 val);
		
		// methods for stack access
		u8 readStackPointer(); // calls deep on stack
		u16 readAddressOnStack(); // inspects the address on top of the stack. does not pop it
		void pushToStack(u16 addr); // push the address to the stack
		u16 popFromStack(); // pop the address from the stack, return the popped address
	};
}

#endif
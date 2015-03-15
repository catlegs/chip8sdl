#ifndef CHIP8CPU
#define CHIP8CPU

#include "Chip8Common.hpp"
#include "Chip8Memory.hpp"
const int NumberOfRegs = 16;
const int StackDepth = 16;

class Chip8Cpu {
private:
	u8 V[NumberOfRegs]; // general purpose regs
	u16 I; // memory addressing reg
	u16 PC; // program counter
	u8 SP; // stack pointer

	u8 DT; // delay timer
	u8 ST; // sound timer
	
	u16 freq; // cpu frequency (instructions per second, not clock ticks)

	Chip8Memory* memoryManager;
};
#endif // CHIP8CPU
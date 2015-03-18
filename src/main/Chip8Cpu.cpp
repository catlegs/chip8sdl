/*
Copyright (c) 2015 Ross Keenum (rosskeenum@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Chip8Cpu.hpp"

Chip8Cpu::Chip8Cpu(Chip8Memory &memManager, Chip8Timers &timManager, u16 frequency) {
	// zero out the registers
	for (int i = 0; i < NumberOfRegs; ++i) {
		V[i] = 0;
	}

	// clear the stack
	for (int i = 0; i < StackDepth; ++i) {
		stack[i] = 0;
	}

	// set the memory addressing register
	I = 0;

	// PC starts at 0x200
	PC = 0x200;

	// stack pointer is initially set at 0
	SP = 0;

	// set the cpu frequency
	freq = frequency;
	
	// set the other managers that this object
	// holds a reference to.
	memoryManager = &memManager;
	timerManager = &timManager;
}

u8 Chip8Cpu::readRegister(u8 n) {
	return n < NumberOfRegs ?
		V[n] :
		0;
}

u16 Chip8Cpu::readMemAddrReg() {
	return I;
}

u16 Chip8Cpu::readProgCounter() {
	return PC;
}

u8 Chip8Cpu::readStackPointer() {
	return SP;
}

u16 Chip8Cpu::readAddressOnStack() {
	// TODO error handling
	return SP == 0 || SP > StackDepth ?
		0 :
		stack[SP - 1];
}

void Chip8Cpu::pushToStack(u16 addr) {
	// TODO error handling
	stack[SP] = addr;
	++SP;
}

u16 Chip8Cpu::popFromStack() {
	// TODO error handling
	--SP;
	return stack[SP];
}

void Chip8Cpu::executeInstruction() {
	// TODO implement
}

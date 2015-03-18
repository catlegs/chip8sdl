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

#ifndef CHIP8CPU
#define CHIP8CPU

#include "Chip8Common.hpp"
#include "Chip8Memory.hpp"
#include "Chip8Timers.hpp"
const int NumberOfRegs = 16;
const int StackDepth = 16;

class Chip8Cpu {
private:
	u8 V[NumberOfRegs]; // general purpose regs
	u16 stack[StackDepth]; // stack
	u16 I; // memory addressing reg
	u16 PC; // program counter
	u8 SP; // stack pointer. is one higher than index of stack[]
	       // represents how many calls deep we are
	
	
	u16 freq; // cpu frequency (instructions per second, not clock ticks)

	// thread-safe. memory manager object. 
	// This object does NOT own it.
	// just calls it for reads/writes
	Chip8Memory* memoryManager;

	// Thread-safe timer manager object
	// This object does NOT own it, just holds
	// onto it to set the timers
	Chip8Timers* timerManager;


	// TODO Chip8Video* videoManager;;
public:
	Chip8Cpu(Chip8Memory &memManager, Chip8Timers &timManager, u16 frequency);

	// executes the instruction the PC is pointed at
	void executeInstruction();

	// getters for internal register (to inspect state)
	u8 readRegister(u8 n);
	u16 readMemAddrReg();
	u16 readProgCounter();
	u8 readStackPointer(); // calls deep on stack
	u16 readAddressOnStack(); // inspects the address on top of the stack. does not pop it

	// helper methods for stack access
	void pushToStack(u16 addr); // push the address to the stack
	u16 popFromStack(); // pop the address from the stack, return the popped address
};

#endif // CHIP8CPU
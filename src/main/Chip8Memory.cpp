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

#include "Chip8Memory.hpp"


#include <cstring> // for memset

Chip8Memory::Chip8Memory() {
	// zero out the memory
	memset(memory, 0, sizeof(memory));
}

u8 Chip8Memory::readByte(u16 address) {
	// check that we are accessing within bounds of array
	//   if in bounds, read the byte and return it
	//   if out of bounds, return NULL
	return address < Chip8MemorySize ?
		memory[address] :
		0;
}

bool Chip8Memory::writeByte(u16 address, u8 value) {
	if (address < Chip8MemorySize) {
		mutexForWrites.lock();
		memory[address] = value;
		mutexForWrites.unlock();
		return true;
	}
	else {
		return false;
	}
}

u16 Chip8Memory::readWord(u16 address) {
	return address < Chip8MemorySize - 1 ?
		u16((memory[address] << 8) | memory[address + 1]) :
		0;
}

bool Chip8Memory::writeWord(u16 address, u16 value) {
	if (address < Chip8MemorySize - 1) {
		u8 msb = u8(value >> 8);
		u8 lsb = u8(0xFF & value);
		
		mutexForWrites.lock();
		// CHIP8 is big-endian
		memory[address] = msb;
		memory[address + 1] = lsb;
		mutexForWrites.unlock();
		
		return true;
	}
	else {
		return false;
	}
}

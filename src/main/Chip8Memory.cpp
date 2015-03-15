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
		NULL;
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
		NULL;
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

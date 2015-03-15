#ifndef CHIP8MEMORY_HPP
#define CHIP8MEMORY_HPP

#include <mutex>
#include "Chip8Common.hpp"


const int Chip8MemorySize = 0x1000;

class Chip8Memory {
private:
	u8 memory[Chip8MemorySize];
	std::mutex mutexForWrites;

public:
	Chip8Memory();

	u8 readByte(u16 address);
	bool writeByte(u16 address, u8 value);
	u16 readWord(u16 address);
	bool writeWord(u16 address, u16 value);
};

#endif // CHIP8MEMORY_HPP

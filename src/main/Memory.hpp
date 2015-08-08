#ifndef CHIP8_MEMORY_HPP
#define CHIP8_MEMORY_HPP

#include <mutex>
#include "Chip8Common.hpp"

namespace Chip8 {
	const int MemorySize = 0x1000;

	class Memory {
	private:
		u8 memory[MemorySize];
		std::mutex memLock;

	public:
		Memory();

		u8 readByte(u16 address);
		bool writeByte(u16 address, u8 value);
		u16 readWord(u16 address);
		bool writeWord(u16 address, u16 value);
	};
}

#endif
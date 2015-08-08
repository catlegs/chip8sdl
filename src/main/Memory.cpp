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

#include "Chip8Common.hpp"
#include "Memory.hpp"


#include <cstring> // for memset

namespace Chip8 {
	Memory::Memory() {
		// zero out the memory
		memset(memory, 0, sizeof(memory));
	}

	u8 Memory::readByte(u16 address) {
		// check that we are accessing within bounds of array
		//   if in bounds, read the byte and return it
		//   if out of bounds, return NULL
		memLock.lock();
		u8 result = address < MemorySize ?
			memory[address] :
			0;
		memLock.unlock();
		return result;
	}

	bool Memory::writeByte(u16 address, u8 value) {
		if (address < MemorySize) {
			memLock.lock();
			memory[address] = value;
			memLock.unlock();
			return true;
		}
		else {
			return false;
		}
	}

	u16 Memory::readWord(u16 address) {
		memLock.lock();
		u16 result = address < MemorySize - 1 ?
			u16((memory[address] << 8) | memory[address + 1]) :
			0;
		memLock.unlock();
		return result;
	}

	bool Memory::writeWord(u16 address, u16 value) {
		if (address < MemorySize - 1) {
			u8 msb = u8(value >> 8);
			u8 lsb = u8(0xFF & value);

			memLock.lock();
			// CHIP8 is big-endian
			memory[address] = msb;
			memory[address + 1] = lsb;
			memLock.unlock();

			return true;
		}
		else {
			return false;
		}
	}
}
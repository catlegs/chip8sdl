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

#include "catch.hpp"
#include "Chip8Common.hpp"
#include "Memory.hpp"
#include <iostream>

using Chip8::Memory;
using Chip8::MemorySize;
using Chip8::u8;
using Chip8::u16;

TEST_CASE("Memory/constructor") {
	Memory obj;

	// assert that first and last memory
	//   location are set to zero
	REQUIRE(obj.readByte(0) == 0);
	REQUIRE(obj.readByte(MemorySize - 1) == 0);
}

TEST_CASE("Memory/readByte") {
	Memory obj;

	SECTION("out of bounds, should return 0") {
		// fill memory with 0x1F
		for (int i = 0; i < MemorySize; ++i) {
			obj.writeByte(i, 0x1F);
		}

		// execute
		u8 result = obj.readByte(MemorySize);

		// assert
		REQUIRE(result == 0);
	}

	SECTION("in bounds, should return 0x1C") {
		// write 0x1C to memory
		obj.writeByte(0x543, 0x1C);

		// execute
		u8 result = obj.readByte(0x543);

		// assert
		REQUIRE(result == 0x1C);
	}
}

TEST_CASE("Memory/writeByte") {
	Memory obj;

	SECTION("out of bounds, write should fail") {
		// write out of bounds
		bool result = obj.writeByte(MemorySize, 0xFF);

		REQUIRE(result == false);
	}

	SECTION("in bounds, write should succeed") {
		// write in bounds
		bool result = obj.writeByte(0x31, 0xFC);
		u8 writtenVal = obj.readByte(0x31);

		REQUIRE(result == true);
		REQUIRE(writtenVal == 0xFC);
	}
}


TEST_CASE("Memory/readWord") {
	Memory obj;

	SECTION("out of bounds, should return 0") {
		// fill memory with 0x1F
		for (int i = 0; i < MemorySize; ++i) {
			obj.writeByte(i, 0x1F);
		}

		// execute
		u16 result = obj.readWord(MemorySize - 1);

		// assert
		REQUIRE(result == 0);
	}

	SECTION("in bounds, should return 0xDEAD") {
		// write 0xDEAD to memory
		obj.writeWord(0x543, 0xDEAD);

		// execute
		u16 result = obj.readWord(0x543);

		// assert
		REQUIRE(result == 0xDEAD);
	}
}
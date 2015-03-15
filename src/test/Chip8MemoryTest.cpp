
#include "catch.hpp"
#include "Chip8Memory.hpp"
#include <iostream>

TEST_CASE("Chip8Memory/constructor") {
	Chip8Memory obj;

	for (u16 i = 0; i < Chip8MemorySize; ++i) {
		u8 result = obj.readByte(i);
		REQUIRE(result == 0);
	}
}

TEST_CASE("Chip8Memory/readByte") {
	Chip8Memory obj;

	SECTION("out of bounds, should return NULL") {
		// fill memory with 0x1F
		for (u16 i = 0; i < Chip8MemorySize; ++i) {
			obj.writeByte(i, 0x1F);
		}

		// execute
		u8 result = obj.readByte(Chip8MemorySize);

		// assert
		REQUIRE(result == NULL);
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

TEST_CASE("Chip8Memory/writeByte") {
	Chip8Memory obj;

	SECTION("out of bounds, write should fail") {
		// write out of bounds
		bool result = obj.writeByte(Chip8MemorySize, 0xFF);

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


TEST_CASE("Chip8Memory/readWord") {
	Chip8Memory obj;

	SECTION("out of bounds, should return NULL") {
		// fill memory with 0x1F
		for (u16 i = 0; i < Chip8MemorySize; ++i) {
			obj.writeByte(i, 0x1F);
		}

		// execute
		u16 result = obj.readWord(Chip8MemorySize - 1);

		// assert
		REQUIRE(result == NULL);
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
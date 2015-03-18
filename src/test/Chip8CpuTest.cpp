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
#include "Chip8Cpu.hpp"

TEST_CASE("Chip8Cpu/constructor") {
	Chip8Memory memManager;
	Chip8Timers timManager;

	Chip8Cpu obj(memManager, timManager, 800);

	SECTION("Check initial values") {
		REQUIRE(obj.readProgCounter() == 0x200);
	}
}

TEST_CASE("Chip8Cpu/pushAndPopStack") {
	Chip8Memory memManager;
	Chip8Timers timManager;

	Chip8Cpu obj(memManager, timManager, 800);

	SECTION("Initial stack state") {
		REQUIRE(obj.readAddressOnStack() == 0);
		REQUIRE(obj.readStackPointer() == 0);
	}

	SECTION("Check push stack") {
		obj.pushToStack(0xDEAD);
		REQUIRE(obj.readAddressOnStack() == 0xDEAD);
		REQUIRE(obj.readStackPointer() == 1);
	}

	SECTION("Check pop stack") {
		obj.pushToStack(0xDEAD);

		REQUIRE(obj.readStackPointer() == 1);

		u16 result = obj.popFromStack();

		REQUIRE(result == 0xDEAD);
		REQUIRE(obj.readStackPointer() == 0);
	}
}
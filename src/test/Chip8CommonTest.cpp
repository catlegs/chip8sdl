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

#include <thread>
#include <chrono>

using Chip8::invokeFuncAtInterval;
using Chip8::getHighByte;
using Chip8::getHighNibble;
using Chip8::getLowByte;
using Chip8::getLowNibble;
using Chip8::allocate2dBuffer;
using Chip8::destroy2dBuffer;
using Chip8::copy2dBuffer;
using Chip8::u8;
using Chip8::u16;

// test function + values for the function we're testing
bool testFuncBreakVal;
int testFuncCountVal;

bool testFunc() {
	if (testFuncBreakVal) {
		++testFuncCountVal;
	}

	return testFuncBreakVal;
}

void sleepForMillis(long millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

// using 20 executions per second in these tests
// the first execution is executed right when the thread is created,
//   so we're sleeping for 25ms less in this thread
TEST_CASE("Chip8Common/invokeFuncAtInterval", "[threading]") {
	testFuncBreakVal = true;
	testFuncCountVal = 0;

	SECTION("Should be executed once") {
		std::thread t(&invokeFuncAtInterval, &testFunc, 20L);
		sleepForMillis(25);
		testFuncBreakVal = false;
		t.join();

		REQUIRE(testFuncCountVal == 1);
	}

	SECTION("Should be executed twice") {
		std::thread t(&invokeFuncAtInterval, &testFunc, 20L);
		sleepForMillis(75);
		testFuncBreakVal = false;
		t.join();

		REQUIRE(testFuncCountVal == 2);
	}
}

TEST_CASE("Chip8Common/byteWordAndNibbleOperations", "[inline]") {

	SECTION("getHighNibble") {
		u8 result = getHighNibble(0xDE);

		REQUIRE(result == 0xD);
	}

	SECTION("getLowNibble") {
		u8 result = getLowNibble(0xDE);

		REQUIRE(result == 0xE);
	}

	SECTION("getHighByte") {
		u8 result = getHighByte(0xDEAD);

		REQUIRE(result == 0xDE);
	}

	SECTION("getLowByte") {
		u8 result = getLowByte(0xDEAD);

		REQUIRE(result == 0xAD);
	}
}

TEST_CASE("Chip8Common/bufferOperations") {
	u8** buffer;

	// allocate the buffer
	buffer = allocate2dBuffer(2, 10);
	buffer[1][9] = 8;

	REQUIRE(buffer[1][9] == 8);

	SECTION("destroyBuffer/should succeed") {
		destroy2dBuffer(buffer, 2);
		// cannot dereference since dereferencing an invalid pointer is undefined
	}

	SECTION("copyBuffer/should hold the same values at different addresses") {
		u8** bufferCopy = allocate2dBuffer(2, 10);
		copy2dBuffer(buffer, bufferCopy, 2, 10);

		REQUIRE(bufferCopy[1][9] == 8);
		REQUIRE(&buffer[1][9] != &bufferCopy[1][9]);
	}
}
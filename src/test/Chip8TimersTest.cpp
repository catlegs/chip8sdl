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
#include "Chip8Timers.hpp"
#include <iostream>

#include <thread>
#include <chrono>
#include <iostream>

TEST_CASE("Chip8Timers/constructor") {
	Chip8Timers obj;

	SECTION("Delay timer should be zero") {
		u8 result = obj.readDelayTimer();

		REQUIRE(result == 0);
	}

	SECTION("Sound timer should be zero") {
		u8 result = obj.readSoundTimer();

		REQUIRE(result == 0);
	}

	SECTION("Thread should not be started") {
		bool result = obj.isStarted();

		REQUIRE(result == false);
	}
}

TEST_CASE("Chip8Timers/updateTimers") {
	Chip8Timers obj;

	SECTION("timers are set to zero") {
		obj.updateTimers();

		u8 DTresult = obj.readDelayTimer();
		u8 STresult = obj.readSoundTimer();

		REQUIRE(DTresult == 0);
		REQUIRE(STresult == 0);
	}
}
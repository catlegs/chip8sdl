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

#ifndef CHIP8_COMMON
#define CHIP8_COMMON

#include <cinttypes>
#include <functional>
#include <chrono>

namespace Chip8 {
	// define width-based integer types
	typedef uint_least8_t u8;
	typedef int_least8_t s8;
	typedef uint_least16_t u16;
	typedef int_least16_t s16;
	typedef uint_least32_t u32;
	typedef int_least32_t s32;

	// functions for woking with bytes, words, and nibbles
	inline u8 getHighNibble(u8 byte) { return byte >> 4; }
	inline u8 getLowNibble(u8 byte) { return byte & 0xF; }
	inline u8 getHighByte(u16 word) { return word >> 8; }
	inline u8 getLowByte(u16 word) { return word & 0xFF; }

	// invokes a passed in function at the specified times per second until it returns false
	// should ALWAYS be passed into a thread
	void invokeFuncAtInterval(std::function<bool()> func, unsigned long timesPerSecond);
}

#endif // CHIP8_COMMON
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
#include <chrono>
#include <thread>
#include <cstring>

namespace Chip8 {
	// invokes a passed in function at the specified times per second until it returns false
	void invokeFuncAtInterval(std::function<bool()> func, unsigned long timesPerSecond) {
		// get the duration between the invocations in nanoseconds
		std::chrono::nanoseconds interval(1000000000ULL / timesPerSecond);

		// priming read for the next invocation of the function
		auto nextInvocation = std::chrono::high_resolution_clock::now();

		// infinite loop until the function returns false
		while (func()) {
			// function returned true
			// calculate when the next time to invoke it is
			nextInvocation += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(interval);

			// put the thread to sleep until the nex invocation
			std::this_thread::sleep_until(nextInvocation);
		}
	}

	u8** allocate2dBuffer(unsigned int width, unsigned int height) {
		u8** buf = new u8*[width];
		for (unsigned int i = 0; i < width; ++i) {
			buf[i] = new u8[height];
		}
		return buf;
	}

	void destroy2dBuffer(u8** buf, unsigned int width) {
		for (unsigned int i = 0; i < width; ++i) {
			delete[] buf[i];
		}
		delete[] buf;
	}

	void copy2dBuffer(u8** source, u8** dest, unsigned int width, unsigned int height) {
		for (unsigned int i = 0; i < width; ++i) {
			memcpy(dest[i], source[i], sizeof(u8) * height);
		}
	}
}
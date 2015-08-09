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
#include "Video.hpp"

using Chip8::Video;
using Chip8::VideoMode;
using Chip8::u8;

// driver implementation that simply copys the buffer
class TestVideoDriver : public Chip8::VideoDriver {
public:
	u8** outputBuf;
	VideoMode mode;

	TestVideoDriver(VideoMode vidMode) {
		const Chip8::ScreenDimensions* dims = &Chip8::VideoModeDimensions[vidMode];
		outputBuf = Chip8::allocate2dBuffer(dims->width, dims->height);
		mode = vidMode;
	}
	
	~TestVideoDriver() {
		const Chip8::ScreenDimensions* dims = &Chip8::VideoModeDimensions[mode];
		Chip8::destroy2dBuffer(outputBuf, dims->width);
	}

	void updateScreen(u8** buf, VideoMode vidMode) {
		const Chip8::ScreenDimensions* dims = &Chip8::VideoModeDimensions[vidMode];
		
		if (mode != vidMode) {
			Chip8::destroy2dBuffer(outputBuf, dims->width);
			outputBuf = Chip8::allocate2dBuffer(dims->width, dims->height);
			mode = vidMode;
		}

		// copy the buffer
		Chip8::copy2dBuffer(buf, outputBuf, dims->width, dims->height);
	}
};

// TODO WRITE TESTS
TEST_CASE("Video/constructor") {
	TestVideoDriver driver(Chip8::vid_CHIP8);
	Video obj(&driver, Chip8::vid_CHIP8);

	SECTION("not a test at all") {
		REQUIRE(0 == 0);
	}
}
